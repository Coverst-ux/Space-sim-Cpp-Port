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

    vector3d radial_direction = position.normalized(); //p-hat (p^)
    vector3d magnetic_axis = config.magnetic_axis.normalized(); //m-hat (m^)

    double alignment = magnetic_axis.dot(radial_direction);

    vector3d radial_term = radial_direction.mult(alignment * 3.0);
    vector3d direction_term = radial_term.sub(magnetic_axis);

    double distance_ratio = config.stellar_radius / d;
    double distance_scale = std::pow(distance_ratio, 3);

    double field_scale = (config.polar_field_strength / 2.0) * distance_scale;
    vector3d magnetic_field = direction_term.mult(field_scale);

    return magnetic_field;
}