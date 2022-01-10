#pragma once

#include "geometry/Hittable.h"
#include "Vec3.h"
#include "Onb.h"
#include "Pdf.h"

class Sphere : public Hittable {
public:
    Sphere() {}
    Sphere(Point3 cen, double r, shared_ptr<Material> m)
            : center_(cen), radius_(r), mat_ptr_(m) {};

    virtual bool hit(
            const Ray& r, double t_min, double t_max, HitRecord& rec) const override;
    virtual bool bounding_box(double time0, double time1, AABB& output_box) const override;

    virtual double pdf_value(const Point3& o, const Vec3& v) const override;
    virtual Vec3 random(const Point3& o) const override;


public:
    Point3 center_;
    double radius_;
    shared_ptr<Material> mat_ptr_;

private:
    static void get_sphere_uv(const Point3& p, double& u, double& v) {
        // p_: a given point on the sphere of radius one, centered at the origin.
        // u: returned value [0,1] of angle around the Y axis_ from X=-1.
        // v: returned value [0,1] of angle from Y=-1 to Y=+1.
        //     <1 0 0> yields <0.50 0.50>       <-1  0  0> yields <0.00 0.50>
        //     <0 1 0> yields <0.50 1.00>       < 0 -1  0> yields <0.50 0.00>
        //     <0 0 1> yields <0.25 0.50>       < 0  0 -1> yields <0.75 0.50>

        auto theta = acos(-p.y());
        auto phi = atan2(-p.z(), p.x()) + pi;

        u = phi / (2*pi);
        v = theta / pi;
    }
};