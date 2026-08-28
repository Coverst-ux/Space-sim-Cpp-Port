#pragma once
#include <string>
#include "vector3d.h"

enum class StellarType {
    PROTOSTAR,
    RED_GIANT,
    WHITE_DWARF,
    NEUTRON_STAR,
    BLACK_HOLE,
    SUPERNOVA,
    NONE
};

class Body{
    public:
        Body(std::string name_val, double mass_val, vector3d position_val, vector3d velocity_val, 
             double radius_val, StellarType type_val = StellarType::NONE, int binary_id_val = -1);
        std::string name;
        double mass;
        vector3d position;
        vector3d velocity;
        double radius;
        vector3d acceleration = vector3d(0,0,0);
        StellarType stellar_type;
        int binary_id;
        bool active = true;
        bool just_merged = false;
};