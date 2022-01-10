#pragma once

#include "utils/rt_utils.h"
#include "Vec3.h"
#include "Ray.h"

class AABB {
public:
    AABB() {}
    AABB(const Point3& a, const Point3& b) { minimum_ = a; maximum_ = b;}

    Point3 min() const {return minimum_; }
    Point3 max() const {return maximum_; }

    bool hit(const Ray& r, double t_min, double t_max) const;
    static AABB surrounding_box(AABB box0, AABB box1);

    Point3 minimum_;
    Point3 maximum_;
};


inline bool AABB::hit(const Ray& r, double t_min, double t_max) const {
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

