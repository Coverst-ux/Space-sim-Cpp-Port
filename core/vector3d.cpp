#include <iostream>
#include <cmath>
#include "vector3d.h"
    
    vector3d::vector3d(float x_val, float y_val, float z_val) {
        x = x_val;
        y = y_val;
        z = z_val;  
    }

vector3d vector3d::add(vector3d other) const {
    vector3d result = vector3d(x + other.x, y + other.y, z + other.z);
    return result;
}

vector3d vector3d::sub(vector3d other) const {
    vector3d result = vector3d(x - other.x, y - other.y, z - other.z);
    return result;
}

vector3d vector3d::mult(float scalar) const {
    vector3d result = vector3d(x * scalar, y * scalar, z * scalar);
    return result;
}

float vector3d::magnitude() const {
    float result = std::sqrt(x*x + y*y + z*z);
    return result;
    
}

vector3d vector3d::normalized() const {
    float mag = magnitude();
    if (mag == 0) {
        throw std::invalid_argument("Can't normalize a zero vector.");
    }
    vector3d result = vector3d(x / mag, y / mag, z / mag);
    return result;
}

float vector3d::dot(vector3d other) const {
    float result = x*other.x + y*other.y + z*other.z;
    return result;
}

vector3d vector3d::cross(const vector3d& other) const {
    return vector3d(
        y * other.z - z * other.y,
        z * other.x - x * other.z,
        x * other.y - y * other.x
    );
}

vector3d lerp(vector3d a, vector3d b, float t) {
    return a.add(b.sub(a).mult(t));
}