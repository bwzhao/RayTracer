#pragma once

#include "rt_utils.h"

#include "texture.h"

struct hit_record;

class material {
public:
    virtual color emitted(double u, double v, const point3& p) const {
        return color(0,0,0);
    }
    virtual bool scatter(
            const ray& r_in, const hit_record& rec, color& albedo, ray& scattered, double& pdf
    ) const {
        return false;
    }

    virtual double scattering_pdf(
            const ray& r_in, const hit_record& rec, const ray& scattered
    ) const {
        return 0;
    }
};


class lambertian : public material {
public:
    lambertian(const color& a) : albedo_(make_shared<solid_color>(a)) {}
    lambertian(shared_ptr<texture> a) : albedo_(a) {}

    virtual bool scatter(
            const ray& r_in, const hit_record& rec, color& alb, ray& scattered, double& pdf
    ) const override {
        auto direction = random_in_hemisphere(rec.normal_);
        scattered = ray(rec.p_, unit_vector(direction), r_in.time());
        alb = albedo_->value(rec.u_, rec.v_, rec.p_);
        pdf = dot(rec.normal_, scattered.direction()) / pi;

        return true;
    }
    double scattering_pdf(
            const ray& r_in, const hit_record& rec, const ray& scattered
    ) const {
        auto cosine = dot(rec.normal_, unit_vector(scattered.direction()));
        return cosine < 0 ? 0 : cosine/pi;
    }

public:
    shared_ptr<texture> albedo_;
};


class metal : public material {
public:
    metal(const color& a, double f) : albedo_(a), fuzz_(f < 1 ? f : 1) {}

    virtual bool scatter(
            const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered, double& pdf
    ) const override {
        vec3 reflected = reflect(unit_vector(r_in.direction()), rec.normal_);
        scattered = ray(rec.p_, reflected + fuzz_*random_in_unit_sphere(), r_in.time());
        attenuation = albedo_;
        return (dot(scattered.direction(), rec.normal_) > 0);
    }

public:
    color albedo_;
    double fuzz_;
};


class dielectric : public material {
public:
    dielectric(double index_of_refraction) : ir_(index_of_refraction) {}

    virtual bool scatter(
            const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered, double& pdf
    ) const override {
        attenuation = color(1.0, 1.0, 1.0);
        double refraction_ratio = rec.front_face_ ? (1.0/ir_) : ir_;

        vec3 unit_direction = unit_vector(r_in.direction());
//        vec3 refracted = refract(unit_direction, rec.normal_, refraction_ratio);
        double cos_theta = fmin(dot(-unit_direction, rec.normal_), 1.0);
        double sin_theta = sqrt(1.0 - cos_theta*cos_theta);

        bool cannot_refract = refraction_ratio * sin_theta > 1.0;
        vec3 direction;

        if (cannot_refract || reflectance(cos_theta, refraction_ratio) > random_double())
            direction = reflect(unit_direction, rec.normal_);
        else
            direction = refract(unit_direction, rec.normal_, refraction_ratio);

        scattered = ray(rec.p_, direction, r_in.time());
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

class diffuse_light : public material  {
public:
    diffuse_light(shared_ptr<texture> a) : emit_(a) {}
    diffuse_light(color c) : emit_(make_shared<solid_color>(c)) {}

    virtual bool scatter(
            const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered, double& pdf
    ) const override {
        return false;
    }

    virtual color emitted(double u, double v, const point3& p) const override {
        return emit_->value(u, v, p);
    }

public:
    shared_ptr<texture> emit_;
};

class isotropic : public material {
public:
    isotropic(color c) : albedo_(make_shared<solid_color>(c)) {}
    isotropic(shared_ptr<texture> a) : albedo_(a) {}

    virtual bool scatter(
            const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered, double& pdf
    ) const override {
        scattered = ray(rec.p_, random_in_unit_sphere(), r_in.time());
        attenuation = albedo_->value(rec.u_, rec.v_, rec.p_);
        return true;
    }

public:
    shared_ptr<texture> albedo_;
};

