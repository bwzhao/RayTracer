#pragma once

#include "rt_utils.h"
#include "vec3.h"
#include "ray.h"

class aabb {
public:
    aabb() {}
    aabb(const point3& a, const point3& b) { minimum_ = a; maximum_ = b;}

    point3 min() const {return minimum_; }
    point3 max() const {return maximum_; }

    bool hit(const ray& r, double t_min, double t_max) const;
    static aabb surrounding_box(aabb box0, aabb box1);

    point3 minimum_;
    point3 maximum_;
};


inline bool aabb::hit(const ray& r, double t_min, double t_max) const {
    for (int a = 0; a < 3; a++) {
        auto invD = 1.0f / r.direction()[a];
        auto t0 = (min()[a] - r.origin()[a]) * invD;
        auto t1 = (max()[a] - r.origin()[a]) * invD;
        if (invD < 0.0f)
            std::swap(t0, t1);
        t_min = t0 > t_min ? t0 : t_min;
        t_max = t1 < t_max ? t1 : t_max;
        if (t_max <= t_min)
            return false;
    }
    return true;
}

