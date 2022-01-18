#include "Vertex.h"

const Point3 &Vertex::p() const { return p_;}

bool Vertex::is_specular() const {return is_specular_;}

double Vertex::get_bxdf(const Vertex &next) const {
    auto wo = unit_vector(next.p() - p());
    return mat_ptr_->scattering_bxdf(wi_, normal_, wo);
}

bool Vertex::is_connectable() const {
    switch (type_) {
        case VertexType::Medium:  return true;
        case VertexType::Light:   return true;
        case VertexType::Camera:  return true;
        case VertexType::Surface: return !is_specular_;
    }
}

bool Vertex::is_light() const {
    return type_ == VertexType::Light;
}

Color Vertex::emitted(const Ray &r_in, const HitRecord &rec, double u, double v, const Point3 &p) {return mat_ptr_->emitted(r_in, rec, u, v, p);}

bool Vertex::is_on_surface() const { return ng() != Vec3(); }

double Vertex::convert_density(double pdf, const Vertex &next) const {
    Vec3 w = next.p() - p();
    auto invDist2 = 1 / w.length_squared();
    if (next.is_on_surface())
        pdf *= fabs(dot(next.ng(), w * std::sqrt(invDist2)));
    return pdf * invDist2;
}

Vec3 Vertex::ng() const{return normal_;}

double Vertex::get_pdf(const Scene &scene, const Vertex *prev, const Vertex &next) {
    if (type_ == VertexType::Light)
        return pdf_light(scene, next);
    Vec3 wp, wn = unit_vector(next.p() - p());
    if (prev)
        wp = unit_vector(prev->p() - p());
    double pdf = 0;
    if (type_ == VertexType::Surface || type_ == VertexType::Medium) {
        pdf = pdf_ptr_->get_pdf(wn);
    }
    return convert_density(pdf, next);
}

double Vertex::pdf_light(const Scene &scene, const Vertex &next) const {
    Vec3 w = next.p() - p();
    double invDist2 = 1 / w.length_squared();
    w *= std::sqrt(invDist2);
    double pdf = pdf_ptr_->get_pdf(w);
    if (next.is_on_surface())
        pdf *= fabs(dot(next.ng(), w));
    return pdf;
}

double Vertex::pdf_light_origin(const Scene &scene, const Vertex &v) const {
    Vec3 w = unit_vector(v.p() - p());

    return 0;
}

Vertex Vertex::create_camera(const Ray & ray, const Color &beta) {
    auto temp_vertex = Vertex();
    temp_vertex.type_ = VertexType::Camera;

    temp_vertex.beta_ = beta;
    temp_vertex.p_ = ray.origin();
    temp_vertex.is_specular_ = false;
    temp_vertex.mat_ptr_ = nullptr;
    temp_vertex.wi_ = Vec3(0, 0, 0);
    temp_vertex.normal_ = ray.direction();
    temp_vertex.pdf_ptr_ = nullptr;

    return temp_vertex;
}

Vertex Vertex::create_light(const Light & light, const Ray & ray, const Color &beta, double pdf_pos){
    auto temp_vertex = Vertex();
    temp_vertex.type_ = VertexType::Light;

    temp_vertex.beta_ = beta;
    temp_vertex.p_ = ray.origin();
    temp_vertex.is_specular_ = false;
    temp_vertex.mat_ptr_ = light.object_ptr_->mat_ptr_;
    temp_vertex.wi_ = Vec3(0, 0, 0);
    temp_vertex.normal_ = ray.direction();
    temp_vertex.pdf_ptr_ = light.pdf_ptr;
    temp_vertex.pdf_pos_ = pdf_pos;

    return temp_vertex;
}

Vertex Vertex::create_medium(const ScatterRecord &srec_, HitRecord rec_, const Color &beta, const Vertex &prev) {
    auto temp_vertex = Vertex();
    temp_vertex.type_ = VertexType::Medium;

    temp_vertex.beta_ = beta;
    temp_vertex.p_ = rec_.p_;
    temp_vertex.is_specular_ = srec_.is_specular_;
    temp_vertex.mat_ptr_ = rec_.mat_ptr_;
    temp_vertex.wi_ = rec_.wi_;
    temp_vertex.normal_ = rec_.normal_;
    temp_vertex.pdf_ptr_ = srec_.pdf_ptr_;

    return temp_vertex;
}

Vertex Vertex::create_surface(const ScatterRecord &srec_, HitRecord rec_, const Color &beta, const Vertex &prev) {
    auto temp_vertex = Vertex();
    temp_vertex.type_ = VertexType::Surface;

    temp_vertex.beta_ = beta;
    temp_vertex.p_ = rec_.p_;
    temp_vertex.is_specular_ = srec_.is_specular_;
    temp_vertex.mat_ptr_ = rec_.mat_ptr_;
    temp_vertex.wi_ = rec_.wi_;
    temp_vertex.normal_ = rec_.normal_;
    temp_vertex.pdf_ptr_ = srec_.pdf_ptr_;

    return temp_vertex;
}
