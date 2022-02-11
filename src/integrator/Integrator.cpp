#include "integrator/Integrator.h"

void Integrator::set_camera(Point3 lookfrom, Point3 lookat, Vec3 vup, double vfov, double aspect_ratio, double aperture,
                       double focus_dist, double time0, double time1) {
    cam_.set_camera(lookfrom, lookat, vup, vfov, aspect_ratio, aperture, focus_dist, time0, time1);
}

void Integrator::set_film(int image_width, int image_height, double aspect_ratio, int samples_per_pixel) {
    film_.set_film(image_width, image_height, aspect_ratio, samples_per_pixel);
}

void Integrator::render(const Scene &scene) {
    for (int s = 0; s < film_.samples_per_pixel_; ++s) {
        std::cout << "\rSamples remaining: " << film_.samples_per_pixel_ - s - 1<< ' ' << std::flush;
        for (int j = film_.image_height_ - 1; j >= 0; --j) {
            for (int i = 0; i < film_.image_width_; ++i) {
                auto idx = i + (film_.image_height_ - 1 - j) * film_.image_width_;
                auto u = (i + random_double()) / (film_.image_width_ - 1);
                auto v = (j + random_double()) / (film_.image_height_ - 1);
                auto pixel_color = render_pixel(scene, u, v, max_depth_);
                film_.set_pixel(idx, pixel_color);
            }
        }
    }
    std::cout << "\nDone.\n";
}

void Integrator::render_test(const Scene &scene) {
    for (int s = 0; s < film_.samples_per_pixel_; ++s) {
        std::cout << "\rSamples remaining: " << film_.samples_per_pixel_ - s - 1<< ' ' << std::flush;
        for (int j = film_.image_height_ - 1; j >= 0; --j) {
            for (int i = 0; i < film_.image_width_; ++i) {
                auto idx = i + (film_.image_height_ - 1 - j) * film_.image_width_;
                auto u = (i + random_double()) / (film_.image_width_ - 1);
                auto v = (j + random_double()) / (film_.image_height_ - 1);
                render_pixel_test(scene, u, v, max_depth_, idx);
            }
        }
    }
    std::cout << "\nDone.\n";
}

void Integrator::write_image(char *file_name) {
    film_.write_image(file_name);
}

void Integrator::write_image(char *file_name, int s, int t) {
    film_.write_image(file_name, s, t);
}
