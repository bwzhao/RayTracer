#pragma once

#include <vector>
#include "Vec3.h"
#include <iostream>
#include <fstream>

class Film {
public:
    int image_width_;
    int image_height_;
    double aspect_ratio_;
    int samples_per_pixel_;

    std::vector<Color> image_;
    std::vector<int> samples_;

    std::vector<std::vector<std::vector<Color>>> image_debug;
    std::vector<std::vector<std::vector<int>>> samples_debug;

public:
    Film() = default;
    void set_film(int image_width, int image_height, double aspect_ratio, int samples_per_pixel);

    void set_pixel(int idx, Color pixel_color);
    void write_image(char *file_name);

    // Only for debug
    void set_pixel(int idx, Color pixel_color, int s, int t);
    void write_image(char *file_name, int s, int t);
};