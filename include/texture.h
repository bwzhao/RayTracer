#pragma once

#include "rt_utils.h"
#include "image_utils.h"

#include "perlin.h"

class texture {
public:
    virtual color value(double u, double v, const point3& p) const = 0;
};

class solid_color : public texture {
public:
    solid_color() {}
    solid_color(color c) : color_value_(c) {}

    solid_color(double red, double green, double blue)
            : solid_color(color(red,green,blue)) {}

    virtual color value(double u, double v, const vec3& p) const override {
        return color_value_;
    }

private:
    color color_value_;
};


class checker_texture : public texture {
public:
    checker_texture() {}

    checker_texture(shared_ptr<texture> _even, shared_ptr<texture> _odd)
            : even_(_even), odd_(_odd) {}

    checker_texture(color c1, color c2)
            : even_(make_shared<solid_color>(c1)) , odd_(make_shared<solid_color>(c2)) {}

    virtual color value(double u, double v, const point3& p) const override {
        auto sines = sin(10*p.x())*sin(10*p.y())*sin(10*p.z());
        if (sines < 0)
            return odd_->value(u, v, p);
        else
            return even_->value(u, v, p);
    }

public:
    shared_ptr<texture> odd_;
    shared_ptr<texture> even_;
};

class noise_texture : public texture {
public:
    noise_texture() {}
    noise_texture(double sc) : scale_(sc) {}

    virtual color value(double u, double v, const point3& p) const override {
//        return color(1,1,1) * 0.5 * (1.0 + noise_.noise(scale_ * p_));
        return color(1,1,1) * 0.5 * (1 + sin(scale_*p.z() + 10*noise_.turb(p)));

    }

public:
    perlin noise_;
    double scale_;
};

class image_texture : public texture {
public:
    const static int bytes_per_pixel = 3;

    image_texture()
            : data_(nullptr), width_(0), height_(0), bytes_per_scanline_(0) {}

    image_texture(const char* filename) {
        auto components_per_pixel = bytes_per_pixel;

        data_ = stbi_load(
                filename, &width_, &height_, &components_per_pixel, components_per_pixel);

        if (!data_) {
            std::cerr << "ERROR: Could not load texture image file '" << filename << "'.\n";
            width_ = height_ = 0;
        }

        bytes_per_scanline_ = bytes_per_pixel * width_;
    }

    ~image_texture() {
        delete data_;
    }

    virtual color value(double u, double v, const vec3& p) const override {
        // If we have no texture data_, then return solid cyan as a debugging aid.
        if (data_ == nullptr)
            return color(0,1,1);

        // Clamp input texture coordinates to [0,1] x [1,0]
        u = clamp(u, 0.0, 1.0);
        v = 1.0 - clamp(v, 0.0, 1.0);  // Flip V to image coordinates

        auto i = static_cast<int>(u * width_);
        auto j = static_cast<int>(v * height_);

        // Clamp integer mapping, since actual coordinates should be less than 1.0
        if (i >= width_) i = width_ - 1;
        if (j >= height_) j = height_ - 1;

        const auto color_scale = 1.0 / 255.0;
        auto pixel = data_ + j * bytes_per_scanline_ + i * bytes_per_pixel;

        return color(color_scale*pixel[0], color_scale*pixel[1], color_scale*pixel[2]);
    }

private:
    unsigned char *data_;
    int width_, height_;
    int bytes_per_scanline_;
};