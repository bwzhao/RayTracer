#include "geometry/Hittable.h"

bool translate::hit(const Ray& r, double t_min, double t_max, HitRecord& rec) const {
    Ray moved_r(r.origin() - offset_, r.direction(), r.time());
    if (!ptr_->hit(moved_r, t_min, t_max, rec))
        return false;

    rec.p_ += offset_;
    rec.set_face_normal(moved_r, rec.normal_);

    return true;
}

bool translate::bounding_box(double time0, double time1, AABB& output_box) const {
    if (!ptr_->bounding_box(time0, time1, output_box))
        return false;

    output_box = AABB(
            output_box.min() + offset_,
            output_box.max() + offset_);

    return true;
}

rotate_y::rotate_y(shared_ptr<Hittable> p, double angle) : ptr_(p) {
    auto radians = degrees_to_radians(angle);
    sin_theta_ = sin(radians);
    cos_theta_ = cos(radians);
    hasbox_ = ptr_->bounding_box(0, 1, bbox_);

    Point3 min(infinity, infinity, infinity);
    Point3 max(-infinity, -infinity, -infinity);

    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            for (int k = 0; k < 2; k++) {
                auto x = i*bbox_.max().x() + (1-i)*bbox_.min().x();
                auto y = j*bbox_.max().y() + (1-j)*bbox_.min().y();
                auto z = k*bbox_.max().z() + (1-k)*bbox_.min().z();

                auto newx =  cos_theta_*x + sin_theta_*z;
                auto newz = -sin_theta_*x + cos_theta_*z;

                Vec3 tester(newx, y, newz);

                for (int c = 0; c < 3; c++) {
                    min[c] = fmin(min[c], tester[c]);
                    max[c] = fmax(max[c], tester[c]);
                }
            }
        }
    }

    bbox_ = AABB(min, max);
}

bool rotate_y::hit(const Ray& r, double t_min, double t_max, HitRecord& rec) const {
    auto origin = r.origin();
    auto direction = r.direction();

    origin[0] = cos_theta_*r.origin()[0] - sin_theta_*r.origin()[2];
    origin[2] = sin_theta_*r.origin()[0] + cos_theta_*r.origin()[2];

    direction[0] = cos_theta_*r.direction()[0] - sin_theta_*r.direction()[2];
    direction[2] = sin_theta_*r.direction()[0] + cos_theta_*r.direction()[2];

    Ray rotated_r(origin, direction, r.time());

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
