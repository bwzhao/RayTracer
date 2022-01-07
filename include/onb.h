#pragma once

#include "vec3.h"

class onb {
public:
    onb() {}

    inline vec3 operator[](int i) const { return axis_[i]; }

    vec3 u() const { return axis_[0]; }
    vec3 v() const { return axis_[1]; }
    vec3 w() const { return axis_[2]; }

    vec3 local(double a, double b, double c) const {
        return a*u() + b*v() + c*w();
    }

    vec3 local(const vec3& a) const {
        return a.x()*u() + a.y()*v() + a.z()*w();
    }

    void build_from_w(const vec3&);

public:
    vec3 axis_[3];
};
