#include "integrator/BDPTIntegrator.h"


int BDPTIntegrator::generate_camera_subpath(const Scene &scene, int max_depth, double u, double v, std::vector<Vertex> &path_camera) {
    if (max_depth == 0)
        return 0;
    auto r_ini = scene.cam_.sample_ray(u, v);

    Color beta = Vec3(1., 1., 1.);
    path_camera[0] = Vertex::create_camera(r_ini, beta);

    double pdf_pos = 1., pdf_dir = 1., pdf;

    pdf = pdf_pos * pdf_dir;
    return random_walk(scene, r_ini, beta, pdf, max_depth - 1, TransportMode::Radiance, path_camera) + 1;
}

int BDPTIntegrator::generate_light_subpath(const Scene &scene, int max_depth, std::vector<Vertex> &path_light) {
    if (max_depth == 0)
        return 0;
    auto r_ini = Ray();

    Color beta = Vec3(1., 1., 1.);
    Vec3 normal;
    double pdf_pos = 1., pdf_dir = 1., pdf;

    beta = scene.light_ptr_->sample_ray(r_ini, pdf_pos, pdf_dir, normal);
    pdf = pdf_pos * pdf_dir;
    beta *= dot(r_ini.direction(), normal);
    path_light[0] = Vertex::create_light(*scene.get_light_ptr(), r_ini, beta, pdf, normal);

    return random_walk(scene, r_ini, beta, pdf, max_depth - 1, TransportMode::Importance, path_light) + 1;
}

int BDPTIntegrator::random_walk(const Scene &scene, Ray ray, Color cur_beta, double pdf, int maxDepth, TransportMode mode, std::vector<Vertex> & path) {
    if (maxDepth == 0)
        return 0;
    int bounces = 0;
    double pdf_fwd = pdf, pdf_rev = 0.;

    while (true) {
        HitRecord rec;
        Vertex &vertex = path[bounces + 1], &prev = path[bounces];

        // If the Ray hits nothing
        if (!scene.world_ptr_->hit(ray, RAY_EPSILON, infinity, rec)){
            if (mode == TransportMode::Radiance) {
                cur_beta = cur_beta * scene.background_ / pdf_fwd;
                vertex = Vertex::create_background(rec, cur_beta, prev, pdf_fwd);
                ++bounces;
            }
            break;
        }

        ScatterRecord srec;
        Color emitted = rec.mat_ptr_->emitted(ray, rec, rec.u_, rec.v_, rec.p_);
        // If it hits a light
        if (!rec.mat_ptr_->scatter(ray, rec, srec)){
            if (mode == TransportMode::Radiance) {
                cur_beta = cur_beta * emitted / pdf_fwd;
                vertex = Vertex::create_background(rec, cur_beta, prev, pdf_fwd);
                ++bounces;
            }
            break;
        }

        // If the ray is a medium
        if (rec.mat_ptr_->is_medium()) {
            Vec3 scattered_dir = srec.pdf_ptr_->get_random_dir();
            Ray scattered_ray = Ray(rec.p_, scattered_dir, ray.time());
            auto pdf_val = srec.pdf_ptr_->get_pdf(scattered_dir);
            auto f = cur_beta * srec.attenuation_ * rec.mat_ptr_->scattering_bxdf(ray.direction(), rec.normal_, scattered_dir);

            cur_beta = cur_beta * f / pdf_fwd;
            vertex = Vertex::create_medium(srec, rec, cur_beta, prev, pdf_fwd);
            if (++bounces >= maxDepth)
                break;

            pdf_fwd = pdf_rev = pdf_val;
            ray = scattered_ray;
        }
        // If it is a surface
        else{
            if (srec.is_specular_) {
                cur_beta = cur_beta * srec.attenuation_ / pdf_fwd;
                vertex = Vertex::create_surface(srec, rec, cur_beta, prev, pdf_fwd);
                vertex.is_specular_ = true;

                if (++bounces >= maxDepth)
                    break;
                pdf_fwd = pdf_rev = 1.;

                ray = srec.specular_ray_;
            }
            else{
                Vec3 wo = srec.pdf_ptr_->get_random_dir();
                Ray scattered_ray = Ray(rec.p_, wo, ray.time());
                auto f = srec.attenuation_ * rec.mat_ptr_->scattering_bxdf(ray.direction(), rec.normal_, wo);

                const auto & wi = ray.direction();

                cur_beta = cur_beta * f * fabs(dot(wi, rec.normal_)) / pdf_fwd;
                vertex = Vertex::create_surface(srec, rec, cur_beta, prev, pdf_fwd);
                if (++bounces >= maxDepth)
                    break;

                pdf_fwd = srec.pdf_ptr_->get_pdf(wo);
                if (cur_beta.near_zero() || pdf_fwd == 0.f)
                    break;

                pdf_rev = srec.pdf_ptr_->get_pdf(-wi);

                ray = scattered_ray;
            }
        }
        prev.pdf_rev_ = vertex.convert_density(pdf_rev, prev);
    }


    return bounces;
}

Color BDPTIntegrator::render_pixel(const Scene & scene, double u, double v, int max_depth) {
    std::vector<Vertex> path_camera(max_depth + 2);
    std::vector<Vertex> path_light(max_depth + 1);

    int n_path_camera = generate_camera_subpath(scene, max_depth, u, v, path_camera);
    int n_path_light = generate_light_subpath(scene, max_depth, path_light);

    return path_camera[n_path_camera - 1].beta_;
}
