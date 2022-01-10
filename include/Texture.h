#pragma once

#include "utils/rt_utils.h"
#include "Vec3.h"

#include "Perlin.h"

class Texture {
public:
    virtual Color value(double u, double v, const Point3& p) const = 0;
};

class SolidColor : public Texture {
public:
    SolidColor() {}
    SolidColor(Color c) : color_value_(c) {}

    SolidColor(double red, double green, double blue)
            : SolidColor(Color(red, green, blue)) {}

    virtual Color value(double u, double v, const Vec3& p) const override {
        return color_value_;
    }

private:
    Color color_value_;
};

class CheckerTexture : public Texture {
public:
    CheckerTexture() {}

    CheckerTexture(shared_ptr<Texture> _even, shared_ptr<Texture> _odd)
            : even_(_even), odd_(_odd) {}

    CheckerTexture(Color c1, Color c2)
            : even_(make_shared<SolidColor>(c1)) , odd_(make_shared<SolidColor>(c2)) {}

    virtual Color value(double u, double v, const Point3& p) const override {
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

    virtual Color value(double u, double v, const Point3& p) const override {
//        return Color(1,1,1) * 0.5 * (1.0 + noise_.noise(scale_ * p_));
        return Color(1, 1, 1) * 0.5 * (1 + sin(scale_ * p.z() + 10 * noise_.turb(p)));

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

    ImageTexture(const char* filename);

    ~ImageTexture() {
        delete data_;
    }

    virtual Color value(double u, double v, const Vec3& p) const override;

private:
    unsigned char *data_;
    int width_, height_;
    int bytes_per_scanline_;
};