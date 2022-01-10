#pragma once

#include "utils/rt_utils.h"

#include "geometry/Object.h"
#include "AABB.h"

class MovingSphere : public Object {
public:
    MovingSphere() {}
    MovingSphere(
            Point3 cen0, Point3 cen1, double _time0, double _time1, double r, shared_ptr<Material> m)
            : center0_(cen0), center1_(cen1), time0_(_time0), time1_(_time1), radius_(r), mat_ptr_(m)
    {};

    virtual bool hit(
            const Ray& r, double t_min, double t_max, HitRecord& rec) const override;
    virtual bool bounding_box(
            double _time0, double _time1, AABB& output_box) const override;

    Point3 center(double time) const;

public:
    Point3 center0_, center1_;
    double time0_, time1_;
    double radius_;
    shared_ptr<Material> mat_ptr_;
};

