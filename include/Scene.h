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

    ObjectList world_;
    shared_ptr<ObjectList> lights_;

    Camera cam_;

    std::vector<Color> image_;
    std::vector<int> samples_;

public:
    Scene(int image_width, int image_height, double aspect_ratio, int samples_per_pixel, int max_depth);

    void set_camera(Point3 lookfrom, Point3 lookat, Vec3 vup, double vfov, double aspect_ratio, double aperture,
                    double focus_dist, double time0, double time1);

    void set_world(ObjectList world) { world_ = std::move(world);}
    void set_lights(shared_ptr<ObjectList> lights) { lights_ = std::move(lights);}

    void render();
    static Color ray_color(const Ray& r, const Color& background, const Object& world,
                           shared_ptr<ObjectList>& lights, int depth
    );

    void set_pixel(int idx, Color pixel_color);
    void write_image(char *file_name);

};


