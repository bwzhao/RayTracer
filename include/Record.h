#pragma once

#include "Ray.h"

class Material;
class Pdf;
class Light;
class Camera;

struct HitRecord {
    Point3 p_;
    Vec3 normal_;
    std::shared_ptr<Material> mat_ptr_;
    double t_;
    double u_;
    double v_;
    bool front_face_;
    Vec3 wi_;

    inline void set_face_normal(const Ray& r, const Vec3& outward_normal) {
        front_face_ = dot(r.direction(), outward_normal) < 0;
        normal_ = front_face_ ? outward_normal :-outward_normal;
    }
};

struct ScatterRecord {
    Ray specular_ray_;
    bool is_specular_;
    Color attenuation_;
    shared_ptr<Pdf> pdf_ptr_;
};

struct EndpointRecord {
    union {
        const Camera *camera;
        const Light *light;
    };
    Point3 p_;
    Vec3 normal_;
    bool front_face_;
};