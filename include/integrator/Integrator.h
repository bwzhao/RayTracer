#pragma once

class Scene;

class Integrator {
public:
    virtual Color render_pixel(const Scene & scene, double u, double v, int max_depth) = 0;
    virtual void render_pixel_test(Scene &scene, double u, double v, int max_depth, int idx) {};
};