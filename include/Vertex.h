#pragma once

#include "Vec3.h"
#include "Record.h"
#include "Material.h"
#include "Light.h"

enum class VertexType {Camera, Light, Surface, Medium};

class Scene;

struct Vertex {
    VertexType type_;
    // the product of the BSDF or phase function values, transmittances,
    // and cosine terms for the vertices in the path generated so far,
    // divided by their respective sampling PDFs
    Color beta_;
    Point3 p_;
    bool is_specular_;
    std::shared_ptr<Material> mat_ptr_;
    Vec3 wi_, normal_;
    std::shared_ptr<Pdf> pdf_ptr_;

    double pdf_fwd_ = 1., pdf_rev_ = 1.;
    double pdf_pos_ = 1.;

    const Point3 &p() const;
    bool is_specular() const;
    double get_bxdf(const Vertex &next) const;
    bool is_connectable() const;
    bool is_light() const;
    Color emitted (const Ray& r_in, const HitRecord& rec, double u, double v, const Point3& p);
    Vec3 ng() const;
    bool is_on_surface() const;
    double convert_density(double pdf, const Vertex &next) const;
    double get_pdf (const Scene &scene, const Vertex *prev,
                    const Vertex &next);
    double pdf_light (const Scene &scene, const Vertex &next) const;
    double pdf_light_origin(const Scene &scene, const Vertex &v) const;

    static inline Vertex create_camera(const Ray & ray, const Color &beta);
    static inline Vertex create_light(const Light & light, const Ray & ray, const Color &beta, double pdf_pos);
    static inline  Vertex create_medium(const ScatterRecord &srec_, HitRecord rec_,
                                        const Color &beta, const Vertex &prev);
    static inline Vertex create_surface(const ScatterRecord &srec_, HitRecord rec_,
                                        const Color &beta, const Vertex &prev);
};