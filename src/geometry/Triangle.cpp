#include "geometry/Triangle.h"


Triangle::Triangle(Point3 v0, Point3 v1, Point3 v2, shared_ptr<Material> m,
                   Vec3 n0, Vec3 n1, Vec3 n2,
                   Vec3 t0, Vec3 t1, Vec3 t2) :
v0_(v0),
v1_(v1),
v2_(v2),
n0_(n0),
n1_(n1),
n2_(n2),
t0_(t0),
t1_(t1),
t2_(t2),
Object(m)
{
    e1_ = v1 - v0;
    e2_ = v2 - v0;
//    normal_ = (n0 + n1 + n2) / 3.;
}

bool Triangle::hit(const Ray& r, double t_min, double t_max, HitRecord& rec) const {
    if (dot(r.direction(), n0_) > 0 && dot(r.direction(), n1_) > 0 && dot(r.direction(), n2_) > 0)
        return false;

    double u, v, t_tmp = 0.;
    Vec3 pvec = cross(r.direction(), e2_);
    double det = dot(e1_, pvec);
    if (fabs(det) < EPSILON)
        return false;

    double det_inv = 1. / det;
    Vec3 tvec = r.origin() - v0_;
    u = dot(tvec, pvec) * det_inv;
    if (u < 0 || u > 1)
        return false;
    Vec3 qvec = cross(tvec, e1_);
    v = dot(r.direction(), qvec) * det_inv;
    if (v < 0 || u + v > 1)
        return false;
    t_tmp = dot(e2_, qvec) * det_inv;

    if (t_tmp < 0) {
        return false;
    }
    if (t_tmp < t_min || t_max < t_tmp)
        return false;

    auto tmp_uv = (1. - u - v) * t0_ + u * t1_ + v * t2_;
    auto tmp_normal = unit_vector((1. - u - v) * n0_ + u * n1_ + v * n2_);

    rec.t_ = t_tmp;
    rec.p_ = r.at(rec.t_);
    rec.set_face_normal(r, tmp_normal);
    rec.u_ = tmp_uv[0];
    rec.v_ = tmp_uv[1];
    rec.mat_ptr_ = mat_ptr_;
    rec.wi_ = r.direction();

    return true;
}

bool Triangle::bounding_box(double time0, double time1, AABB& output_box) const {
    Vec3 mininum, maximum = Vec3();
    for (int idx = 0; idx != 3; ++idx) {
        auto minmax = std::minmax({v0_[idx], v1_[idx], v2_[idx]});
        // Add padding
        if (minmax.second - minmax.first < 0.0001){
            mininum[idx] = minmax.first;
            maximum[idx] = minmax.first + 0.0001;
        }
        else{
            mininum[idx] = minmax.first;
            maximum[idx] = minmax.second;
        }

    }

    output_box = AABB(mininum, maximum);
    return true;
}

double Triangle::pdf_value_from_point(const Point3& o, const Vec3& v) const {
    HitRecord rec;
    if (!this->hit(Ray(o, v), 0.001, infinity, rec))
        return 0;

    auto area = this->area();
    auto distance_squared = rec.t_ * rec.t_ * v.length_squared();
    auto cosine = fabs(dot(v, rec.normal_) / v.length());

    return distance_squared / (cosine * area);
}

Vec3 Triangle::random_from_point(const Point3& o) const {
    auto r1 = random_double(0., 1.);
    auto r2 = random_double(0., 1.);
    auto random_point = (1. - sqrt(r1)) * v0_ + (sqrt(r1) * (1. - r2)) * v1_ + r2 * sqrt(r1) * v2_;

    return random_point - o;
}

double Triangle::area() const {
    return 0.5 * cross(e1_, e2_).length();
}