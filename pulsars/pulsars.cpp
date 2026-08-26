#include "pulsars.h"
#include <stdexcept>

PulsarConfig config{1.0, 1.0, 1.0, 0.1};
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


double period = rotation_period(config.omega);
double light_cylinder = light_cylinder_radius(config.speed_of_light, config.omega);
