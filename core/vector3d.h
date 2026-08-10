#pragma once

class vector3d {
public:
    float x, y, z;
    vector3d(float x_val, float y_val, float z_val);
    vector3d add(vector3d other) const;
    vector3d sub(vector3d other) const;
    vector3d mult(float scalar) const;
    float magnitude() const;
    vector3d normalized() const;
    vector3d() {x=0; y=0; z=0;};
    float dot(vector3d other) const;
    vector3d cross(const vector3d& other) const;
    vector3d operator+(const vector3d& other) const { return add(other); }
    vector3d operator-(const vector3d& other) const { return sub(other); }
};

vector3d lerp(vector3d a, vector3d b, float t);