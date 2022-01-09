#pragma once

#include "utils/rt_utils.h"

#include "Texture.h"
#include "Onb.h"
#include "Pdf.h"

struct HitRecord;

struct ScatterRecord {
    Ray specular_ray;
    bool is_specular;
    color attenuation;
    shared_ptr<Pdf> pdf_ptr;
};

class Material {
public:
    virtual color emitted(
            const Ray& r_in, const HitRecord& rec, double u, double v, const point3& p
    ) const {
        return color(0,0,0);
    }

    virtual bool scatter(
            const Ray& r_in, const HitRecord& rec, ScatterRecord& srec
    ) const {
        return false;
    }

    virtual double scattering_pdf(
            const Ray& r_in, const HitRecord& rec, const Ray& scattered
    ) const {
        return 0;
    }
};

class Lambertian : public Material {
public:
    Lambertian(const color& a) : albedo_(make_shared<SolidColor>(a)) {}
    Lambertian(shared_ptr<Texture> a) : albedo_(a) {}

    virtual bool scatter(
            const Ray& r_in, const HitRecord& rec, ScatterRecord& srec
    ) const override {
        srec.is_specular = false;
        srec.attenuation = albedo_->value(rec.u_, rec.v_, rec.p_);
        srec.pdf_ptr = static_cast<const shared_ptr<Pdf>>(new CosinePdf(rec.normal_));
        return true;
    }

    double scattering_pdf(
            const Ray& r_in, const HitRecord& rec, const Ray& scattered
    ) const override{
        auto cosine = dot(rec.normal_, unit_vector(scattered.direction()));
        return cosine < 0 ? 0 : cosine/pi;
    }

public:
    shared_ptr<Texture> albedo_;
};

class Metal : public Material {
public:
    Metal(const color& a, double f) : albedo_(a), fuzz_(f < 1 ? f : 1) {}

    virtual bool scatter(
            const Ray& r_in, const HitRecord& rec, ScatterRecord& srec
    ) const override {
        Vec3 reflected = reflect(unit_vector(r_in.direction()), rec.normal_);
        srec.specular_ray = Ray(rec.p_, reflected + fuzz_ * random_in_unit_sphere());
        srec.attenuation = albedo_;
        srec.is_specular = true;
        srec.pdf_ptr = 0;
        return true;
    }

public:
    color albedo_;
    double fuzz_;
};

class Dielectric : public Material {
public:
    Dielectric(double index_of_refraction) : ir_(index_of_refraction) {}

    virtual bool scatter(
            const Ray& r_in, const HitRecord& rec, ScatterRecord& srec
    ) const override {
        srec.is_specular = true;
        srec.pdf_ptr = nullptr;
        srec.attenuation = color(1.0, 1.0, 1.0);
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

        srec.specular_ray = Ray(rec.p_, direction, r_in.time());
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
    DiffuseLight(color c) : emit_(make_shared<SolidColor>(c)) {}

    virtual bool scatter(
            const Ray& r_in, const HitRecord& rec, ScatterRecord& srec
    ) const {
        return false;
    }

    virtual color emitted(const Ray& r_in, const HitRecord& rec, double u, double v,
                          const point3& p) const {

        if (rec.front_face_)
            return emit_->value(u, v, p);
        else
            return color(0,0,0);
    }

public:
    shared_ptr<Texture> emit_;
};

//class isotropic : public material {
//public:
//    isotropic(color c) : albedo_(make_shared<SolidColor>(c)) {}
//    isotropic(shared_ptr<Texture> a) : albedo_(a) {}
//
//    virtual bool scatter(
//            const Ray& r_in, const HitRecord& rec, color& attenuation, Ray& scattered, double& Pdf
//    ) const override {
//        scattered = Ray(rec.p_, random_in_unit_sphere(), r_in.time());
//        attenuation = albedo_->value(rec.u_, rec.v_, rec.p_);
//        return true;
//    }
//
//public:
//    shared_ptr<Texture> albedo_;
//};


