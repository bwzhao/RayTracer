#pragma once

#include "utils.h"

class material;

struct hit_record {
    point3 p_;
    vec3 normal_;
    std::shared_ptr<material> mat_ptr_;
    double t_;
    bool front_face_;

    inline void set_face_normal(const ray& r, const vec3& outward_normal) {
        front_face_ = dot(r.direction(), outward_normal) < 0;
        normal_ = front_face_ ? outward_normal :-outward_normal;
    }
};

class hittable {
public:
    virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const = 0;
};
