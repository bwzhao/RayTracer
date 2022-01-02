#pragma once

#include "utils.h"

#include "hittable.h"
#include "aabb.h"

class moving_sphere : public hittable {
public:
    moving_sphere() {}
    moving_sphere(
            point3 cen0, point3 cen1, double _time0, double _time1, double r, shared_ptr<material> m)
            : center0_(cen0), center1_(cen1), time0_(_time0), time1_(_time1), radius_(r), mat_ptr_(m)
    {};

    virtual bool hit(
            const ray& r, double t_min, double t_max, hit_record& rec) const override;
    virtual bool bounding_box(
            double _time0, double _time1, aabb& output_box) const override;

    point3 center(double time) const;

public:
    point3 center0_, center1_;
    double time0_, time1_;
    double radius_;
    shared_ptr<material> mat_ptr_;
};

point3 moving_sphere::center(double time) const {
    return center0_ + ((time - time0_) / (time1_ - time0_)) * (center1_ - center0_);
}


bool moving_sphere::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {
    vec3 oc = r.origin() - center(r.time());
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

bool moving_sphere::bounding_box(double _time0, double _time1, aabb& output_box) const {
    aabb box0(
            center(_time0) - vec3(radius_, radius_, radius_),
            center(_time0) + vec3(radius_, radius_, radius_));
    aabb box1(
            center(_time1) - vec3(radius_, radius_, radius_),
            center(_time1) + vec3(radius_, radius_, radius_));
    output_box = surrounding_box(box0, box1);
    return true;
}