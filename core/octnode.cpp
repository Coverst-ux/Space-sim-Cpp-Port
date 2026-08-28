#include "octnode.h"
#include "gravity.h"


octnode::octnode(double cx_val, double cy_val, double cz_val, double size_val) {
    cx = cx_val;
    cy = cy_val;
    cz = cz_val;
    size = size_val;
}
octnode::~octnode() = default;

void octnode::insert(Body new_body) {
    const double MIN_SIZE = 1e-3;

    if (!body.has_value() && children[0] == nullptr) {
        // if there isnt a root node and no children
        body = new_body;
        return;
    }
    if (body.has_value() && children[0] == nullptr) {
        if (size < MIN_SIZE) {
            double total = body->mass + new_body.mass;
            body->position = body->position.mult(body->mass).add(new_body.position.mult(new_body.mass)).mult(1.0 / total);
            body->mass = total;
            return;
        
            
        }
        // if there is a root but no children
        Body old_body = body.value();
        body = std::nullopt;
        subdivide(); 

        int old_idx = get_octant(old_body.position);
        int idx = get_octant(new_body.position);

        children[old_idx]->insert(old_body);
        children[idx]->insert(new_body);

    }
    else {
        // if there is a root and children
        int idx = get_octant(new_body.position);
        children[idx]->insert(new_body);
    }
    
}
void octnode::subdivide() {
    double offset = size / 4.0;
    for (int i = 0; i < 8; i++){

        double x_off = (i & 1) ? offset : -offset;
        double y_off = (i & 2) ? offset : -offset;
        double z_off = (i & 4) ? offset : -offset;

        children[i] = std::make_unique<octnode>(cx + x_off, cy + y_off, cz + z_off, size/2.0);


    }
}

int octnode::get_octant(vector3d position){
    int idx = 0;
    if (position.x >= cx) {
        idx |= 1;
    }
        if (position.y >= cy) {
        idx |= 2;
    }
        if (position.z >= cz) {
        idx |= 4;
    }
    return idx;
}

vector3d octnode::calculate_forces(Body target_body, double theta){
    if (body.has_value() && children[0] == nullptr){
        if (body->position.x == target_body.position.x &&
            body->position.y == target_body.position.y &&
            body->position.z == target_body.position.z) {
                // same position — this is the body we're computing forces for, skip it
                return vector3d(0, 0, 0);
        }
        return gravitational_force_softened(
            target_body.mass,
            body->mass,
            target_body.position,
            body->position
        );
    }

    vector3d com = vector3d(center_of_mass_x, center_of_mass_y, center_of_mass_z);
    double d = target_body.position.sub(com).magnitude();

    if (d == 0){
        return vector3d(0,0,0);
    }

    if (size / d < theta) {
        return gravitational_force_softened(
            target_body.mass,
            total_mass,
            target_body.position,
            com
        );
    }

    vector3d init_force = vector3d(0, 0, 0);
    for (auto& child : children) {
        if (child != nullptr) {
            init_force = init_force.add(child->calculate_forces(target_body, theta));
        }
    }
    return init_force;
}

void octnode::update_mass(){
    if (body.has_value() && children[0] == nullptr){
        total_mass = body->mass;
        center_of_mass_x = body->position.x;
        center_of_mass_y = body->position.y;
        center_of_mass_z = body->position.z;
        return;
    }
    total_mass = 0.0;
    center_of_mass_x = 0.0;
    center_of_mass_y = 0.0;
    center_of_mass_z = 0.0;
    
    for (int i = 0; i < 8; i++){
        if (children[i] != nullptr){
            children[i]->update_mass();

            if (children[i]->total_mass > 0){
                total_mass += children[i]->total_mass;
                center_of_mass_x += children[i]->total_mass * children[i]->center_of_mass_x;
                center_of_mass_y += children[i]->total_mass * children[i]->center_of_mass_y;
                center_of_mass_z += children[i]->total_mass * children[i]->center_of_mass_z;
            }
        }
    }

    if (total_mass > 0){
        center_of_mass_x /= total_mass;
        center_of_mass_y /= total_mass;
        center_of_mass_z /= total_mass;
    }
}