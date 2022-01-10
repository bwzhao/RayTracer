#pragma once

#include "utils/rt_utils.h"

class Camera {
public:
    void set_camera(
            Point3 lookfrom,
            Point3 lookat,
            Vec3   vup,
            double vfov, // vertical field-of-view in degrees
            double aspect_ratio,
            double aperture,
            double focus_dist,
            double time0,
            double time1
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

        time0_ = time0;
        time1_ = time1;
    }

    Ray get_ray(double s, double t) const {
        Vec3 rd = lens_radius_ * random_in_unit_disk();
        Vec3 offset = u_ * rd.x() + v_ * rd.y();

        return Ray(
                origin_ + offset,
                lower_left_corner_ + s*horizontal_ + t*vertical_ - origin_ - offset,
                random_double(time0_, time1_)
        );
    }

private:
    Point3 origin_;
    Point3 lower_left_corner_;
    Vec3 horizontal_;
    Vec3 vertical_;
    Vec3 u_, v_, w_;
    double lens_radius_;
    double time0_, time1_;
};
