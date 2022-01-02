#pragma once

#include "utils.h"

struct hit_record;

class material {
public:
    virtual bool scatter(
            const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered
    ) const = 0;
};


class lambertian : public material {
public:
    lambertian(const color& a) : albedo_(a) {}

    virtual bool scatter(
            const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered
    ) const override {
        auto scatter_direction = rec.normal_ + random_unit_vector();

        // Catch degenerate scatter direction
        if (scatter_direction.near_zero())
            scatter_direction = rec.normal_;

        scattered = ray(rec.p_, scatter_direction, r_in.time());
        attenuation = albedo_;

        return true;
    }

public:
    color albedo_;
};


class metal : public material {
public:
    metal(const color& a, double f) : albedo_(a), fuzz_(f < 1 ? f : 1) {}

    virtual bool scatter(
            const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered
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
            const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered
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