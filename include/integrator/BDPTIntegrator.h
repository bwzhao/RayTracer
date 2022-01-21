#pragma once

#include <vector>
#include "Vec3.h"
#include "Scene.h"
#include "Vertex.h"
#include "integrator/Integrator.h"

class Scene;
class Vertex;
class Ray;


enum class TransportMode { Radiance, Importance };

class BDPTIntegrator: public Integrator {
public:
    int generate_camera_subpath(const Scene &scene, int max_depth, double u, double v, std::vector<Vertex> &path_camera);
    int generate_light_subpath(const Scene &scene, int max_depth, std::vector<Vertex> &path_light);

    int random_walk(const Scene &scene, Ray ray, Color cur_beta, double pdf, int maxDepth, TransportMode mode, std::vector<Vertex> & path);
    Color connect_BDPT(const Scene & scene, std::vector<Vertex> &lightVertices,
                       std::vector<Vertex> &cameraVertices, int s, int t);
    Color G(const Scene &scene, const Vertex &v0, const Vertex &v1);

    virtual Color render_pixel(const Scene & scene, double u, double v, int max_depth) override;
};


