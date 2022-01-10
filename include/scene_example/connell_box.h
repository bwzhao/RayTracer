#pragma once

#include "utils/io_utils.h"
#include "geometry/ObjectList.h"
#include "geometry/Sphere.h"
#include "Camera.h"
#include "Material.h"
#include "geometry/MovingSphere.h"
#include "geometry/aarect.h"
#include "geometry/Box.h"
#include "geometry/constant_medium.h"
#include "Bvh.h"
#include "geometry/Triangle.h"

ObjectList cornell_box() {
    ObjectList objects;

    auto red   = make_shared<Lambertian>(Color(.65, .05, .05));
    auto white = make_shared<Lambertian>(Color(.73, .73, .73));
    auto green = make_shared<Lambertian>(Color(.12, .45, .15));
    auto light = make_shared<DiffuseLight>(Color(15, 15, 15));

    objects.add(make_shared<YZRect>(0, 555, 0, 555, 555, green));
    objects.add(make_shared<YZRect>(0, 555, 0, 555, 0, red));
    objects.add(make_shared<flip_face>(make_shared<XZRect>(213, 343, 227, 332, 554, light)));
    objects.add(make_shared<XZRect>(0, 555, 0, 555, 555, white));
    objects.add(make_shared<XZRect>(0, 555, 0, 555, 0, white));
    objects.add(make_shared<XYRect>(0, 555, 0, 555, 555, white));
    objects.add(make_shared<Triangle>(Vec3(0, 0, 0),
                                      Vec3(0, 100, 0),
                                      Vec3(0, 100, 100), light));

    shared_ptr<Material> aluminum = make_shared<Metal>(Color(0.8, 0.85, 0.88), 0.0);
    shared_ptr<Object> box1 = make_shared<Box>(Point3(0, 0, 0), Point3(165, 330, 165), aluminum);
    box1 = make_shared<rotate_y>(box1, 15);
    box1 = make_shared<translate>(box1, Vec3(265,0,295));
    objects.add(box1);

    auto glass = make_shared<Dielectric>(1.5);
    objects.add(make_shared<Sphere>(Point3(190, 90, 190), 90 , glass));

    return objects;
}