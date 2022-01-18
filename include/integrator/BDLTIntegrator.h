#pragma once

#include "Vertex.h"
#include <vector>

int generate_camera_subpath(const Scene &scene, int max_depth, const Ray& r_initial,
                            std::vector<Vertex> &path) {
    if (max_depth == 0)
        return 0;
    path.push_back(Vertex::create_camera(r_initial, 1.));

    double pdf_pos = 1., pdf_dir = 1., beta = 1.;
    return RandomWalk(scene, r_initial, beta, pdf_pos, pdf_dir,
                      max_depth - 1, path) + 1;
}

int generate_light_subpath(const Scene &scene, int max_depth, const Ray& r_initial,
                            std::vector<Vertex> &path) {
    if (max_depth == 0)
        return 0;
    path.push_back(Vertex::create_light());

    double pdf_pos = 1., pdf_dir = 1., beta = 1.;
    return RandomWalk(scene, r_initial, beta, pdf_pos, pdf_dir,
                      max_depth - 1, path) + 1;
}
