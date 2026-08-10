#pragma once
#include "body.h"
#include <vector>

struct binarystate {
    int binary_id;
    double a, e, nu;
    double m1, m2;
    int idx1, idx2;
    bool merged = false;
    double initial_separation = 0.0;
    bool merging = false;
    vector3d pending_velocity;
    float pending_mass;
    float pending_radius;
    int settle_frames_elapsed = 0;
    vector3d settle_start_position;
};
    // holds all active binary pairs
    extern std::vector<binarystate> binaries;

    // derive a, e from current position/velocity
    binarystate compute_orbital_elements(const Body& body1, const Body& body2, int binary_id);

    // initialize nu from current position (needed once, at setup)
    double compute_initial_true_anomaly(const Body& body1, const Body& body2, const binarystate& state);

    // peters decay: shrinks a and e based on gravitational wave energy loss
    void decay_orbital_elements(binarystate& state, float m1, float m2, double dt, double speedup);

    // advances nu forward based on current orbital speed
    void advance_true_anomaly(binarystate& state, float m1, float m2, double dt);

    //  writes new position/velocity back onto both bodies
    void apply_orbital_state(Body& body1, Body& body2, const binarystate& state);

    // registers a new binary pair, computes initial a, e, nu once and returns the state to be stored
    binarystate create_binary_state(const Body& body1, const Body& body2, int binary_id, int idx1, int idx2, float m1, float m2);

    // updates every active binary pair per frame and writes results back into bodies
    void update_binaries(std::vector<Body>& bodies, double dt);