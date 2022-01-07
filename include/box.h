#pragma once

#include "rt_utils.h"

#include "aarect.h"
#include "hittable.h"
#include "hittable_list.h"


class box : public hittable  {
public:
    box() {}
    box(const point3& p0, const point3& p1, shared_ptr<material> ptr);

    virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const override;

    virtual bool bounding_box(double time0, double time1, aabb& output_box) const override {
        output_box = aabb(box_min_, box_max_);
        return true;
    }

public:
    point3 box_min_;
    point3 box_max_;
    hittable_list sides_;
};