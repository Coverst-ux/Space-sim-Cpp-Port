#pragma once

class vector3d {
public:
    double x, y, z;
    vector3d(double x_val, double y_val, double z_val);
    vector3d add(vector3d other) const;
    vector3d sub(vector3d other) const;
    vector3d mult(double scalar) const;
    double magnitude() const;
    vector3d normalized() const;
    vector3d() {x=0; y=0; z=0;};
    double dot(vector3d other) const;
    vector3d cross(const vector3d& other) const;
    vector3d operator+(const vector3d& other) const { return add(other); }
    vector3d operator-(const vector3d& other) const { return sub(other); }
};

vector3d lerp(vector3d a, vector3d b, double t);