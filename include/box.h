#pragma once

#include "rt_utils.h"

#include "aarect.h"
#include "hittable.h"


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

box::box(const point3& p0, const point3& p1, shared_ptr<material> ptr) {
    box_min_ = p0;
    box_max_ = p1;

    sides_.add(make_shared<xy_rect>(p0.x(), p1.x(), p0.y(), p1.y(), p1.z(), ptr));
    sides_.add(make_shared<xy_rect>(p0.x(), p1.x(), p0.y(), p1.y(), p0.z(), ptr));

    sides_.add(make_shared<xz_rect>(p0.x(), p1.x(), p0.z(), p1.z(), p1.y(), ptr));
    sides_.add(make_shared<xz_rect>(p0.x(), p1.x(), p0.z(), p1.z(), p0.y(), ptr));

    sides_.add(make_shared<yz_rect>(p0.y(), p1.y(), p0.z(), p1.z(), p1.x(), ptr));
    sides_.add(make_shared<yz_rect>(p0.y(), p1.y(), p0.z(), p1.z(), p0.x(), ptr));
}

bool box::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {
    return sides_.hit(r, t_min, t_max, rec);
}