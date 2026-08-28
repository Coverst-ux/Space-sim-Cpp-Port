#include "gravity.h"
#include <iostream>
#include "constants.h"
#include "vector3d.h"


vector3d gravitational_force_softened(double m1, double m2, vector3d pos1, vector3d pos2) {
    vector3d displacement = vector3d(pos2.x - pos1.x, pos2.y - pos1.y, pos2.z - pos1.z);
    double distance_sq = displacement.x*displacement.x + displacement.y*displacement.y + displacement.z*displacement.z + EPSILON*EPSILON;
    double magnitude = G * m1 * m2 / distance_sq;
    return displacement.normalized().mult(magnitude);
}

double compute_total_energy(const std::vector<Body>& bodies) {
    double kinetic = 0.0;
    double potential = 0.0;

    // Kinetic energy
    for (size_t i = 0; i < bodies.size(); i++) {
        if (!bodies[i].active)
            continue;

        double speed_sq = bodies[i].velocity.dot(bodies[i].velocity);
        kinetic += 0.5 * bodies[i].mass * speed_sq;
    }

    // Potential energy
    for (size_t i = 0; i < bodies.size(); i++) {
        if (!bodies[i].active)
            continue;

        for (size_t j = i + 1; j < bodies.size(); j++) {
            if (!bodies[j].active)
                continue;

            vector3d diff = bodies[j].position.sub(bodies[i].position);
            double distance = diff.magnitude();

            potential +=
                -G * bodies[i].mass * bodies[j].mass / distance;
        }
    }

    return kinetic + potential;
}