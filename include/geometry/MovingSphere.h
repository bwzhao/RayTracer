#pragma once

#include "utils/rt_utils.h"

#include "geometry/Hittable.h"
#include "AABB.h"

class MovingSphere : public Hittable {
public:
    MovingSphere() {}
    MovingSphere(
            point3 cen0, point3 cen1, double _time0, double _time1, double r, shared_ptr<Material> m)
            : center0_(cen0), center1_(cen1), time0_(_time0), time1_(_time1), radius_(r), mat_ptr_(m)
    {};

    virtual bool hit(
            const Ray& r, double t_min, double t_max, HitRecord& rec) const override;
    virtual bool bounding_box(
            double _time0, double _time1, AABB& output_box) const override;

    point3 center(double time) const;

public:
    point3 center0_, center1_;
    double time0_, time1_;
    double radius_;
    shared_ptr<Material> mat_ptr_;
};

point3 MovingSphere::center(double time) const {
    return center0_ + ((time - time0_) / (time1_ - time0_)) * (center1_ - center0_);
}

bool MovingSphere::hit(const Ray& r, double t_min, double t_max, HitRecord& rec) const {
    Vec3 oc = r.origin() - center(r.time());
    auto a = r.direction().length_squared();
    auto half_b = dot(oc, r.direction());
    auto c = oc.length_squared() - radius_*radius_;

    auto discriminant = half_b*half_b - a*c;
    if (discriminant < 0) return false;
    auto sqrtd = sqrt(discriminant);

    // Find the nearest root that lies in the acceptable range.
    auto root = (-half_b - sqrtd) / a;
    if (root < t_min || t_max < root) {
        root = (-half_b + sqrtd) / a;
        if (root < t_min || t_max < root)
            return false;
    }

    rec.t_ = root;
    rec.p_ = r.at(rec.t_);
    auto outward_normal = (rec.p_ - center(r.time())) / radius_;
    rec.set_face_normal(r, outward_normal);
    rec.mat_ptr_ = mat_ptr_;

    return true;
}

bool MovingSphere::bounding_box(double _time0, double _time1, AABB& output_box) const {
    AABB box0(
            center(_time0) - Vec3(radius_, radius_, radius_),
            center(_time0) + Vec3(radius_, radius_, radius_));
    AABB box1(
            center(_time1) - Vec3(radius_, radius_, radius_),
            center(_time1) + Vec3(radius_, radius_, radius_));
    output_box = AABB::surrounding_box(box0, box1);
    return true;
}