#include "Vec3.h"

Vec3 reflect(const Vec3& v, const Vec3& n) {
    return v - 2*dot(v,n)*n;
}


Vec3 refract(const Vec3& uv, const Vec3& n, double etai_over_etat) {
    auto cos_theta = fmin(dot(-uv, n), 1.0);
    Vec3 r_out_perp =  etai_over_etat * (uv + cos_theta*n);
    Vec3 r_out_parallel = -sqrt(fabs(1.0 - r_out_perp.length_squared())) * n;
    return r_out_perp + r_out_parallel;
}

Vec3 random_in_unit_sphere() {
    while (true) {
        auto p = Vec3::random(-1,1);
        if (p.length_squared() >= 1) continue;
        return p;
    }
}

Vec3 random_unit_vector() {
    return unit_vector(random_in_unit_sphere());
}

Vec3 random_in_hemisphere(const Vec3& normal) {
    Vec3 in_unit_sphere = random_in_unit_sphere();
    if (dot(in_unit_sphere, normal) > 0.0) // In the same hemisphere as the ng
        return in_unit_sphere;
    else
        return -in_unit_sphere;
}

Vec3 random_in_unit_disk() {
    while (true) {
        auto p = Vec3(random_double(-1,1), random_double(-1,1), 0);
        if (p.length_squared() >= 1) continue;
        return p;
    }
}