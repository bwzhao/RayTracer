#pragma once

#include "utils.h"

class camera {
public:
    camera(
            point3 lookfrom,
            point3 lookat,
            vec3   vup,
            double vfov, // vertical field-of-view in degrees
            double aspect_ratio,
            double aperture,
            double focus_dist
    ) {
        auto theta = degrees_to_radians(vfov);
        auto h = tan(theta/2);
        auto viewport_height = 2.0 * h;
        auto viewport_width = aspect_ratio * viewport_height;

        w_ = unit_vector(lookfrom - lookat);
        u_ = unit_vector(cross(vup, w_));
        v_ = cross(w_, u_);

        origin_ = lookfrom;
        horizontal_ = focus_dist * viewport_width * u_;
        vertical_ = focus_dist * viewport_height * v_;
        lower_left_corner_ = origin_ - horizontal_/2 - vertical_/2 - focus_dist * w_;

        lens_radius_ = aperture / 2;
    }

    ray get_ray(double s, double t) const {
        vec3 rd = lens_radius_ * random_in_unit_disk();
        vec3 offset = u_ * rd.x() + v_ * rd.y();

        return ray(
                origin_ + offset,
                lower_left_corner_ + s*horizontal_ + t*vertical_ - origin_ - offset
        );
    }

private:
    point3 origin_;
    point3 lower_left_corner_;
    vec3 horizontal_;
    vec3 vertical_;
    vec3 u_, v_, w_;
    double lens_radius_;
};
