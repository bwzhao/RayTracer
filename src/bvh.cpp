#include "bvh.h"

bool bvh_node::bounding_box(double time0, double time1, aabb& output_box) const {
    output_box = box_;
    return true;
}

bool bvh_node::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {
    if (!box_.hit(r, t_min, t_max))
        return false;

    bool hit_left = left_->hit(r, t_min, t_max, rec);
    bool hit_right = right_->hit(r, t_min, hit_left ? rec.t_ : t_max, rec);

    return hit_left || hit_right;
}

bvh_node::bvh_node(
        const std::vector<shared_ptr<hittable>>& src_objects,
        size_t start, size_t end, double time0, double time1
) {
    auto objects = src_objects; // Create a modifiable array of the source scene objects

    int axis = random_int(0,2);
    auto comparator = (axis == 0) ? box_x_compare
                                  : (axis == 1) ? box_y_compare
                                                : box_z_compare;

    size_t object_span = end - start;

    if (object_span == 1) {
        left_ = right_ = objects[start];
    } else if (object_span == 2) {
        if (comparator(objects[start], objects[start+1])) {
            left_ = objects[start];
            right_ = objects[start+1];
        } else {
            left_ = objects[start+1];
            right_ = objects[start];
        }
    } else {
        std::sort(objects.begin() + start, objects.begin() + end, comparator);

        auto mid = start + object_span/2;
        left_ = make_shared<bvh_node>(objects, start, mid, time0, time1);
        right_ = make_shared<bvh_node>(objects, mid, end, time0, time1);
    }

    aabb box_left, box_right;

    if (  !left_->bounding_box (time0, time1, box_left)
          || !right_->bounding_box(time0, time1, box_right)
            )
        std::cerr << "No bounding box in bvh_node constructor.\n";

    box_ = aabb::surrounding_box(box_left, box_right);
}

bool box_x_compare (const shared_ptr<hittable> a, const shared_ptr<hittable> b) {
    return box_compare(a, b, 0);
}

bool box_y_compare (const shared_ptr<hittable> a, const shared_ptr<hittable> b) {
    return box_compare(a, b, 1);
}

bool box_z_compare (const shared_ptr<hittable> a, const shared_ptr<hittable> b) {
    return box_compare(a, b, 2);
}