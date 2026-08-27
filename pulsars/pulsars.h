#pragma once
#include "vector3d.h"
struct PulsarConfig {
    // changed only by preset or advanced configuration option
    double stellar_radius;
    // wont be set by users, fixed
    double speed_of_light;
    // can be seen and changed by users
    double polar_field_strength;
    // set based off of user's chosen rotation period 
    double omega;

    vector3d magnetic_axis;
};

// Returns the time required for one full rotation from the angular velocity
double rotation_period(double omega);
// Returns the radius for the light cylinder of the pulsar
double light_cylinder_radius(double speed_of_light, double omega);
// Returns a 3D vector called B measured in tesla
vector3d get_magnetic_field(PulsarConfig& config, vector3d& position);