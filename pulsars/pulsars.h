#pragma once
#include "vector3d.h"
struct PulsarConfig {
    // changed only by preset or advanced configuration option
    // unit: meters
    double stellar_radius;
    // wont be set by users, fixed
    // unit: meters per second
    double speed_of_light;
    // can be seen and changed by users
    // unit: tesla
    double polar_field_strength;
    // set based off of user's chosen rotation period 
    double omega;
    // radians per second
    double alpha;
    // direction vector
    vector3d magnetic_axis;
    
    double mass;

    double phase;
};

// Returns the time required for one full rotation from the angular velocity
double rotation_period(double omega);
// Returns the radius for the light cylinder of the pulsar
double light_cylinder_radius(double speed_of_light, double omega);
// Returns a 3D vector called B measured in tesla
vector3d get_magnetic_field(const PulsarConfig& config, const vector3d& position); // position unit: meters relative to the pulsar's center
// Returns the rotating magnetic axis at time t
vector3d rotating_magnetic_axis(double alpha, double phase);
// Returns the rate of which the pulsar's angular velocity decreases due to the magnetic radiation
double spin_down_rate(const PulsarConfig& config);
void update_spin(PulsarConfig& config, double dt);
double dipole_radiation_power(const PulsarConfig& config);

void update_phase(PulsarConfig& config, double dt);
void update_pulsar(PulsarConfig& config, double dt);