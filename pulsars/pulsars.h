#pragma once
struct PulsarConfig {
    double stellar_radius;
    double speed_of_light;
    double polar_field_strength;
    double omega;
};


double rotation_period(double omega);
double light_cylinder_radius(double speed_of_light, double omega);
