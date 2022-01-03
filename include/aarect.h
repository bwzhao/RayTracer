#pragma once

#include "rt_utils.h"

#include "hittable.h"

class xy_rect : public hittable {
public:
    xy_rect() {}

    xy_rect(double _x0, double _x1, double _y0, double _y1, double _k,
            shared_ptr<material> mat)
            : x0_(_x0), x1_(_x1), y0_(_y0), y1_(_y1), k_(_k), mp_(mat) {};

    virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const override;

    virtual bool bounding_box(double time0, double time1, aabb& output_box) const override {
        // The bounding box must have non-zero width in each dimension, so pad the Z
        // dimension a small amount.
        output_box = aabb(point3(x0_, y0_, k_ - 0.0001), point3(x1_, y1_, k_ + 0.0001));
        return true;
    }

public:
    shared_ptr<material> mp_;
    double x0_, x1_, y0_, y1_, k_;
};

bool xy_rect::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {
    auto t = (k_-r.origin().z()) / r.direction().z();
    if (t < t_min || t > t_max)
        return false;
    auto x = r.origin().x() + t*r.direction().x();
    auto y = r.origin().y() + t*r.direction().y();
    if (x < x0_ || x > x1_ || y < y0_ || y > y1_)
        return false;
    rec.u_ = (x-x0_)/(x1_-x0_);
    rec.v_ = (y-y0_)/(y1_-y0_);
    rec.t_ = t;
    auto outward_normal = vec3(0, 0, 1);
    rec.set_face_normal(r, outward_normal);
    rec.mat_ptr_ = mp_;
    rec.p_ = r.at(t);
    return true;
}

class xz_rect : public hittable {
public:
    xz_rect() {}

    xz_rect(double _x0, double _x1, double _z0, double _z1, double _k,
            shared_ptr<material> mat)
            : x0_(_x0), x1_(_x1), z0_(_z0), z1_(_z1), k_(_k), mp_(mat) {};

    virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const override;

    virtual bool bounding_box(double time0, double time1, aabb& output_box) const override {
        // The bounding box must have non-zero width in each dimension, so pad the Y
        // dimension a small amount.
        output_box = aabb(point3(x0_, k_ - 0.0001, z0_), point3(x1_, k_ + 0.0001, z1_));
        return true;
    }

public:
    shared_ptr<material> mp_;
    double x0_, x1_, z0_, z1_, k_;
};

class yz_rect : public hittable {
public:
    yz_rect() {}

    yz_rect(double _y0, double _y1, double _z0, double _z1, double _k,
            shared_ptr<material> mat)
            : y0_(_y0), y1_(_y1), z0_(_z0), z1_(_z1), k_(_k), mp_(mat) {};

    virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const override;

    virtual bool bounding_box(double time0, double time1, aabb& output_box) const override {
        // The bounding box must have non-zero width in each dimension, so pad the X
        // dimension a small amount.
        output_box = aabb(point3(k_ - 0.0001, y0_, z0_), point3(k_ + 0.0001, y1_, z1_));
        return true;
    }

public:
    shared_ptr<material> mp_;
    double y0_, y1_, z0_, z1_, k_;
};

bool xz_rect::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {
    auto t = (k_-r.origin().y()) / r.direction().y();
    if (t < t_min || t > t_max)
        return false;
    auto x = r.origin().x() + t*r.direction().x();
    auto z = r.origin().z() + t*r.direction().z();
    if (x < x0_ || x > x1_ || z < z0_ || z > z1_)
        return false;
    rec.u_ = (x-x0_)/(x1_-x0_);
    rec.v_ = (z-z0_)/(z1_-z0_);
    rec.t_ = t;
    auto outward_normal = vec3(0, 1, 0);
    rec.set_face_normal(r, outward_normal);
    rec.mat_ptr_ = mp_;
    rec.p_ = r.at(t);
    return true;
}

bool yz_rect::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {
    auto t = (k_-r.origin().x()) / r.direction().x();
    if (t < t_min || t > t_max)
        return false;
    auto y = r.origin().y() + t*r.direction().y();
    auto z = r.origin().z() + t*r.direction().z();
    if (y < y0_ || y > y1_ || z < z0_ || z > z1_)
        return false;
    rec.u_ = (y-y0_)/(y1_-y0_);
    rec.v_ = (z-z0_)/(z1_-z0_);
    rec.t_ = t;
    auto outward_normal = vec3(1, 0, 0);
    rec.set_face_normal(r, outward_normal);
    rec.mat_ptr_ = mp_;
    rec.p_ = r.at(t);
    return true;
}