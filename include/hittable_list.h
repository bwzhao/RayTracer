#pragma once

#include "hittable.h"
#include "aabb.h"

#include <memory>
#include <vector>

using std::shared_ptr;
using std::make_shared;

class hittable_list : public hittable {
public:
    hittable_list() {}
    hittable_list(shared_ptr<hittable> object) { add(object); }

    void clear() { objects_.clear(); }
    void add(shared_ptr<hittable> object) { objects_.push_back(object); }

    virtual bool hit(
            const ray& r, double t_min, double t_max, hit_record& rec) const override;
    virtual bool bounding_box(
            double time0, double time1, aabb& output_box) const override;
    double pdf_value(const point3& o, const vec3& v) const;
    vec3 random(const vec3& o) const;

public:
    std::vector<shared_ptr<hittable>> objects_;
};