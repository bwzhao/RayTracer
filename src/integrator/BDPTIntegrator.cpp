#include "integrator/BDPTIntegrator.h"
#include "Vertex.h"


int BDPTIntegrator::generate_camera_subpath(const Scene &scene, int max_depth, double u, double v, std::vector<Vertex> &path_camera) {
    if (max_depth == 0)
        return 0;
    auto r_ini = scene.cam_.sample_ray(u, v);

    Color beta = Vec3(1., 1., 1.);
    path_camera[0] = Vertex::create_camera(r_ini, beta);

    double pdf_pos = 1., pdf_dir = 1., pdf;

    pdf = pdf_pos * pdf_dir;
    return random_walk(scene, r_ini, beta, pdf, max_depth - 1, TransportMode::Forward, path_camera) + 1;
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

    return random_walk(scene, r_ini, beta, pdf, max_depth - 1, TransportMode::Backward, path_light) + 1;
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
            if (mode == TransportMode::Forward) {
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
            if (mode == TransportMode::Forward) {
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
            auto f = srec.attenuation_ * rec.mat_ptr_->scattering_bxdf(ray.direction(), rec.normal_, scattered_dir);

            cur_beta = cur_beta * f / pdf_fwd;
            vertex = Vertex::create_medium(srec, rec, cur_beta, prev, pdf_fwd);
            if (++bounces >= maxDepth)
                break;

            pdf_fwd = pdf_rev = pdf_val;
            ray = scattered_ray;
        }
        // If it is a surface
        else{
            // Specular surface
            if (srec.is_specular_) {
                const auto & wi = ray.direction();
                cur_beta = cur_beta * srec.attenuation_ * fabs(dot(wi, rec.normal_))/ pdf_fwd;
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

    Color L = Vec3(0.);
    int sample_count = 0;
    for (int t = 1; t <= n_path_camera; ++t) {
        for (int s = 0; s <= n_path_light; ++s) {
            int depth = t + s - 2;
            if ((s == 1 && t == 1) || depth < 0 || depth > max_depth)
                continue;
            Color Lpath = connect_BDPT(scene, path_light, path_camera, s, t);
            if (Lpath.x() < 0){
                continue;
            }

            L += Lpath;
            ++sample_count;
        }
    }
    if (sample_count != 0) {
        return L;
    }
    else{
        return Vec3(0.);
    }
}

void BDPTIntegrator::render_pixel_test(Scene & scene, double u, double v, int max_depth, int idx) {
    std::vector<Vertex> path_camera(max_depth + 2);
    std::vector<Vertex> path_light(max_depth + 1);

    int n_path_camera = generate_camera_subpath(scene, max_depth, u, v, path_camera);
    int n_path_light = generate_light_subpath(scene, max_depth, path_light);

    for (int t = 1; t <= n_path_camera; ++t) {
        for (int s = 0; s <= n_path_light; ++s) {
            int depth = t + s - 2;
            if ((s == 1 && t == 1) || depth < 0 || depth > max_depth)
                continue;
            Color Lpath = connect_BDPT(scene, path_light, path_camera, s, t);
            if (Lpath.x() < 0){
                continue;
            }
            if (s < 10 && t < 10) {
                scene.set_pixel(idx, Lpath, s, t);
            }
        }
    }
}

Color BDPTIntegrator::connect_BDPT(const Scene &scene, std::vector<Vertex> &lightVertices,
                                          std::vector<Vertex> &cameraVertices, int s, int t) {
    Color L = Vec3(0.);
    if (t > 1 && s != 0 && cameraVertices[t - 1].type_ == VertexType::Background) {
        return cameraVertices[t - 1].beta_;
    }

    Vertex sampled;
    if (s == 0) {
        const Vertex &pt = cameraVertices[t - 1];
        if (pt.type_ == VertexType::Background)
            L = pt.beta_;
    } else if (t == 1) {
        //TODO: Add this case in the future;
        L = Vec3(-1.);
    } else if (s == 1) {
        const Vertex &v_cam_path = cameraVertices[t - 1];
        const Vertex &v_light = lightVertices[0];
        if (v_cam_path.is_connectable()) {
            HitRecord rec;
            auto p1 = v_cam_path.p(), p2 = v_light.p();
            auto dir = unit_vector(p2 - p1);
            auto ray_1to2 = Ray(p1, dir, 0.);

            scene.world_ptr_->hit(ray_1to2, RAY_EPSILON, infinity, rec);
            if ((rec.p_ - p2).near_zero()){
                L = v_light.emitted(ray_1to2, rec, rec.u_, rec.v_, v_light.p());
            }

            Vec3 d = (p2 - p1);
            double g = 1 / d.length_squared();
            d *= std::sqrt(g);
            if (v_cam_path.is_on_surface()) {
                g *= abs_dot(v_cam_path.ng(), d);
            }
            g *= abs_dot(v_light.ng(), d);

            L *= g;
        }
    } else {
        const Vertex &v_light_path = lightVertices[s - 1], &v_cam_path = cameraVertices[t - 1];
        if (v_light_path.is_connectable() && v_cam_path.is_connectable()) {
            L = v_light_path.beta_ * v_light_path.f(v_cam_path) * v_cam_path.f(v_light_path) * v_cam_path.beta_;
            L *= G(scene, v_light_path, v_cam_path);
        }
    }

    return L;
}

Color BDPTIntegrator::G(const Scene &scene, const Vertex &v0, const Vertex &v1) {
        Vec3 d = v0.p() - v1.p();
        double g = 1 / d.length_squared();
        d *= std::sqrt(g);
        if (v0.is_on_surface())
            g *= abs_dot(v0.ng(), d);
        if (v1.is_on_surface())
            g *= abs_dot(v1.ng(), d);

        HitRecord rec;
        if (Vertex::vis_test(v0, v1, scene, rec)){
            return g;
        }
        else{
            return Color(0.);
        }
}
