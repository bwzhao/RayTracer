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

bool translate::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {
    ray moved_r(r.origin() - offset_, r.direction(), r.time());
    if (!ptr_->hit(moved_r, t_min, t_max, rec))
        return false;

    rec.p_ += offset_;
    rec.set_face_normal(moved_r, rec.normal_);

    return true;
}

bool translate::bounding_box(double time0, double time1, aabb& output_box) const {
    if (!ptr_->bounding_box(time0, time1, output_box))
        return false;

    output_box = aabb(
            output_box.min() + offset_,
            output_box.max() + offset_);

    return true;
}

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

rotate_y::rotate_y(shared_ptr<hittable> p, double angle) : ptr_(p) {
    auto radians = degrees_to_radians(angle);
    sin_theta_ = sin(radians);
    cos_theta_ = cos(radians);
    hasbox_ = ptr_->bounding_box(0, 1, bbox_);

    point3 min( infinity,  infinity,  infinity);
    point3 max(-infinity, -infinity, -infinity);

    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            for (int k = 0; k < 2; k++) {
                auto x = i*bbox_.max().x() + (1-i)*bbox_.min().x();
                auto y = j*bbox_.max().y() + (1-j)*bbox_.min().y();
                auto z = k*bbox_.max().z() + (1-k)*bbox_.min().z();

                auto newx =  cos_theta_*x + sin_theta_*z;
                auto newz = -sin_theta_*x + cos_theta_*z;

                vec3 tester(newx, y, newz);

                for (int c = 0; c < 3; c++) {
                    min[c] = fmin(min[c], tester[c]);
                    max[c] = fmax(max[c], tester[c]);
                }
            }
        }
    }

    bbox_ = aabb(min, max);
}

bool rotate_y::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {
    auto origin = r.origin();
    auto direction = r.direction();

    origin[0] = cos_theta_*r.origin()[0] - sin_theta_*r.origin()[2];
    origin[2] = sin_theta_*r.origin()[0] + cos_theta_*r.origin()[2];

    direction[0] = cos_theta_*r.direction()[0] - sin_theta_*r.direction()[2];
    direction[2] = sin_theta_*r.direction()[0] + cos_theta_*r.direction()[2];

    ray rotated_r(origin, direction, r.time());

    if (!ptr_->hit(rotated_r, t_min, t_max, rec))
        return false;

    auto p = rec.p_;
    auto normal = rec.normal_;

    p[0] =  cos_theta_*rec.p_[0] + sin_theta_*rec.p_[2];
    p[2] = -sin_theta_*rec.p_[0] + cos_theta_*rec.p_[2];

    normal[0] =  cos_theta_*rec.normal_[0] + sin_theta_*rec.normal_[2];
    normal[2] = -sin_theta_*rec.normal_[0] + cos_theta_*rec.normal_[2];

    rec.p_ = p;
    rec.set_face_normal(rotated_r, normal);

    return true;
}

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
