#pragma once

#include "geometry/ObjectList.h"
#include "Camera.h"
#include "Vec3.h"
#include <fstream>

class Scene {
private:
    int image_width_;
    int image_height_;
    double aspect_ratio_;
    int samples_per_pixel_;
    int max_depth_;

    Color background_;

    shared_ptr<ObjectList> world_ptr_;
    shared_ptr<ObjectList> lights_ptr_;

    Camera cam_;

    std::vector<Color> image_;
    std::vector<int> samples_;

public:
    Scene(int image_width, int image_height, double aspect_ratio, int samples_per_pixel, int max_depth);

    void set_camera(Point3 lookfrom, Point3 lookat, Vec3 vup, double vfov, double aspect_ratio, double aperture,
                    double focus_dist, double time0, double time1);

    void set_world(ObjectList &world) {world_ptr_ = std::make_shared<ObjectList>(world);}
    void set_lights(shared_ptr<ObjectList> lights_ptr) {lights_ptr_ = std::move(lights_ptr);}

    void render();
    Color path_integrator(const Ray& r, int depth);

    void set_pixel(int idx, Color pixel_color);
    void write_image(char *file_name);

};


