#include "body.h"
#include "vector3d.h"

Body::Body(std::string name_val, float mass_val, vector3d position_val, vector3d velocity_val, 
           double radius_val, StellarType type_val, int binary_id_val)
    : name(name_val), mass(mass_val), position(position_val), velocity(velocity_val), 
      radius(radius_val), stellar_type(type_val), binary_id(binary_id_val)
{
    acceleration = vector3d(0, 0, 0);
}