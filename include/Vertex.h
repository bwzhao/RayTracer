#pragma once

#include "Vec3.h"
#include "Record.h"
#include "Material.h"

enum class VertexType {Camera, Light, Surface, Medium};

class Scene;

struct Vertex {
    VertexType type_;
    Color beta_; // product of the BSDF or phase function values, transmittances, and cosine terms for the vertices in the path generated so far
    ScatterRecord srec_;
    HitRecord rec_;
    double pdf_fwd_ = 0, pdf_rev_ = 0;

    const ScatterRecord & get_scatter_record() const;
    const HitRecord & get_hit_record() const;
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

    static inline Vertex create_camera();
    static inline Vertex create_light();
    static inline  Vertex CreateMedium(const ScatterRecord &srec_, HitRecord rec_,
                                       const Color &beta, const Vertex &prev);
    static inline Vertex CreateSurface(const ScatterRecord &srec_, HitRecord rec_,
                                       const Color &beta, const Vertex &prev);
};