#pragma once
#include "Vec3.h"
#include "Ray.h"
#include "geometry/Object.h"
#include "geometry/ObjectList.h"
#include "Integrator.h"
#include "Material.h"
#include "Vertex.h"

class Scene;

class PathTracingIntegrator: public Integrator{
private:

public:
    PathTracingIntegrator(int max_depth): Integrator(max_depth){};
    Color Li(const Ray& r, const Scene & scene, int depth);

    virtual Color render_pixel(const Scene & scene, double u, double v, int max_depth) override;
};
