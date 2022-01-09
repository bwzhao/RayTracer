#include "utils/rt_utils.h"

#include "utils/io_utils.h"
#include "geometry/HittableList.h"
#include "geometry/Sphere.h"
#include "Camera.h"
#include "Material.h"
#include "geometry/MovingSphere.h"
#include "geometry/aarect.h"
#include "geometry/Box.h"
#include "geometry/constant_medium.h"
#include "Bvh.h"

#include <iostream>
const double SIGMA = 0.001;

color ray_color(
        const Ray& r,
        const color& background,
        const Hittable& world,
        shared_ptr<HittableList>& lights,
        int depth
) {
    HitRecord rec;

    // If we've exceeded the Ray bounce limit, no more light is gathered.
    if (depth <= 0)
        return color(0,0,0);

    // If the Ray hits nothing, return the background color.
    if (!world.hit(r, 0.001, infinity, rec))
        return background;

    ScatterRecord srec;
    color emitted = rec.mat_ptr_->emitted(r, rec, rec.u_, rec.v_, rec.p_);
    if (!rec.mat_ptr_->scatter(r, rec, srec))
        return emitted;

    if (srec.is_specular) {
        return srec.attenuation
               * ray_color(srec.specular_ray, background, world, lights, depth-1);
    }

    auto light_ptr = make_shared<HittablePdf>(lights, rec.p_);
    MixturePdf p(light_ptr, srec.pdf_ptr);

    Ray scattered = Ray(rec.p_, p.generate(), r.time());
    auto pdf_val = p.value(scattered.direction());

    return emitted
           + srec.attenuation * rec.mat_ptr_->scattering_pdf(r, rec, scattered)
             * ray_color(scattered, background, world, lights, depth-1) / pdf_val;
}

HittableList cornell_box() {
    HittableList objects;

    auto red   = make_shared<Lambertian>(color(.65, .05, .05));
    auto white = make_shared<Lambertian>(color(.73, .73, .73));
    auto green = make_shared<Lambertian>(color(.12, .45, .15));
    auto light = make_shared<DiffuseLight>(color(15, 15, 15));

    objects.add(make_shared<YZRect>(0, 555, 0, 555, 555, green));
    objects.add(make_shared<YZRect>(0, 555, 0, 555, 0, red));
    objects.add(make_shared<flip_face>(make_shared<XZRect>(213, 343, 227, 332, 554, light)));
    objects.add(make_shared<XZRect>(0, 555, 0, 555, 555, white));
    objects.add(make_shared<XZRect>(0, 555, 0, 555, 0, white));
    objects.add(make_shared<XYRect>(0, 555, 0, 555, 555, white));

    shared_ptr<Material> aluminum = make_shared<Metal>(color(0.8, 0.85, 0.88), 0.0);
    shared_ptr<Hittable> box1 = make_shared<Box>(point3(0, 0, 0), point3(165, 330, 165), aluminum);
    box1 = make_shared<rotate_y>(box1, 15);
    box1 = make_shared<translate>(box1, Vec3(265,0,295));
    objects.add(box1);

    auto glass = make_shared<Dielectric>(1.5);
    objects.add(make_shared<Sphere>(point3(190, 90, 190), 90 , glass));

    return objects;
}


int main() {
// Image
    const auto aspect_ratio = 1.0 / 1.0;
    const int image_width = 600;
    const int image_height = static_cast<int>(image_width / aspect_ratio);
    const int samples_per_pixel = 10;
    const int max_depth = 50;

    // World
    auto world = cornell_box();
    auto lights = make_shared<HittableList>();
    lights->add(make_shared<XZRect>(213, 343, 227, 332, 554, shared_ptr<Material>()));
    lights->add(make_shared<Sphere>(point3(190, 90, 190), 90, shared_ptr<Material>()));

    color background(0,0,0);

    // Camera

    point3 lookfrom(278, 278, -800);
    point3 lookat(278, 278, 0);
    Vec3 vup(0, 1, 0);
    auto dist_to_focus = 10.0;
    auto aperture = 0.0;
    auto vfov = 40.0;
    auto time0 = 0.0;
    auto time1 = 1.0;

    Camera cam(lookfrom, lookat, vup, vfov, aspect_ratio, aperture, dist_to_focus, time0, time1);

    // Render

    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    for (int j = image_height-1; j >= 0; --j) {
        std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
        for (int i = 0; i < image_width; ++i) {
            color pixel_color(0, 0, 0);
            for (int s = 0; s < samples_per_pixel; ++s) {
                auto u = (i + random_double()) / (image_width-1);
                auto v = (j + random_double()) / (image_height-1);
                Ray r = cam.get_ray(u, v);
                pixel_color += ray_color(r, background, world, lights, max_depth);
            }
            write_color(std::cout, pixel_color, samples_per_pixel);
        }
    }
    std::cerr << "\nDone.\n";

    return 0;
}
