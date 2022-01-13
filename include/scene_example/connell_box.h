#pragma once

#include "utils/io_utils.h"
#include "geometry/ObjectList.h"
#include "geometry/Sphere.h"
#include "Camera.h"
#include "Material.h"
#include "geometry/MovingSphere.h"
#include "geometry/aarect.h"
#include "geometry/Box.h"
#include "geometry/ConstantMedium.h"
#include "Bvh.h"
#include "geometry/Triangle.h"

void corneel_frame(ObjectList& objects);

ObjectList cornell_box();

ObjectList cornell_box_reverse_light();

ObjectList cornell_box_obj();