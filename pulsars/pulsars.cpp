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


vector3d get_magnetic_field(const PulsarConfig& config, const vector3d& position, double t ) {
    double d = position.magnitude(); // distance from the center
    if (d == 0){
        throw std::invalid_argument("d must be greater than zero");
    }

    if (d < config.stellar_radius) {
        throw std::invalid_argument(
            "Position cannot be inside the star"
        );
    }

    vector3d magnetic_axis = rotating_magnetic_axis(config.alpha, config.omega, t); //m-hat (m^)
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

vector3d rotating_magnetic_axis(double alpha, double omega, double t){
    double phase = omega * t;
    double sideways = std::sin(alpha);
    double vertical = std::cos(alpha);
    double x_coord = sideways * std::cos(phase);
    double y_coord = sideways  * std::sin(phase);
    double z_coord = vertical;

    

    return vector3d(x_coord, y_coord, z_coord);
}