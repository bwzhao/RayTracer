#pragma once

#include "utils/rt_utils.h"

#include "geometry/Hittable.h"

class XYRect : public Hittable {
public:
    XYRect() {}

    XYRect(double _x0, double _x1, double _y0, double _y1, double _k,
           shared_ptr<Material> mat)
            : x0_(_x0), x1_(_x1), y0_(_y0), y1_(_y1), k_(_k), mp_(mat) {};

    virtual bool hit(const Ray& r, double t_min, double t_max, HitRecord& rec) const override;

    virtual bool bounding_box(double time0, double time1, AABB& output_box) const override {
        // The bounding box must have non-zero width in each dimension, so pad the Z
        // dimension a small amount.
        output_box = AABB(point3(x0_, y0_, k_ - 0.0001), point3(x1_, y1_, k_ + 0.0001));
        return true;
    }

public:
    shared_ptr<Material> mp_;
    double x0_, x1_, y0_, y1_, k_;
};

class XZRect : public Hittable {
public:
    XZRect() {}

    XZRect(double _x0, double _x1, double _z0, double _z1, double _k,
           shared_ptr<Material> mat)
            : x0_(_x0), x1_(_x1), z0_(_z0), z1_(_z1), k_(_k), mp_(mat) {};

    virtual bool hit(const Ray& r, double t_min, double t_max, HitRecord& rec) const override;

    virtual bool bounding_box(double time0, double time1, AABB& output_box) const override {
        // The bounding box must have non-zero width in each dimension, so pad the Y
        // dimension a small amount.
        output_box = AABB(point3(x0_, k_ - 0.0001, z0_), point3(x1_, k_ + 0.0001, z1_));
        return true;
    }

    virtual double pdf_value(const point3& origin, const Vec3& v) const override {
        HitRecord rec;
        if (!this->hit(Ray(origin, v), 0.001, infinity, rec))
            return 0;

        auto area = (x1_-x0_)*(z1_-z0_);
        auto distance_squared = rec.t_ * rec.t_ * v.length_squared();
        auto cosine = fabs(dot(v, rec.normal_) / v.length());

        return distance_squared / (cosine * area);
    }

    virtual Vec3 random(const point3& origin) const override {
        auto random_point = point3(random_double(x0_, x1_), k_, random_double(z0_, z1_));
        return random_point - origin;
    }

public:
    shared_ptr<Material> mp_;
    double x0_, x1_, z0_, z1_, k_;
};

class YZRect : public Hittable {
public:
    YZRect() {}

    YZRect(double _y0, double _y1, double _z0, double _z1, double _k,
           shared_ptr<Material> mat)
            : y0_(_y0), y1_(_y1), z0_(_z0), z1_(_z1), k_(_k), mp_(mat) {};

    virtual bool hit(const Ray& r, double t_min, double t_max, HitRecord& rec) const override;

    virtual bool bounding_box(double time0, double time1, AABB& output_box) const override {
        // The bounding box must have non-zero width in each dimension, so pad the X
        // dimension a small amount.
        output_box = AABB(point3(k_ - 0.0001, y0_, z0_), point3(k_ + 0.0001, y1_, z1_));
        return true;
    }

public:
    shared_ptr<Material> mp_;
    double y0_, y1_, z0_, z1_, k_;
};

