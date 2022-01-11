#include "utils/rt_utils.h"

#include "utils/io_utils.h"
#include "geometry/ObjectList.h"
#include "geometry/Sphere.h"
#include "Material.h"
#include "geometry/aarect.h"
#include "Scene.h"
#include "scene_example/connell_box.h"
#include "utils/obj_loader.h"

int main(int argc, char **argv) {
    // Set up scene
    const auto aspect_ratio = 1.0 / 1.0;
    const int image_width = 500;
    const int image_height = static_cast<int>(image_width / aspect_ratio);
    const int samples_per_pixel = 100;
    const int max_depth = 50;

    Point3 lookfrom(278, 278, -800);
    Point3 lookat(278, 278, 0);
    Vec3 vup(0, 1, 0);
    auto dist_to_focus = 10.0;
    auto aperture = 0.0;
    auto vfov = 40.0;
    auto time0 = 0.0;
    auto time1 = 1.0;

    Scene scene(image_width, image_height, aspect_ratio, samples_per_pixel, max_depth);
    scene.set_camera(lookfrom, lookat, vup, vfov, aspect_ratio, aperture, dist_to_focus, time0, time1);

    // World
    auto world = cornell_box();
//    auto world = load_obj();
    shared_ptr<ObjectList> lights = make_shared<ObjectList>();
    lights->add(make_shared<XZRect>(213, 343, 227, 332, 554, shared_ptr<Material>()));
    lights->add(make_shared<Sphere>(Point3(190, 90, 190), 90, shared_ptr<Material>()));
//    lights->add(make_shared<flip_face>(make_shared<XZRect>(-1, 1, -1, 1, 2, shared_ptr<Material>())));

    scene.set_world(world);
    scene.set_lights(lights);

    // Render
    scene.render();
    scene.write_image(argv[1]);


    return 0;
}
