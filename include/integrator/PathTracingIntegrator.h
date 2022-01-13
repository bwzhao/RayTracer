#pragma once
#include "Vec3.h"
#include "Ray.h"
#include "geometry/Object.h"
#include "geometry/ObjectList.h"
#include "Material.h"

class Scene;

class PathTracingIntegrator{
private:

public:
    PathTracingIntegrator() {}

    Color get_radiance(const Ray& r, const Scene & scene, int depth);
};
