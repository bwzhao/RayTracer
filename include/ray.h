#pragma once

#include "vec3.h"

class ray {
public:
    point3 orig_;
    vec3 dir_;
    double tm_;

public:
    ray() {}
    ray(const point3 & orig, const vec3 & dir, double time = 0.0): orig_(orig), dir_(dir), tm_(time) {}

    point3 origin() const {return orig_;}
    vec3 direction() const {return dir_;}
    double time() const {return tm_;}

    point3 at(double t) const {return orig_ + t * dir_;}
};