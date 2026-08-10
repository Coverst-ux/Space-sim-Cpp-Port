#include "vector3d.h"
#include <iostream>
#include "body.h"
#include "gravity.h"
#include "integrator.h"
#include "json_loader.h"
#include <chrono>
// TESTING PURPOSES
// USE cd "Your_File_Path"; g++ -O3 -fopenmp *.cpp -o main; if ($?) { .\main }

int main(){

    float dt = 3600.0f;

    // Generate 750 bodies for performance stress-testing (not physically meaningful)
    // std::vector<body> bodies;
    // for (int i = 0; i < 750; i++) {
    //     float mass = 1e24f;
    //     vector3d pos(i * 1e9f, (i % 100) * 1e9f, 0);
    //     vector3d vel(0, 1000.0f, 0);
    //     bodies.push_back(body("Body" + std::to_string(i), mass, pos, vel, 1e6));
    // }

    std::vector<Body> bodies;
    bodies.push_back(Body("Sun", 1.989e30f, vector3d(0, 0, 0), vector3d(0, 0, 0), 6.957e8));
    bodies.push_back(Body("Earth", 5.972e24f, vector3d(1.496e11f, 0, 0), vector3d(0, 29780, 0), 6.371e6));
    bodies.push_back(Body("Mercury", 3.301e23f, vector3d(4.6e10f, 0, 0), vector3d(0, 58980, 0), 2.439e6));

    double E_initial = compute_total_energy(bodies);

    for (int step = 0; step < 10000; step++) {
        leapfrog_step(bodies, dt, step == 0);

        if (step % 1000 == 0) {
            double E_current = compute_total_energy(bodies);
            double drift = (E_current - E_initial) / E_initial * 100.0;
            std::cout << "Step " << step << " | E = " << E_current
                       << " | drift = " << drift << "%" << std::endl;
        }
    }


    // for (int i = 0; i < 5; i++) {
    //     std::cout << bodies[i].name << " pos: ("
    //               << bodies[i].position.x << ", "
    //               << bodies[i].position.y << ", "
    //               << bodies[i].position.z << ")" << std::endl;
    // }



    // In gravity.h / gravity.cpp — assumes G is already defined there

    return 0;
}