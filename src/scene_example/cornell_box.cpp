#include "scene_example/connell_box.h"
#include "Texture.h"
#include "utils/obj_loader.h"

void corneel_frame(ObjectList& objects) {

    auto red   = make_shared<Lambertian>(Color(.65, .05, .05));
    auto white = make_shared<Lambertian>(Color(.73, .73, .73));
    auto green = make_shared<Lambertian>(Color(.12, .45, .15));
    auto texture = make_shared<ImageTexture>("../textures/viking_room.png");
    auto matereial = make_shared<Lambertian>(texture);

//    objects.add(make_shared<YZRect>(0, 555, 0, 555, 555, green));
    objects.add(make_shared<Triangle>(Vec3(555, 0, 0),
                                      Vec3(555, 555, 0),
                                      Vec3(555, 555, 555), green,
                                      Vec3(-1, 0, 0),
                                      Vec3(-1, 0, 0),
                                      Vec3(-1, 0, 0)
    ));
    objects.add(make_shared<Triangle>(Vec3(555, 0, 0),
                                      Vec3(555, 0, 555),
                                      Vec3(555, 555, 555), green,
                                      Vec3(-1, 0, 0),
                                      Vec3(-1, 0, 0),
                                      Vec3(-1, 0, 0)
    ));

//    objects.add(make_shared<YZRect>(0, 555, 0, 555, 0, red));
    objects.add(make_shared<Triangle>(Vec3(0, 0, 0),
                                      Vec3(0, 555, 0),
                                      Vec3(0, 555, 555), red,
                                      Vec3(1, 0, 0),
                                      Vec3(1, 0, 0),
                                      Vec3(1, 0, 0)
    ));
    objects.add(make_shared<Triangle>(Vec3(0, 0, 0),
                                      Vec3(0, 0, 555),
                                      Vec3(0, 555, 555), red,
                                      Vec3(1, 0, 0),
                                      Vec3(1, 0, 0),
                                      Vec3(1, 0, 0)
    ));

    objects.add(make_shared<XZRect>(0, 555, 0, 555, 555, white));
    objects.add(make_shared<XZRect>(0, 555, 0, 555, 0, white));
//    objects.add(make_shared<XYRect>(0, 555, 0, 555, 555, white));
    objects.add(make_shared<Triangle>(Vec3(0, 0, 555),
                                      Vec3(0, 555, 555),
                                      Vec3(555, 555, 555), matereial,
                                      Vec3(0, 0, -1),
                                      Vec3(0, 0, -1),
                                      Vec3(0, 0, -1),
                                      Vec3(0, 0, 0),
                                      Vec3(0, 1, 0),
                                      Vec3(1, 1, 0)
    ));
    objects.add(make_shared<Triangle>(Vec3(0, 0, 555),
                                      Vec3(555, 0, 555),
                                      Vec3(555, 555, 555), matereial,
                                      Vec3(0, 0, -1),
                                      Vec3(0, 0, -1),
                                      Vec3(0, 0, -1),
                                      Vec3(0, 0, 0),
                                      Vec3(1, 0, 0),
                                      Vec3(1, 1, 0)
    ));
}

ObjectList cornell_box() {
    ObjectList objects;

    auto light = make_shared<DiffuseLight>(Color(15, 15, 15));
    objects.add(make_shared<flip_face>(make_shared<XZRect>(213, 343, 227, 332, 554, light)));

    corneel_frame(objects);
    shared_ptr<Material> aluminum = make_shared<Metal>(Color(0.8, 0.85, 0.88), 0.0);
    shared_ptr<Object> box1 = make_shared<Box>(Point3(0, 0, 0), Point3(165, 330, 165), aluminum);
    box1 = make_shared<rotate_y>(box1, 15);
    box1 = make_shared<translate>(box1, Vec3(265,0,295));
    objects.add(box1);

    auto glass = make_shared<Dielectric>(1.5);
    objects.add(make_shared<Sphere>(Point3(190, 90, 190), 90 , glass));

    return objects;
}

ObjectList cornell_box_reverse_light() {
    ObjectList objects;

    auto light = make_shared<DiffuseLight>(Color(15, 15, 15));
    objects.add(make_shared<XZRect>(213, 343, 227, 332, 500, light));

    corneel_frame(objects);

    shared_ptr<Material> aluminum = make_shared<Metal>(Color(0.8, 0.85, 0.88), 0.0);
    shared_ptr<Object> box1 = make_shared<Box>(Point3(0, 0, 0), Point3(165, 330, 165), aluminum);
    box1 = make_shared<rotate_y>(box1, 15);
    box1 = make_shared<translate>(box1, Vec3(265,0,295));
    objects.add(box1);

    auto glass = make_shared<Dielectric>(1.5);
    objects.add(make_shared<Sphere>(Point3(190, 90, 190), 90 , glass));

    return objects;
}

ObjectList cornell_box_obj() {
    ObjectList objects;

    auto light = make_shared<DiffuseLight>(Color(15, 15, 15));
    objects.add(make_shared<flip_face>(make_shared<XZRect>(213, 343, 227, 332, 554, light)));

    corneel_frame(objects);

    shared_ptr<Material> aluminum = make_shared<Metal>(Color(0.8, 0.85, 0.88), 0.0);
    shared_ptr<Material> white = make_shared<Lambertian>(Color(.73, .73, .73));


//    ObjectList box_obj = load_obj("../models/dodecahedron.obj", 100, 100, 100, aluminum);
//    shared_ptr<Object> box1 = make_shared<ObjectList>(box_obj);
//    box1 = make_shared<rotate_y>(box1, 15);
//    box1 = make_shared<translate>(box1, Vec3(400,150,300));
//    objects.add(box1);


//    ObjectList box_obj = load_obj("../models/cube.obj", 165, 330, 165, aluminum);
//    shared_ptr<Object> box1 = make_shared<ObjectList>(box_obj);
//    box1 = make_shared<rotate_y>(box1, 15);
//    box1 = make_shared<translate>(box1, Vec3(265,0,295));
//    objects.add(box1);

    auto glass = make_shared<Dielectric>(1.5);
    objects.add(make_shared<Sphere>(Point3(190, 90, 190), 90 , glass));


    return objects;
}