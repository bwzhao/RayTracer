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

bool hittable_list::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {
    hit_record temp_rec;
    bool hit_anything = false;
    auto closest_so_far = t_max;

    for (const auto& object : objects_) {
        if (object->hit(r, t_min, closest_so_far, temp_rec)) {
            hit_anything = true;
            closest_so_far = temp_rec.t_;
            rec = temp_rec;
        }
    }

    return hit_anything;
}

bool hittable_list::bounding_box(double time0, double time1, aabb& output_box) const {
    if (objects_.empty()) return false;

    aabb temp_box;
    bool first_box = true;

    for (const auto& object : objects_) {
        if (!object->bounding_box(time0, time1, temp_box)) return false;
        output_box = first_box ? temp_box : surrounding_box(output_box, temp_box);
        first_box = false;
    }

    return true;
}

double hittable_list::pdf_value(const point3& o, const vec3& v) const {
    auto weight = 1.0/objects_.size();
    auto sum = 0.0;

    for (const auto& object : objects_)
        sum += weight * object->pdf_value(o, v);

    return sum;
}

vec3 hittable_list::random(const vec3& o) const {
    auto int_size = static_cast<int>(objects_.size());
    return objects_[random_int(0, int_size-1)]->random(o);
}