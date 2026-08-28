#include <optional>
#include <stdio.h>
#include <memory>
#include "body.h"
#include "vector3d.h"
#include <array>

class octnode {
public:
    ~octnode();
    double cx, cy, cz, total_mass, center_of_mass_x, center_of_mass_y, center_of_mass_z;
    double size;
    std::optional<Body> body = std::nullopt;
    std::array<std::unique_ptr<octnode>, 8> children; // automatically assigns nullptr to all the children
    octnode(double cx_val, double cy_val, double cz_val, double size_val);
    void insert(Body new_body);
    void subdivide();
    int get_octant(vector3d position);
    void update_mass();
    vector3d calculate_forces(Body target_body, double theta);
};