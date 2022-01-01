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

        scattered = ray(rec.p_, scatter_direction);
        attenuation = albedo_;

        return true;
    }

public:
    color albedo_;
};


class metal : public material {
public:
    metal(const color& a) : albedo_(a) {}

    virtual bool scatter(
            const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered
    ) const override {
        vec3 reflected = reflect(unit_vector(r_in.direction()), rec.normal_);
        scattered = ray(rec.p_, reflected);
        attenuation = albedo_;
        return (dot(scattered.direction(), rec.normal_) > 0);
    }

public:
    color albedo_;
};