#include "geometry/Sphere.h"

bool Sphere::hit(const Ray& r, double t_min, double t_max, HitRecord& rec) const {
    Vec3 oc = r.origin() - center_;
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
    Vec3 outward_normal = (rec.p_ - center_) / radius_;
    rec.set_face_normal(r, outward_normal);
    get_sphere_uv(outward_normal, rec.u_, rec.v_);
    rec.mat_ptr_ = mat_ptr_;
    rec.wi_ = r.direction();

    return true;
}

bool Sphere::bounding_box(double time0, double time1, AABB& output_box) const {
    output_box = AABB(
            center_ - Vec3(radius_, radius_, radius_),
            center_ + Vec3(radius_, radius_, radius_));
    return true;
}

double Sphere::pdf_value_from_point(const Point3& o, const Vec3& v) const {
    HitRecord rec;
    if (!this->hit(Ray(o, v), 0.001, infinity, rec))
        return 0;

    auto cos_theta_max = sqrt(1 - radius_*radius_/(center_-o).length_squared());
    auto solid_angle = 2*pi*(1-cos_theta_max);

    return  1 / solid_angle;
}

Vec3 Sphere::random_from_point(const Point3& o) const {
    Vec3 direction = center_ - o;
    auto distance_squared = direction.length_squared();
    Onb uvw;
    uvw.build_from_w(direction);
    return uvw.local(random_to_sphere(radius_, distance_squared));
}

void Sphere::random_pos(Point3 &pos, Vec3 &normal, double &pdf_pos, double & u, double & v) const {
    auto random_dir = random_unit_vector();
    pos = center_ + radius_ * random_dir;
    normal = random_dir;

    auto solid_angle = 4. * pi;

    get_sphere_uv(random_dir, u, v);
    pdf_pos = 1. / solid_angle;

}
