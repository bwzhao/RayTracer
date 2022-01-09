#pragma once

#include "utils/rt_utils.h"

#include "aarect.h"
#include "geometry/Hittable.h"
#include "HittableList.h"


class Box : public Hittable  {
public:
    Box() {}
    Box(const point3& p0, const point3& p1, shared_ptr<Material> ptr);

    virtual bool hit(const Ray& r, double t_min, double t_max, HitRecord& rec) const override;

    virtual bool bounding_box(double time0, double time1, AABB& output_box) const override {
        output_box = AABB(box_min_, box_max_);
        return true;
    }

public:
    point3 box_min_;
    point3 box_max_;
    HittableList sides_;
};