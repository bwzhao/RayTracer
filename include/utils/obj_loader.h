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

#include <iostream>

ObjectList load_obj(std::string obj_file_path, double scale_x, double scale_y, double scale_z,
                    std::shared_ptr<Material> material = nullptr);