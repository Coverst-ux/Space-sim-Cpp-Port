#pragma once
#include <vector>
#include "body.h"

void euler_step(std::vector<Body>& bodies, float dt);
void update_forces(std::vector<Body>& bodies);
void leapfrog_step(std::vector<Body>& bodies, float dt, bool is_first_step = false);