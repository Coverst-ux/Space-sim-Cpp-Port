#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <omp.h>
#include "vector3d.h"
#include "constants.h"
#include "body.h"
#include "gravity.h"
#include "octnode.h"

void euler_step (std::vector<Body>& bodies, double dt) {
    // purely for research purposes, not actually used anywhere in the project
   std::vector<vector3d> forces(bodies.size(), vector3d(0, 0, 0));
   for (int i = 0; i < bodies.size(); i++) {
    for (int j = 0; j < bodies.size(); j++) {
        // calculates the gravitational force and adds them to the bodies 
        if (i >= j) {
            continue;
        }
        vector3d f = gravitational_force_softened(bodies[i].mass, bodies[j].mass, bodies[i].position, bodies[j].position);

        forces[i] = forces[i].add(f);
        forces[j] = forces[j].add(f.mult(-1));

            
    }
   }
   for (int i = 0; i < bodies.size(); i++) {
        // implements the euler integration by drifting (approximates) then kicks (applies the approximation)
        vector3d acceleration = forces[i].mult(1 / bodies[i].mass);
        vector3d old_velocity = bodies[i].velocity;
        bodies[i].position = bodies[i].position.add(old_velocity.mult(dt));
        bodies[i].velocity = bodies[i].velocity.add(acceleration.mult(dt));

   };}

// void update_forces_bruteforce(std::vector<Body>& bodies) {
// OLD VERSION, RESEARCH PURPOSES
//     int num_threads = omp_get_max_threads();
//     std::vector<vector3d> forces(bodies.size(), vector3d(0,0,0));
//     std::vector<std::vector<vector3d>> thread_forces(
//         num_threads,
//         std::vector<vector3d>(bodies.size(), vector3d(0,0,0))
//     );
//     #pragma omp parallel for 
//     for (int i = 0; i < bodies.size(); i++ ) {
//         int thread_id = omp_get_thread_num();
//         for (int j = 0; j < bodies.size(); j++) {
//         // calculates the gravitational force and adds them to the bodies while also using threads for performance 
//             if (i >= j) {
//                 continue; 
//             }
//             vector3d f = gravitational_force_softened(bodies[i].mass, bodies[j].mass, bodies[i].position, bodies[j].position);
//             thread_forces[thread_id][i] = thread_forces[thread_id][i].add(f);
//             thread_forces[thread_id][j] = thread_forces[thread_id][j].add(f.mult(-1));
//         }
//     }
    
//     for (int k = 0; k < bodies.size(); k++) {
//         for (int t = 0; t < num_threads; t++) {
//             forces[k] = forces[k].add(thread_forces[t][k]);
//         }
//     bodies[k].acceleration = forces[k].mult(1 / bodies[k].mass);
//     }
// }

void update_forces(std::vector<Body>& bodies) {
    auto t0 = std::chrono::high_resolution_clock::now();

    double theta = 0.5;

    // Find bounding box
    double min_x = bodies[0].position.x, max_x = bodies[0].position.x;
    double min_y = bodies[0].position.y, max_y = bodies[0].position.y;
    double min_z = bodies[0].position.z, max_z = bodies[0].position.z;



    for (const auto& b : bodies) {
        if (!b.active)
            continue;
        min_x = std::min(min_x, b.position.x);
        max_x = std::max(max_x, b.position.x);
        min_y = std::min(min_y, b.position.y);
        max_y = std::max(max_y, b.position.y);
        min_z = std::min(min_z, b.position.z);
        max_z = std::max(max_z, b.position.z);
    }

    double center_x = (min_x + max_x) / 2.0;
    double center_y = (min_y + max_y) / 2.0;
    double center_z = (min_z + max_z) / 2.0;

    double span = std::max({max_x - min_x, max_y - min_y, max_z - min_z});
    double octree_size = span * 1.1; // small padding so edge bodies aren't exactly on the boundary


    octnode root(center_x, center_y, center_z, octree_size);
    
    for (const auto& b : bodies) {
        if (!b.active)
            continue;

        root.insert(b);
    }


    root.update_mass();
    


    for (auto& b : bodies){
        if (!b.active)
            continue;

        vector3d force = root.calculate_forces(b, theta);
        b.acceleration = force.mult(1.0 / b.mass);
    }


for (int i = 0; i < (int)bodies.size(); i++) {
    if (!bodies[i].active || bodies[i].binary_id == -1)
        continue;

    for (int j = i + 1; j < (int)bodies.size(); j++) {
        if (!bodies[j].active)
            continue;

        if (bodies[j].binary_id == bodies[i].binary_id) {
            vector3d f = gravitational_force_softened(
                bodies[i].mass,
                bodies[j].mass,
                bodies[i].position,
                bodies[j].position
            );

            bodies[i].acceleration =
                bodies[i].acceleration.sub(
                    f.mult(1. / bodies[i].mass)
                );

            bodies[j].acceleration =
                bodies[j].acceleration.add(
                    f.mult(1.0 / bodies[j].mass)
                );
        }
    }
}


}

void leapfrog_step(std::vector<Body>& bodies, double dt, bool is_first_step = false) {
    // if its the first step then it calculates the forces since there is nothing there to start with
if (is_first_step) {
    update_forces(bodies);
}

// implements the euler integration by kicks (applies the forces) then drifts (approximates the forces) then kicks (applies the forces) again 
for (int i = 0; i < bodies.size(); i++) {
    if (!bodies[i].active)
        continue;

    bodies[i].velocity =
        bodies[i].velocity.add(
            bodies[i].acceleration.mult(dt / 2.0)
        );
} 

for (int i = 0; i < bodies.size(); i++) {
    if (!bodies[i].active)
        continue;

    bodies[i].position =
        bodies[i].position.add(
            bodies[i].velocity.mult(dt)
        );
}

update_forces(bodies);

for (int i = 0; i < bodies.size(); i++) {
    if (!bodies[i].active)
        continue;

    bodies[i].velocity =
        bodies[i].velocity.add(
            bodies[i].acceleration.mult(dt / 2.0)
        );
}
}