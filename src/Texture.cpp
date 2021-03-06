#include "Texture.h"
#include "utils/image_utils.h"


ImageTexture::ImageTexture(const char* filename) {
    auto components_per_pixel = bytes_per_pixel;

    data_ = stbi_load(
            filename, &width_, &height_, &components_per_pixel, components_per_pixel);

    if (!data_) {
        std::cerr << "ERROR: Could not load Texture image file '" << filename << "'.\n";
        width_ = height_ = 0;
    }

    bytes_per_scanline_ = bytes_per_pixel * width_;
}

Color ImageTexture::value(double u, double v, const Vec3 &p) const {
    // If we have no Texture data_, then return solid cyan as a debugging aid.
    if (data_ == nullptr)
        return Color(0,1,1);

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

    return Color(color_scale*pixel[0], color_scale*pixel[1], color_scale*pixel[2]);
}