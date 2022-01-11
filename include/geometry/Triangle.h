#pragma once

#include "Object.h"
#include "Vec3.h"
#include "utils/rt_utils.h"

#include <algorithm>

class Triangle: public Object {
public:
    Point3 v0_, v1_, v2_; // vertices 0, 1, 2 , counter-clockwise order
    Vec3 n0_, n1_, n2_; // texture coords
    Vec3 t0_, t1_, t2_; // texture coords
    Vec3 e1_, e2_; // Two eges: 01, and 02
//    Vec3 normal_;
    shared_ptr<Material> mat_ptr_;

public:
    Triangle(Point3 v0, Point3 v1, Point3 v2, shared_ptr<Material> m,
             Vec3 n0, Vec3 n1, Vec3 n2,
             Vec3 t0=Vec3(0, 0, 0),
             Vec3 t1=Vec3(1, 0, 0),
             Vec3 t2=Vec3(1, 1, 0));
    virtual bool hit(
            const Ray& r, double t_min, double t_max, HitRecord& rec) const override;
    virtual bool bounding_box(double time0, double time1, AABB& output_box) const override;

    virtual double pdf_value(const Point3& o, const Vec3& v) const override;
    virtual Vec3 random(const Point3& o) const override;

    double area() const;
};


