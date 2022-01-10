#pragma once

#include "utils/rt_utils.h"

#include "geometry/Object.h"
#include "geometry/ObjectList.h"

#include <algorithm>

class BvhNode : public Object {
public:
    BvhNode();

    BvhNode(const ObjectList& list, double time0, double time1)
            : BvhNode(list.objects_, 0, list.objects_.size(), time0, time1)
    {}

    BvhNode(
            const std::vector<shared_ptr<Object>>& src_objects,
            size_t start, size_t end, double time0, double time1);

    virtual bool hit(
            const Ray& r, double t_min, double t_max, HitRecord& rec) const override;

    virtual bool bounding_box(double time0, double time1, AABB& output_box) const override;

public:
    shared_ptr<Object> left_;
    shared_ptr<Object> right_;
    AABB box_;
};

inline bool box_compare(const shared_ptr<Object> a, const shared_ptr<Object> b, int axis) {
    AABB box_a;
    AABB box_b;

    if (!a->bounding_box(0,0, box_a) || !b->bounding_box(0,0, box_b))
        std::cerr << "No bounding box in BvhNode constructor.\n";

    return box_a.min().e_[axis] < box_b.min().e_[axis];
}

bool box_x_compare (const shared_ptr<Object> a, const shared_ptr<Object> b);

bool box_y_compare (const shared_ptr<Object> a, const shared_ptr<Object> b);

bool box_z_compare (const shared_ptr<Object> a, const shared_ptr<Object> b);

