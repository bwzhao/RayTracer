#pragma once

#include "utils/rt_utils.h"

#include "aarect.h"
#include "geometry/Hittable.h"
#include "HittableList.h"


class Box : public Hittable  {
public:
    Box() {}
    Box(const Point3& p0, const Point3& p1, shared_ptr<Material> ptr);

    virtual bool hit(const Ray& r, double t_min, double t_max, HitRecord& rec) const override;

    virtual bool bounding_box(double time0, double time1, AABB& output_box) const override {
        output_box = AABB(box_min_, box_max_);
        return true;
    }

public:
    Point3 box_min_;
    Point3 box_max_;
    HittableList sides_;
};