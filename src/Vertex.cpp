#include "Vertex.h"

const ScatterRecord &Vertex::get_scatter_record() const {
    return srec_;
}

const HitRecord &Vertex::get_hit_record() const {
    return rec_;
}

const Point3 &Vertex::p() const { return get_hit_record().p_;}

bool Vertex::is_specular() const {return get_scatter_record().is_specular_;}

double Vertex::get_bxdf(const Vertex &next) const {
    auto wo = unit_vector(next.p() - p());
    return get_hit_record().mat_ptr_->scattering_bxdf(rec_.wi_, rec_, wo);
}

bool Vertex::is_connectable() const {
    switch (type_) {
        case VertexType::Medium:  return true;
        case VertexType::Light:   return true;
        case VertexType::Camera:  return true;
        case VertexType::Surface: return !srec_.is_specular_;
    }
}

bool Vertex::is_light() const {
    return type_ == VertexType::Light;
}

Color Vertex::emitted(const Ray &r_in, const HitRecord &rec, double u, double v, const Point3 &p) {return rec_.mat_ptr_->emitted(r_in, rec, u, v, p);}

bool Vertex::is_on_surface() const { return ng() != Vec3(); }

double Vertex::convert_density(double pdf, const Vertex &next) const {
    Vec3 w = next.p() - p();
    auto invDist2 = 1 / w.length_squared();
    if (next.is_on_surface())
        pdf *= fabs(dot(next.ng(), w * std::sqrt(invDist2)));
    return pdf * invDist2;
}

Vec3 Vertex::ng() const{return rec_.normal_;}

double Vertex::get_pdf(const Scene &scene, const Vertex *prev, const Vertex &next) {
    if (type_ == VertexType::Light)
        return pdf_light(scene, next);
    Vec3 wp, wn = unit_vector(next.p() - p());
    if (prev)
        wp = unit_vector(prev->p() - p());
    double pdf = 0;
    if (type_ == VertexType::Surface || type_ == VertexType::Medium) {
        pdf = get_scatter_record().pdf_ptr_->get_pdf_dir(wn);
    }
    return convert_density(pdf, next);
}

double Vertex::pdf_light(const Scene &scene, const Vertex &next) const {
    Vec3 w = next.p() - p();
    double invDist2 = 1 / w.length_squared();
    w *= std::sqrt(invDist2);
    double pdf = get_scatter_record().pdf_ptr_->get_pdf_dir(w);
    if (next.is_on_surface())
        pdf *= fabs(dot(next.ng(), w));
    return pdf;
}

double Vertex::pdf_light_origin(const Scene &scene, const Vertex &v) const {
    Vec3 w = unit_vector(v.p() - p());

    return 0;
}

Vertex Vertex::create_camera() {
    auto temp_vertex = Vertex();
    temp_vertex.type_ = VertexType::Camera;
    return temp_vertex;
}

Vertex Vertex::create_light() {
    auto temp_vertex = Vertex();
    temp_vertex.type_ = VertexType::Light;
    return temp_vertex;
}

Vertex Vertex::CreateMedium(const ScatterRecord &srec, HitRecord rec, const Color &beta, const Vertex &prev) {
    auto temp_vertex = Vertex();
    temp_vertex.type_ = VertexType::Medium;
    temp_vertex.srec_ = srec;
    temp_vertex.rec_ = rec;
    return temp_vertex;
}

Vertex Vertex::CreateSurface(const ScatterRecord &srec, HitRecord rec, const Color &beta, const Vertex &prev) {
    auto temp_vertex = Vertex();
    temp_vertex.type_ = VertexType::Surface;
    temp_vertex.srec_ = srec;
    temp_vertex.rec_ = rec;
    return temp_vertex;
}
