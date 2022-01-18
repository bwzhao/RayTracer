#pragma once

#include "utils/rt_utils.h"
#include "AABB.h"
#include "Record.h"

class Material;

class Object {
public:
    shared_ptr<Material> mat_ptr_ = nullptr;

public:
    Object(): mat_ptr_(nullptr){}
    Object(shared_ptr<Material> m): mat_ptr_(m){}
    virtual bool hit(const Ray& r, double t_min, double t_max, HitRecord& rec) const = 0;
    virtual bool bounding_box(double time0, double time1, AABB& output_box) const = 0;

    virtual double pdf_value_from_point(const Point3& o, const Vec3& v) const {
        return 0.0;
    }

    virtual Vec3 random_from_point(const Point3& o) const {
        return Vec3(1, 0, 0);
    }

    virtual void random_pos(Point3 & pos, Vec3 & normal, double & pdf_pos, double & u, double & v) const {};

};


class translate : public Object {
public:
    translate(shared_ptr<Object> p, const Vec3& displacement)
            : ptr_(p), offset_(displacement) {}

    virtual bool hit(
            const Ray& r, double t_min, double t_max, HitRecord& rec) const override;

    virtual bool bounding_box(double time0, double time1, AABB& output_box) const override;

public:
    shared_ptr<Object> ptr_;
    Vec3 offset_;
};

class rotate_y : public Object {
public:
    rotate_y(shared_ptr<Object> p, double angle);

    virtual bool hit(
            const Ray& r, double t_min, double t_max, HitRecord& rec) const override;

    virtual bool bounding_box(double time0, double time1, AABB& output_box) const override {
        output_box = bbox_;
        return hasbox_;
    }

public:
    shared_ptr<Object> ptr_;
    double sin_theta_;
    double cos_theta_;
    bool hasbox_;
    AABB bbox_;
};

class flip_face : public Object {
public:
    flip_face(shared_ptr<Object> p) : ptr(p) {}

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
    shared_ptr<Object> ptr;
};