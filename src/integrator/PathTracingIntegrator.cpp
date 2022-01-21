#include "integrator/PathTracingIntegrator.h"
#include "Scene.h"

Color PathTracingIntegrator::get_radiance(
        const Ray& r,
        const Scene & scene,
        int depth
) {
    HitRecord rec;

    // If we've exceeded the Ray bounce limit, no more light is gathered.
    if (depth <= 0)
        return Color(0, 0, 0);

    // If the Ray hits nothing, return the background Color.
    if (!scene.world_ptr_->hit(r, RAY_EPSILON, infinity, rec))
        return scene.background_;

    ScatterRecord srec;
    Color emitted = rec.mat_ptr_->emitted(r, rec, rec.u_, rec.v_, rec.p_);
    if (!rec.mat_ptr_->scatter(r, rec, srec))
        return emitted;

    if (srec.is_specular_) {
        return srec.attenuation_
               * get_radiance(srec.specular_ray_, scene, depth - 1);
    }

//    auto light_ptr = make_shared<ObjectPdf>(scene.focus_lights_ptr_, rec.p_);
//    MixturePdf p(light_ptr, srec.pdf_ptr_);
    auto p = srec.pdf_ptr_;

    Ray scattered = Ray(rec.p_, p->get_random_dir(), r.time());
    auto pdf_val = p->get_pdf(scattered.direction());

    return emitted
           + srec.attenuation_ * rec.mat_ptr_->scattering_bxdf(r.direction(), rec.normal_, scattered.direction())
             * get_radiance(scattered, scene, depth - 1) / pdf_val;
}

Color PathTracingIntegrator::render_pixel(const Scene & scene, double u, double v, int max_depth) {
    Ray r = scene.cam_.sample_ray(u, v);
    return get_radiance(r, scene, max_depth);
}


