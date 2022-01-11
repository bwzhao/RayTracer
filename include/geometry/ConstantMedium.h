#pragma once

#include "utils/rt_utils.h"

#include "geometry/Object.h"
#include "Material.h"
#include "Texture.h"

class ConstantMedium : public Object {
public:
    ConstantMedium(shared_ptr<Object> b, double d, shared_ptr<Texture> a)
            : boundary_(b),
              neg_inv_density_(-1 / d),
              phase_function_(make_shared<Isotropic>(a))
    {}

    ConstantMedium(shared_ptr<Object> b, double d, Color c)
            : boundary_(b),
              neg_inv_density_(-1 / d),
              phase_function_(make_shared<Isotropic>(c))
    {}

    virtual bool hit(
            const Ray& r, double t_min, double t_max, HitRecord& rec) const override;

    virtual bool bounding_box(double time0, double time1, AABB& output_box) const override {
        return boundary_->bounding_box(time0, time1, output_box);
    }

public:
    shared_ptr<Object> boundary_;
    shared_ptr<Material> phase_function_;
    double neg_inv_density_;
};


