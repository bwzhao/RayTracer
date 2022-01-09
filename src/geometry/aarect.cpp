#include "geometry/aarect.h"

bool XYRect::hit(const Ray& r, double t_min, double t_max, HitRecord& rec) const {
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
    auto outward_normal = Vec3(0, 0, 1);
    rec.set_face_normal(r, outward_normal);
    rec.mat_ptr_ = mp_;
    rec.p_ = r.at(t);
    return true;
}

bool XZRect::hit(const Ray& r, double t_min, double t_max, HitRecord& rec) const {
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
    auto outward_normal = Vec3(0, 1, 0);
    rec.set_face_normal(r, outward_normal);
    rec.mat_ptr_ = mp_;
    rec.p_ = r.at(t);
    return true;
}

bool YZRect::hit(const Ray& r, double t_min, double t_max, HitRecord& rec) const {
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
    auto outward_normal = Vec3(1, 0, 0);
    rec.set_face_normal(r, outward_normal);
    rec.mat_ptr_ = mp_;
    rec.p_ = r.at(t);
    return true;
}