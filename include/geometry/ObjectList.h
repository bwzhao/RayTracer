#pragma once

#include "geometry/Object.h"
#include "AABB.h"

#include <memory>
#include <vector>

using std::shared_ptr;
using std::make_shared;

class ObjectList : public Object {
public:
    ObjectList() {}
    ObjectList(shared_ptr<Object> object) { add(object); }

    void clear() { objects_.clear(); }
    void add(shared_ptr<Object> object) { objects_.push_back(object); }

    virtual bool hit(
            const Ray& r, double t_min, double t_max, HitRecord& rec) const override;
    virtual bool bounding_box(
            double time0, double time1, AABB& output_box) const override;
    double pdf_value_from_point(const Point3& o, const Vec3& v) const override;
    Vec3 random_from_point(const Vec3& o) const override;

public:
    std::vector<shared_ptr<Object>> objects_;
};