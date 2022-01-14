#pragma once

#include "utils/rt_utils.h"

#include "Texture.h"
#include "Onb.h"
#include "Pdf.h"
#include "Sampler.h"
#include "Record.h"

struct HitRecord;

class Material {
public:
    virtual Color emitted(
            const Ray& r_in, const HitRecord& rec, double u, double v, const Point3& p
    ) const {
        return Color(0, 0, 0);
    }

    virtual bool scatter(
            const Ray& r_in, const HitRecord& rec, ScatterRecord& srec
    ) const {
        return false;
    }

    virtual double scattering_bxdf(
            const Vec3 & wi, const HitRecord& rec,  const Vec3 & wo
    ) const {
        return 0;
    }
};

class Lambertian : public Material {
public:
    Lambertian(const Color& a) : albedo_(make_shared<SolidColor>(a)) {}
    Lambertian(shared_ptr<Texture> a) : albedo_(a) {}

    virtual bool scatter(
            const Ray& r_in, const HitRecord& rec, ScatterRecord& srec
    ) const override {
        srec.is_specular_ = false;
        srec.attenuation_ = albedo_->value(rec.u_, rec.v_, rec.p_);
        srec.pdf_ptr_ = static_cast<const shared_ptr<Pdf>>(new CosinePdf(rec.normal_));
        return true;
    }

    virtual double scattering_bxdf(
            const Vec3 & wi, const HitRecord& rec,  const Vec3 & wo
    ) const override{
        auto cosine = dot(rec.normal_, unit_vector(wo));
        return cosine < 0 ? 0 : cosine/pi;
    }

public:
    shared_ptr<Texture> albedo_;
};

class Metal : public Material {
public:
    Metal(const Color& a, double f) : albedo_(a), fuzz_(f < 1 ? f : 1) {}

    virtual bool scatter(
            const Ray& r_in, const HitRecord& rec, ScatterRecord& srec
    ) const override {
        Vec3 reflected = reflect(unit_vector(r_in.direction()), rec.normal_);
        srec.specular_ray_ = Ray(rec.p_, reflected + fuzz_ * random_in_unit_sphere());
        srec.attenuation_ = albedo_;
        srec.is_specular_ = true;
        srec.pdf_ptr_ = 0;
        return true;
    }

public:
    Color albedo_;
    double fuzz_;
};

class Dielectric : public Material {
public:
    Dielectric(double index_of_refraction) : ir_(index_of_refraction) {}

    virtual bool scatter(
            const Ray& r_in, const HitRecord& rec, ScatterRecord& srec
    ) const override {
        srec.is_specular_ = true;
        srec.pdf_ptr_ = nullptr;
        srec.attenuation_ = Color(1.0, 1.0, 1.0);
        double refraction_ratio = rec.front_face_ ? (1.0/ir_) : ir_;

        Vec3 unit_direction = unit_vector(r_in.direction());
//        Vec3 refracted = refract(unit_direction, rec.normal_, refraction_ratio);
        double cos_theta = fmin(dot(-unit_direction, rec.normal_), 1.0);
        double sin_theta = sqrt(1.0 - cos_theta*cos_theta);

        bool cannot_refract = refraction_ratio * sin_theta > 1.0;
        Vec3 direction;

        if (cannot_refract || reflectance(cos_theta, refraction_ratio) > random_double())
            direction = reflect(unit_direction, rec.normal_);
        else
            direction = refract(unit_direction, rec.normal_, refraction_ratio);

        srec.specular_ray_ = Ray(rec.p_, direction, r_in.time());
        return true;
    }

public:
    double ir_; // Index of Refraction

private:
    static double reflectance(double cosine, double ref_idx) {
        // Use Schlick's approximation for reflectance.
        auto r0 = (1-ref_idx) / (1+ref_idx);
        r0 = r0*r0;
        return r0 + (1-r0)*pow((1 - cosine),5);
    }
};

class DiffuseLight : public Material  {
public:
    DiffuseLight(shared_ptr<Texture> a) : emit_(a) {}
    DiffuseLight(Color c) : emit_(make_shared<SolidColor>(c)) {}

    virtual bool scatter(
            const Ray& r_in, const HitRecord& rec, ScatterRecord& srec
    ) const {
        return false;
    }

    virtual Color emitted(const Ray& r_in, const HitRecord& rec, double u, double v,
                          const Point3& p) const {

        if (rec.front_face_)
            return emit_->value(u, v, p);
        else
            return Color(0, 0, 0);
    }

public:
    shared_ptr<Texture> emit_;
};

class Isotropic : public Material {
public:
    Isotropic(Color c) : albedo_(make_shared<SolidColor>(c)) {}
    Isotropic(shared_ptr<Texture> a) : albedo_(a) {}

    virtual bool scatter(
            const Ray& r_in, const HitRecord& rec, ScatterRecord& srec
    ) const override {
        srec.is_specular_ = false;
        srec.attenuation_ = albedo_->value(rec.u_, rec.v_, rec.p_);
        srec.pdf_ptr_ = static_cast<const shared_ptr<Pdf>>(new IsotropicPdf());
        return true;
    }

    virtual double scattering_bxdf(
            const Vec3 & wi, const HitRecord& rec,  const Vec3 & wo
    ) const override{
        return 0.25 / pi;
    }

public:
    shared_ptr<Texture> albedo_;
};


