#include "Scene.h"

#include "utils/io_utils.h"
#include "geometry/ObjectList.h"
#include "geometry/Sphere.h"
#include "Camera.h"
#include "Material.h"

Scene::Scene(int image_width, int image_height, double aspect_ratio, int samples_per_pixel, int max_depth):
image_width_(image_width),
image_height_(image_height),
aspect_ratio_(aspect_ratio),
samples_per_pixel_(samples_per_pixel),
max_depth_(max_depth),
background_(0, 0, 0)
{
    image_.resize(image_width_ * image_height_);
    samples_.resize(image_width_ * image_height_);
}

void Scene::set_camera(Point3 lookfrom, Point3 lookat, Vec3 vup, double vfov, double aspect_ratio, double aperture,
                       double focus_dist, double time0, double time1) {
    cam_.set_camera(lookfrom, lookat, vup, vfov, aspect_ratio, aperture, focus_dist, time0, time1);
}

void Scene::render() {
    for (int s = 0; s < samples_per_pixel_; ++s) {
        std::cout << "\rSamples remaining: " << samples_per_pixel_ - s - 1<< ' ' << std::flush;
        for (int j = image_height_ - 1; j >= 0; --j) {
            for (int i = 0; i < image_width_; ++i) {
                auto idx = i + (image_height_ - 1 - j) * image_width_;
                auto u = (i + random_double()) / (image_width_ - 1);
                auto v = (j + random_double()) / (image_height_ - 1);
                Ray r = cam_.get_ray(u, v);
                auto pixel_color = path_integrator(r, max_depth_);
                set_pixel(idx, pixel_color);
            }
        }
    }
    std::cout << "\nDone.\n";
}

Color Scene::path_integrator(
        const Ray& r,
        int depth
) {
    HitRecord rec;

    // If we've exceeded the Ray bounce limit, no more light is gathered.
    if (depth <= 0)
        return Color(0, 0, 0);

    // If the Ray hits nothing, return the background Color.
    if (!world_ptr_->hit(r, RAY_EPSILON, infinity, rec))
        return background_;

    ScatterRecord srec;
    Color emitted = rec.mat_ptr_->emitted(r, rec, rec.u_, rec.v_, rec.p_);
    if (!rec.mat_ptr_->scatter(r, rec, srec))
        return emitted;

    if (srec.is_specular_) {
        return srec.attenuation_
               * path_integrator(srec.specular_ray_, depth - 1);
    }

    auto light_ptr = make_shared<ObjectPdf>(lights_ptr_, rec.p_);
    MixturePdf p(light_ptr, srec.pdf_ptr_);

    Ray scattered = Ray(rec.p_, p.generate(), r.time());
    auto pdf_val = p.value(scattered.direction());

    return emitted
           + srec.attenuation_ * rec.mat_ptr_->scattering_pdf(r, rec, scattered)
             * path_integrator(scattered, depth - 1) / pdf_val;
}

void Scene::set_pixel(int idx, Color pixel_color) {
    auto r = pixel_color.x();
    auto g = pixel_color.y();
    auto b = pixel_color.z();

    // Replace NaN components with zero. See explanation in Ray Tracing: The Rest of Your Life.
    if (r != r) r = 0.0;
    if (g != g) g = 0.0;
    if (b != b) b = 0.0;

    samples_[idx]++;
    image_[idx] += Color(r, g, b);
}

void Scene::write_image(char *file_name) {
    // save framebuffer to file
    std::ofstream outfile(file_name, std::ofstream::out);
    outfile << "P3\n" << image_width_ << ' ' << image_height_ << "\n255\n";

    for (auto idx = 0; idx < image_height_ * image_width_; ++idx) {
        static unsigned char color[3];
        auto scale = 1.0 / samples_[idx];
        auto r = sqrt(scale * image_[idx][0]);
        auto g = sqrt(scale * image_[idx][1]);
        auto b = sqrt(scale * image_[idx][2]);
        color[0] = (char)(255 * clamp(0, 1, r));
        color[1] = (char)(255 * clamp(0, 1, g));
        color[2] = (char)(255 * clamp(0, 1, b));

        outfile << static_cast<int>(256 * clamp(r, 0.0, 0.999)) << ' '
            << static_cast<int>(256 * clamp(g, 0.0, 0.999)) << ' '
            << static_cast<int>(256 * clamp(b, 0.0, 0.999)) << '\n';
    }
    outfile.close();
}
