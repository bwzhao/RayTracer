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

ObjectList corneel_frame();

ObjectList cornell_box();

ObjectList cornell_box_obj();