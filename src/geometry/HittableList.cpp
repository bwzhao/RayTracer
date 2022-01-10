#include "geometry/HittableList.h"

bool HittableList::hit(const Ray& r, double t_min, double t_max, HitRecord& rec) const {
    HitRecord temp_rec;
    bool hit_anything = false;
    auto closest_so_far = t_max;

    for (const auto& object : objects_) {
        if (object->hit(r, t_min, closest_so_far, temp_rec)) {
            hit_anything = true;
            closest_so_far = temp_rec.t_;
            rec = temp_rec;
        }
    }

    return hit_anything;
}

bool HittableList::bounding_box(double time0, double time1, AABB& output_box) const {
    if (objects_.empty()) return false;

    AABB temp_box;
    bool first_box = true;

    for (const auto& object : objects_) {
        if (!object->bounding_box(time0, time1, temp_box)) return false;
        output_box = first_box ? temp_box : AABB::surrounding_box(output_box, temp_box);
        first_box = false;
    }

    return true;
}

double HittableList::pdf_value(const Point3& o, const Vec3& v) const {
    auto weight = 1.0/objects_.size();
    auto sum = 0.0;

    for (const auto& object : objects_)
        sum += weight * object->pdf_value(o, v);

    return sum;
}

Vec3 HittableList::random(const Vec3& o) const {
    auto int_size = static_cast<int>(objects_.size());
    return objects_[random_int(0, int_size-1)]->random(o);
}
