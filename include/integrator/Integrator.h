#pragma once

class Scene;
#include "Vec3.h"
#include "Camera.h"
#include "Film.h"

class Integrator {
protected:
    Camera cam_;
    Film film_;
    int max_depth_;
public:
    Integrator(int max_depth): max_depth_(max_depth){};

    void set_film(int image_width, int image_height, double aspect_ratio, int samples_per_pixel);
    void set_camera(Point3 lookfrom, Point3 lookat, Vec3 vup, double vfov, double aspect_ratio, double aperture,
                    double focus_dist, double time0, double time1);

    void write_image(char *file_name);
    void write_image(char *file_name, int s, int t);

    virtual void render(const Scene &scene);
    virtual void render_test(const Scene &scene);

    virtual Color render_pixel(const Scene & scene, double u, double v, int max_depth) = 0;
    virtual void render_pixel_test(const Scene &scene, double u, double v, int max_depth, int idx) {};
};