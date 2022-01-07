#include "sphere.h"

bool sphere::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {
    vec3 oc = r.origin() - center_;
    auto a = r.direction().length_squared();
    auto half_b = dot(oc, r.direction());
    auto c = oc.length_squared() - radius_*radius_;

    auto discriminant = half_b*half_b - a*c;
    if (discriminant < 0) return false;
    auto sqrtd = sqrt(discriminant);

    // Find the nearest root that lies in the acceptable range.
    auto root = (-half_b - sqrtd) / a;
    if (root < t_min || t_max < root) {
        root = (-half_b + sqrtd) / a;
        if (root < t_min || t_max < root)
            return false;
    }

    rec.t_ = root;
    rec.p_ = r.at(rec.t_);
    vec3 outward_normal = (rec.p_ - center_) / radius_;
    rec.set_face_normal(r, outward_normal);
    get_sphere_uv(outward_normal, rec.u_, rec.v_);
    rec.mat_ptr_ = mat_ptr_;

    return true;
}

bool sphere::bounding_box(double time0, double time1, aabb& output_box) const {
    output_box = aabb(
            center_ - vec3(radius_, radius_, radius_),
            center_ + vec3(radius_, radius_, radius_));
    return true;
}

double sphere::pdf_value(const point3& o, const vec3& v) const {
    hit_record rec;
    if (!this->hit(ray(o, v), 0.001, infinity, rec))
        return 0;

    auto cos_theta_max = sqrt(1 - radius_*radius_/(center_-o).length_squared());
    auto solid_angle = 2*pi*(1-cos_theta_max);

    return  1 / solid_angle;
}

vec3 sphere::random(const point3& o) const {
    vec3 direction = center_ - o;
    auto distance_squared = direction.length_squared();
    onb uvw;
    uvw.build_from_w(direction);
    return uvw.local(random_to_sphere(radius_, distance_squared));
}
