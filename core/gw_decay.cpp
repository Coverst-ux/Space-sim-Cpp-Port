#include "gw_decay.h"
#include "constants.h"
#include <cmath>
#include "vector3d.h"
binarystate compute_orbital_elements(const Body& body1, const Body& body2, int binary_id){
    // takes two bodies position and velocity and calculates the shape of their orbit
    
    vector3d r_vec = body2.position.sub(body1.position);
    vector3d v_vec = body2.velocity.sub(body1.velocity);
    
    double r  = r_vec.magnitude();
    double v  = v_vec.magnitude();

    double mu =  G * (body1.mass + body2.mass);
    double epsilon = (v*v) / 2.0 - mu / r;
    
    vector3d h_vec = r_vec.cross(v_vec); 
    double h = h_vec.magnitude();
    
    double a = -mu / (2.0 * epsilon);
    double e_squared_term = 1.0 + (2.0 * epsilon * h * h) / (mu * mu);
    e_squared_term = std::max(0.0, e_squared_term);  // guard against floating-point cancellation on near-circular orbits
    double e = std::sqrt(e_squared_term);
    
    binarystate state;
    state.binary_id = binary_id; 
    state.a = a;
    state.e = e;
    state.nu = 0.0;
    
    return state;
}

double compute_initial_true_anomaly(const Body& body1, const Body& body2, const binarystate& state) {
    vector3d r_vec = body2.position.sub(body1.position);
    double nu = std::atan2(r_vec.y, r_vec.x);
    return nu;
}

void decay_orbital_elements(binarystate& state, double m1, double m2, double dt, double speedup) {
    double e2 = state.e * state.e;
    double e4 = e2 * e2;
    double G3 = G*G*G;

    double numerator_a = 64.0 * G3 * m1 * m2 * (m1+m2);
    double denominator_a = 5.0 * pow(c, 5) * pow(state.a, 3) * pow((1-e2), 3.5);
    double correction_a = 1.0 + (73.0/24.0) * e2 + (37.0/96.0) * e4;
    double da_dt = -(numerator_a / denominator_a) * correction_a;

    double numerator_e = 304.0 * state.e * G3 * m1 * m2 * (m1+m2);
    double denominator_e = 15.0 * pow(c, 5) * pow(state.a, 4) * pow((1-e2), 2.5);
    double correction_e = 1.0 + (121.0/304.0) * e2;
    double de_dt = -(numerator_e / denominator_e) * correction_e;

    double da = da_dt * dt * speedup;
    double max_da = -0.01 * state.a;  // don't let a shrink by more than 1% in a single step
    if (da < max_da) da = max_da;

    state.a = state.a + da;
    state.e = state.e + de_dt * dt * speedup;
}

void advance_true_anomaly(binarystate& state, double m1, double m2, double dt){
    double e2 = state.e * state.e;
    double mu = G*(m1+m2);
    double h = std::sqrt(mu * state.a * (1- e2));
    double distance_between_bodies = state.a * (1-e2)/ (1+ state.e * cos(state.nu));
    double dv_dt = h / (distance_between_bodies * distance_between_bodies);

    double dnu = dv_dt * dt;
    const double max_dnu = 0.05;  // radians — cap how far nu can advance in a single step
    if (dnu > max_dnu) dnu = max_dnu;

    state.nu = state.nu + dnu;
}

void apply_orbital_state(Body& body1, Body& body2, const binarystate& state){
    // updates the position and velocity of the bodies into a form the integrator can read
    double e2 = state.e * state.e;
    double distance_between_bodies = state.a * (1-e2)/ (1+ state.e * cos(state.nu));
    
    double mu = G*(body1.mass+body2.mass);
    double v_squared = mu * (2.0 / distance_between_bodies - 1.0 / state.a);
    
    double h = std::sqrt(mu * state.a * (1- e2));
    
    double v_radial = (mu/h) * state.e * sin(state.nu);
    double v_tangential = (mu/h) * (1.0 + state.e * cos(state.nu));
    
    vector3d rel_pos(distance_between_bodies * cos(state.nu), distance_between_bodies * sin(state.nu), 0.0);
    vector3d rel_vel(
        v_radial * cos(state.nu) - v_tangential * sin(state.nu),
        v_radial * sin(state.nu) + v_tangential * cos(state.nu),
        0
    );
    
    double total_mass = body1.mass + body2.mass;
    
    vector3d center_of_mass = (body1.position.mult(body1.mass / total_mass))
    .add(body2.position.mult(body2.mass / total_mass));
    
    vector3d pos_offset1 = rel_pos.mult(-body2.mass / total_mass);
    vector3d pos_offset2 = rel_pos.mult(body1.mass / total_mass);
    
    body1.position = center_of_mass.add(pos_offset1);
    body2.position = center_of_mass.add(pos_offset2);
    
    vector3d vel_offset1 = rel_vel.mult(-body2.mass / total_mass);
    vector3d vel_offset2 = rel_vel.mult(body1.mass / total_mass);
    
    vector3d center_of_mass_velocity = (body1.velocity.mult(body1.mass / total_mass)).add(body2.velocity.mult(body2.mass / total_mass));
    body1.velocity = center_of_mass_velocity.add(vel_offset1);  
    body2.velocity = center_of_mass_velocity.add(vel_offset2);
}


binarystate create_binary_state(const Body& body1, const Body& body2, int binary_id, int idx1, int idx2, double m1, double m2) {
    binarystate state = compute_orbital_elements(body1, body2, binary_id);
    state.nu = compute_initial_true_anomaly(body1, body2, state);
    state.m1 = m1;
    state.m2 = m2;
    state.idx1 = idx1;
    state.idx2 = idx2;
    state.initial_separation = state.a;  // reference distance for speedup scaling
    binaries.push_back(state);
    return state;
}

std::vector<binarystate> binaries;

const double SPEEDUP_FAR = 1e7;     // far from merger, fast-forward through slow decay
const double SPEEDUP_NEAR = 1e4;    // near merger, much slower, watchable final approach

const int SETTLE_FRAMES = 1200;  // 20 seconds visually for it to merge    

void update_binaries(std::vector<Body>& bodies, double dt) {
    static int call_count = 0;
    call_count++;

    for (auto& state : binaries) {
        if (state.merged) continue;

        Body& body1 = bodies[state.idx1];
        Body& body2 = bodies[state.idx2];

        if (state.merging) {
            state.settle_frames_elapsed = state.settle_frames_elapsed + 1;

            double pull_strength = (double)state.settle_frames_elapsed / SETTLE_FRAMES;
            body2.position = lerp(state.settle_start_position, body1.position, pull_strength);


            if (state.settle_frames_elapsed >= SETTLE_FRAMES) {
                body1.velocity = state.pending_velocity;
                body1.mass     = state.pending_mass;
                body1.radius   = state.pending_radius;
                body1.binary_id = -1;
                body1.just_merged = true;

                body2.position = body1.position;
                body2.active = false;
                body2.binary_id = -1;

                state.merging = false;
                state.merged = true;

                continue;
            }
            continue;
        }

        double e2 = state.e * state.e;
        double current_separation = state.a * (1 - e2) / (1 + state.e * cos(state.nu));
        double contact_distance = body1.radius + body2.radius;

        double range = state.initial_separation - contact_distance;
        double closeness_ratio = (current_separation - contact_distance) / range;
        closeness_ratio = std::max(0.0, std::min(1.0, closeness_ratio));

        double speedup = SPEEDUP_NEAR + (SPEEDUP_FAR - SPEEDUP_NEAR) * closeness_ratio;


        if (current_separation <= contact_distance) {
            double total_mass = body1.mass + body2.mass;

            vector3d combined_momentum = (body1.velocity.mult(body1.mass)).add(body2.velocity.mult(body2.mass));

            state.pending_velocity = combined_momentum.mult(1.0 / total_mass);
            state.pending_mass = total_mass;
            state.pending_radius = cbrt(pow(body1.radius, 3) + pow(body2.radius, 3));

            state.settle_start_position = body2.position;

            state.merging = true;
            state.settle_frames_elapsed = 0;

            continue;
        }

        decay_orbital_elements(state, state.m1, state.m2, dt, speedup);

        if (state.a < 0.0 || !std::isfinite(state.a)) state.a = contact_distance * 0.5;
        if (state.e < 0.0 || !std::isfinite(state.e)) state.e = 0.0;
        if (state.e >= 1.0) state.e = 0.999;

        advance_true_anomaly(state, state.m1, state.m2, dt);
        apply_orbital_state(body1, body2, state);
    }
}