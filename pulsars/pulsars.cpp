#include "pulsars.h"
#include <stdexcept>
#include "vector3d.h"
#include <cmath>


constexpr double PI = 3.14159265358979;


double rotation_period(double omega) {
    if (omega > 0) {
        return 2.0 * PI / omega;
    }
    else{
        throw std::invalid_argument("Omega must be greater than zero");
    }
}

double light_cylinder_radius(double speed_of_light, double omega) {
    if (omega > 0) {
        return speed_of_light / omega;
    }
    else{
        throw std::invalid_argument("Omega must be greater than zero");
        
    }
    
}


vector3d get_magnetic_field(const PulsarConfig& config, const vector3d& position) {
    double d = position.magnitude(); // distance from the center
    if (d == 0){
        throw std::invalid_argument("d must be greater than zero");
    }

    if (d < config.stellar_radius) {
        throw std::invalid_argument(
            "Position cannot be inside the star"
        );
    }

    vector3d magnetic_axis = rotating_magnetic_axis(config.alpha, config.phase); //m-hat (m^)
    vector3d radial_direction = position.normalized(); //p-hat (p^)
    vector3d magnetic_axis_normalized = magnetic_axis.normalized(); 

    double alignment = magnetic_axis_normalized.dot(radial_direction);

    vector3d radial_term = radial_direction.mult(alignment * 3.0);
    vector3d direction_term = radial_term.sub(magnetic_axis_normalized);

    double distance_ratio = config.stellar_radius / d;
    double distance_scale = std::pow(distance_ratio, 3);

    double field_scale = (config.polar_field_strength / 2.0) * distance_scale;
    vector3d magnetic_field = direction_term.mult(field_scale);

    return magnetic_field;
}

vector3d rotating_magnetic_axis(double alpha, double phase){
    double sideways = std::sin(alpha);
    double vertical = std::cos(alpha);


    double x_coord = sideways * std::cos(phase);
    double y_coord = sideways  * std::sin(phase);
    double z_coord = vertical;

    

    return vector3d(x_coord, y_coord, z_coord);
}


constexpr double MU_0 = 1.25663706212e-6;

double spin_down_rate(const PulsarConfig& config){
    if (config.mass <= 0){
        throw std::invalid_argument("Mass cannot be non positive");
    }
    if (config.stellar_radius <= 0){
        throw std::invalid_argument("Radius cannot be non positive");
    }
    if (config.speed_of_light <= 0){
        throw std::invalid_argument("Speed of light cannot be non positive");
    }
    if (config.omega < 0){
        throw std::invalid_argument("Omega cannot be negative");
    }
    
    
    double moment_of_inertia = 0.35 * config.mass * pow(config.stellar_radius, 2);
    double numerator = 2 * PI * pow(config.polar_field_strength, 2) * pow(config.stellar_radius, 6) * pow(config.omega, 3) * pow(sin(config.alpha), 2);
    double denominator = 3 * MU_0 * moment_of_inertia * pow(config.speed_of_light, 3);

    return (numerator / denominator) * -1;
}

void update_spin(PulsarConfig& config, double dt){
    if (config.omega < 0){
        config.omega = 0;
    }
    config.omega += spin_down_rate(config) * dt;
}

double dipole_radiation_power(const PulsarConfig& config){
    if (config.stellar_radius <= 0){
        throw std::invalid_argument("The radius must be positive");
    }

    if (config.speed_of_light <= 0){
        throw std::invalid_argument("The speed of light must be positive");
    }

    if (config.omega < 0){
        throw std::invalid_argument("The omega cannot be negative");
    }

    if (config.polar_field_strength < 0){
        throw std::invalid_argument("The polar field strength must be positive");
    }

    double numerator = 2 * PI * pow(config.polar_field_strength, 2) * pow(config.stellar_radius, 6) * pow(config.omega, 4) * pow(sin(config.alpha), 2);
    double denominator = 3 * MU_0 * pow(config.speed_of_light, 3);

    return numerator/denominator;
}

void update_phase(PulsarConfig& config, double dt){
    config.phase += config.omega * dt;
}

void update_pulsar(PulsarConfig& config, double dt){
    update_phase(config, dt);
    update_spin(config, dt);
}