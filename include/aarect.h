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

    virtual double pdf_value(const point3& origin, const vec3& v) const override {
        hit_record rec;
        if (!this->hit(ray(origin, v), 0.001, infinity, rec))
            return 0;

        auto area = (x1_-x0_)*(z1_-z0_);
        auto distance_squared = rec.t_ * rec.t_ * v.length_squared();
        auto cosine = fabs(dot(v, rec.normal_) / v.length());

        return distance_squared / (cosine * area);
    }

    virtual vec3 random(const point3& origin) const override {
        auto random_point = point3(random_double(x0_, x1_), k_, random_double(z0_, z1_));
        return random_point - origin;
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

