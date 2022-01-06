#pragma once

#include "rt_utils.h"

#include "hittable.h"
#include "material.h"
#include "texture.h"

//class constant_medium : public hittable {
//public:
//    constant_medium(shared_ptr<hittable> b, double d, shared_ptr<texture> a)
//            : boundary_(b),
//              neg_inv_density_(-1 / d),
//              phase_function_(make_shared<isotropic>(a))
//    {}
//
//    constant_medium(shared_ptr<hittable> b, double d, color c)
//            : boundary_(b),
//              neg_inv_density_(-1 / d),
//              phase_function_(make_shared<isotropic>(c))
//    {}
//
//    virtual bool hit(
//            const ray& r, double t_min, double t_max, hit_record& rec) const override;
//
//    virtual bool bounding_box(double time0, double time1, aabb& output_box) const override {
//        return boundary_->bounding_box(time0, time1, output_box);
//    }
//
//public:
//    shared_ptr<hittable> boundary_;
//    shared_ptr<material> phase_function_;
//    double neg_inv_density_;
//};


//bool constant_medium::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {
//    // Print occasional samples when debugging. To enable, set enableDebug true.
//    const bool enableDebug = false;
//    const bool debugging = enableDebug && random_double() < 0.00001;
//
//    hit_record rec1, rec2;
//
//    if (!boundary_->hit(r, -infinity, infinity, rec1))
//        return false;
//
//    if (!boundary_->hit(r, rec1.t_+0.0001, infinity, rec2))
//        return false;
//
//    if (debugging) std::cerr << "\nt_min=" << rec1.t_ << ", t_max=" << rec2.t_ << '\n';
//
//    if (rec1.t_ < t_min) rec1.t_ = t_min;
//    if (rec2.t_ > t_max) rec2.t_ = t_max;
//
//    if (rec1.t_ >= rec2.t_)
//        return false;
//
//    if (rec1.t_ < 0)
//        rec1.t_ = 0;
//
//    const auto ray_length = r.direction().length();
//    const auto distance_inside_boundary = (rec2.t_ - rec1.t_) * ray_length;
//    const auto hit_distance = neg_inv_density_ * log(random_double());
//
//    if (hit_distance > distance_inside_boundary)
//        return false;
//
//    rec.t_ = rec1.t_ + hit_distance / ray_length;
//    rec.p_ = r.at(rec.t_);
//
//    if (debugging) {
//        std::cerr << "hit_distance = " <<  hit_distance << '\n'
//                  << "rec.t = " <<  rec.t_ << '\n'
//                  << "rec.p_ = " <<  rec.p_ << '\n';
//    }
//
//    rec.normal_ = vec3(1,0,0);  // arbitrary
//    rec.front_face_ = true;     // also arbitrary
//    rec.mat_ptr_ = phase_function_;
//
//    return true;
//}