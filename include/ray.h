#pragma once

#include "vec3.h"

class ray {
public:
    point3 orig_;
    vec3 dir_;

public:
    ray() {}
    ray(const point3 & orig, const vec3 & dir): orig_(orig), dir_(dir) {}

    point3 origin() const {return orig_;};
    vec3 direction() const {return dir_;};

    point3 at(double t) const {return orig_ + t * dir_;};
};