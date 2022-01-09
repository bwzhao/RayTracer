#pragma once

#include "Vec3.h"

class Ray {
public:
    point3 orig_;
    Vec3 dir_;
    double tm_;

public:
    Ray() {}
    Ray(const point3 & orig, const Vec3 & dir, double time = 0.0): orig_(orig), dir_(dir), tm_(time) {}

    point3 origin() const {return orig_;}
    Vec3 direction() const {return dir_;}
    double time() const {return tm_;}

    point3 at(double t) const {return orig_ + t * dir_;}
};