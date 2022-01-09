#pragma once

#include "Vec3.h"

class Onb {
public:
    Onb() {}

    inline Vec3 operator[](int i) const { return axis_[i]; }

    Vec3 u() const { return axis_[0]; }
    Vec3 v() const { return axis_[1]; }
    Vec3 w() const { return axis_[2]; }

    Vec3 local(double a, double b, double c) const {
        return a*u() + b*v() + c*w();
    }

    Vec3 local(const Vec3& a) const {
        return a.x()*u() + a.y()*v() + a.z()*w();
    }

    void build_from_w(const Vec3&);

public:
    Vec3 axis_[3];
};
