#include "utils/rt_utils.h"

#include "utils/io_utils.h"
#include "geometry/ObjectList.h"
#include "geometry/Sphere.h"
#include "Material.h"
#include "geometry/aarect.h"
#include "Scene.h"
#include "scene_example/connell_box.h"
#include "utils/obj_loader.h"
#include "integrator/PathTracingIntegrator.h"
#include "integrator/BDPTIntegrator.h"

int main(int argc, char **argv) {
    // Set up scene
    const auto aspect_ratio = 1.0 / 1.0;
    const int image_width = 512;
    const int image_height = static_cast<int>(image_width / aspect_ratio);
    const int samples_per_pixel = 500;
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

    std::shared_ptr<Integrator> path_tracing_integrator_ptr = std::make_shared<PathTracingIntegrator>();
    std::shared_ptr<Integrator> BDPT_integrator_ptr = std::make_shared<BDPTIntegrator>();

    // World
    auto world = cornell_box_reverse_light();
    shared_ptr<ObjectList> lights_ptr = make_shared<ObjectList>();
    lights_ptr->add(make_shared<XZRect>(213, 343, 227, 332, 500, shared_ptr<Material>()));

    auto light = make_shared<DiffuseLight>(Color(15, 15, 15));
    shared_ptr<Light> light_ptr = std::make_shared<AreaLight>(make_shared<XZRect>(213, 343, 227, 332, 500, shared_ptr<Material>()),  Color(15, 15, 15));

    scene.set_world(world);
    scene.set_focus_lights(lights_ptr);
    scene.set_light(light_ptr);

    // Render
//    scene.render(path_tracing_integrator_ptr);
    scene.render(BDPT_integrator_ptr);
    scene.write_image("CornellBoxReverseLight.ppm");


    return 0;
}
