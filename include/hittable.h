#pragma once

#include "rt_utils.h"
#include "aabb.h"

class material;

struct hit_record {
    point3 p_;
    vec3 normal_;
    std::shared_ptr<material> mat_ptr_;
    double t_;
    double u_;
    double v_;
    bool front_face_;

    inline void set_face_normal(const ray& r, const vec3& outward_normal) {
        front_face_ = dot(r.direction(), outward_normal) < 0;
        normal_ = front_face_ ? outward_normal :-outward_normal;
    }
};

class hittable {
public:
    virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const = 0;
    virtual bool bounding_box(double time0, double time1, aabb& output_box) const = 0;
    virtual double pdf_value(const point3& o, const vec3& v) const {
        return 0.0;
    }

    virtual vec3 random(const vec3& o) const {
        return vec3(1, 0, 0);
    }
};


class translate : public hittable {
public:
    translate(shared_ptr<hittable> p, const vec3& displacement)
            : ptr_(p), offset_(displacement) {}

    virtual bool hit(
            const ray& r, double t_min, double t_max, hit_record& rec) const override;

    virtual bool bounding_box(double time0, double time1, aabb& output_box) const override;

public:
    shared_ptr<hittable> ptr_;
    vec3 offset_;
};

class rotate_y : public hittable {
public:
    rotate_y(shared_ptr<hittable> p, double angle);

    virtual bool hit(
            const ray& r, double t_min, double t_max, hit_record& rec) const override;

    virtual bool bounding_box(double time0, double time1, aabb& output_box) const override {
        output_box = bbox_;
        return hasbox_;
    }

public:
    shared_ptr<hittable> ptr_;
    double sin_theta_;
    double cos_theta_;
    bool hasbox_;
    aabb bbox_;
};

class flip_face : public hittable {
public:
    flip_face(shared_ptr<hittable> p) : ptr(p) {}

    virtual bool hit(
            const ray& r, double t_min, double t_max, hit_record& rec) const override {

        if (!ptr->hit(r, t_min, t_max, rec))
            return false;

        rec.front_face_ = !rec.front_face_;
        return true;
    }

    virtual bool bounding_box(double time0, double time1, aabb& output_box) const override {
        return ptr->bounding_box(time0, time1, output_box);
    }

public:
    shared_ptr<hittable> ptr;
};
