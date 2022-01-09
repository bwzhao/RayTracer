#pragma once

#include "utils/rt_utils.h"
#include "utils/image_utils.h"

#include "Perlin.h"

class Texture {
public:
    virtual color value(double u, double v, const point3& p) const = 0;
};

class SolidColor : public Texture {
public:
    SolidColor() {}
    SolidColor(color c) : color_value_(c) {}

    SolidColor(double red, double green, double blue)
            : SolidColor(color(red, green, blue)) {}

    virtual color value(double u, double v, const Vec3& p) const override {
        return color_value_;
    }

private:
    color color_value_;
};

class CheckerTexture : public Texture {
public:
    CheckerTexture() {}

    CheckerTexture(shared_ptr<Texture> _even, shared_ptr<Texture> _odd)
            : even_(_even), odd_(_odd) {}

    CheckerTexture(color c1, color c2)
            : even_(make_shared<SolidColor>(c1)) , odd_(make_shared<SolidColor>(c2)) {}

    virtual color value(double u, double v, const point3& p) const override {
        auto sines = sin(10*p.x())*sin(10*p.y())*sin(10*p.z());
        if (sines < 0)
            return odd_->value(u, v, p);
        else
            return even_->value(u, v, p);
    }

public:
    shared_ptr<Texture> odd_;
    shared_ptr<Texture> even_;
};

class NoiseTexture : public Texture {
public:
    NoiseTexture() {}
    NoiseTexture(double sc) : scale_(sc) {}

    virtual color value(double u, double v, const point3& p) const override {
//        return color(1,1,1) * 0.5 * (1.0 + noise_.noise(scale_ * p_));
        return color(1,1,1) * 0.5 * (1 + sin(scale_*p.z() + 10*noise_.turb(p)));

    }

public:
    Perlin noise_;
    double scale_;
};

class ImageTexture : public Texture {
public:
    const static int bytes_per_pixel = 3;

    ImageTexture()
            : data_(nullptr), width_(0), height_(0), bytes_per_scanline_(0) {}

    ImageTexture(const char* filename) {
        auto components_per_pixel = bytes_per_pixel;

        data_ = stbi_load(
                filename, &width_, &height_, &components_per_pixel, components_per_pixel);

        if (!data_) {
            std::cerr << "ERROR: Could not load Texture image file '" << filename << "'.\n";
            width_ = height_ = 0;
        }

        bytes_per_scanline_ = bytes_per_pixel * width_;
    }

    ~ImageTexture() {
        delete data_;
    }

    virtual color value(double u, double v, const Vec3& p) const override {
        // If we have no Texture data_, then return solid cyan as a debugging aid.
        if (data_ == nullptr)
            return color(0,1,1);

        // Clamp input Texture coordinates to [0,1] x [1,0]
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