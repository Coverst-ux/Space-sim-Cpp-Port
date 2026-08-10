#pragma once
#include "vector3d.h"
#include <vector>
#include "body.h"


vector3d gravitational_force_softened(float m1, float m2, vector3d pos1, vector3d pos2);
double compute_total_energy(const std::vector<Body>& bodies);