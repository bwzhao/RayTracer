#pragma once

#include "utils/rt_utils.h"
#include "AABB.h"

class Material;

struct HitRecord {
    point3 p_;
    Vec3 normal_;
    std::shared_ptr<Material> mat_ptr_;
    double t_;
    double u_;
    double v_;
    bool front_face_;

    inline void set_face_normal(const Ray& r, const Vec3& outward_normal) {
        front_face_ = dot(r.direction(), outward_normal) < 0;
        normal_ = front_face_ ? outward_normal :-outward_normal;
    }
};

class Hittable {
public:
    virtual bool hit(const Ray& r, double t_min, double t_max, HitRecord& rec) const = 0;
    virtual bool bounding_box(double time0, double time1, AABB& output_box) const = 0;
    virtual double pdf_value(const point3& o, const Vec3& v) const {
        return 0.0;
    }

    virtual Vec3 random(const Vec3& o) const {
        return Vec3(1, 0, 0);
    }
};


class translate : public Hittable {
public:
    translate(shared_ptr<Hittable> p, const Vec3& displacement)
            : ptr_(p), offset_(displacement) {}

    virtual bool hit(
            const Ray& r, double t_min, double t_max, HitRecord& rec) const override;

    virtual bool bounding_box(double time0, double time1, AABB& output_box) const override;

public:
    shared_ptr<Hittable> ptr_;
    Vec3 offset_;
};

class rotate_y : public Hittable {
public:
    rotate_y(shared_ptr<Hittable> p, double angle);

    virtual bool hit(
            const Ray& r, double t_min, double t_max, HitRecord& rec) const override;

    virtual bool bounding_box(double time0, double time1, AABB& output_box) const override {
        output_box = bbox_;
        return hasbox_;
    }

public:
    shared_ptr<Hittable> ptr_;
    double sin_theta_;
    double cos_theta_;
    bool hasbox_;
    AABB bbox_;
};

class flip_face : public Hittable {
public:
    flip_face(shared_ptr<Hittable> p) : ptr(p) {}

    virtual bool hit(
            const Ray& r, double t_min, double t_max, HitRecord& rec) const override {

        if (!ptr->hit(r, t_min, t_max, rec))
            return false;

        rec.front_face_ = !rec.front_face_;
        return true;
    }

    virtual bool bounding_box(double time0, double time1, AABB& output_box) const override {
        return ptr->bounding_box(time0, time1, output_box);
    }

public:
    shared_ptr<Hittable> ptr;
};
