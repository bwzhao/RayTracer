#pragma once

#include "geometry/Hittable.h"
#include "AABB.h"

#include <memory>
#include <vector>

using std::shared_ptr;
using std::make_shared;

class HittableList : public Hittable {
public:
    HittableList() {}
    HittableList(shared_ptr<Hittable> object) { add(object); }

    void clear() { objects_.clear(); }
    void add(shared_ptr<Hittable> object) { objects_.push_back(object); }

    virtual bool hit(
            const Ray& r, double t_min, double t_max, HitRecord& rec) const override;
    virtual bool bounding_box(
            double time0, double time1, AABB& output_box) const override;
    double pdf_value(const point3& o, const Vec3& v) const override;
    Vec3 random(const Vec3& o) const override;

public:
    std::vector<shared_ptr<Hittable>> objects_;
};