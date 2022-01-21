#pragma once
#include "Vec3.h"
#include "Ray.h"
#include "geometry/Object.h"
#include "geometry/ObjectList.h"
#include "Integrator.h"
#include "Material.h"

class Scene;

class PathTracingIntegrator: public Integrator{
private:

public:

    Color get_radiance(const Ray& r, const Scene & scene, int depth);

    virtual Color render_pixel(const Scene & scene, double u, double v, int max_depth) override;
};
