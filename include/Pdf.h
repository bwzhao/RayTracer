#pragma once

#include "geometry/Object.h"
#include "Onb.h"

class Pdf {
public:
    virtual ~Pdf() {}

    virtual double get_pdf(const Vec3& direction) const = 0;
    virtual Vec3 get_random_dir() const = 0;
};

inline Vec3 random_cosine_direction() {
    auto r1 = random_double();
    auto r2 = random_double();
    auto z = sqrt(1-r2);

    auto phi = 2*pi*r1;
    auto x = cos(phi)*sqrt(r2);
    auto y = sin(phi)*sqrt(r2);

    return Vec3(x, y, z);
}

class CosinePdf : public Pdf {
public:
    CosinePdf(const Vec3& w) { uvw.build_from_w(w); }

    virtual double get_pdf(const Vec3& direction) const override {
        auto cosine = dot(unit_vector(direction), uvw.w());
        return (cosine <= 0) ? 0 : cosine/pi;
    }

    virtual Vec3 get_random_dir() const override {
        return uvw.local(random_cosine_direction());
    }

    void set_normal_dir(const Vec3& w) {
        uvw.build_from_w(w);
    }

public:
    Onb uvw;
};

class ObjectPdf : public Pdf {
public:
    ObjectPdf(shared_ptr<Object> p, const Point3& origin) : ptr_(p), o_(origin) {}

    virtual double get_pdf(const Vec3& direction) const override {
        return ptr_->pdf_value_from_point(o_, direction);
    }

    virtual Vec3 get_random_dir() const override {
        return ptr_->random_from_point(o_);
    }

public:
    Point3 o_;
    shared_ptr<Object> ptr_;
};

class MixturePdf : public Pdf {
public:
    MixturePdf(shared_ptr<Pdf> p0, shared_ptr<Pdf> p1) {
        p_[0] = p0;
        p_[1] = p1;
    }

    virtual double get_pdf(const Vec3& direction) const override {
        return 0.5 * p_[0]->get_pdf(direction) + 0.5 * p_[1]->get_pdf(direction);
    }

    virtual Vec3 get_random_dir() const override {
        if (random_double() < 0.5)
            return p_[0]->get_random_dir();
        else
            return p_[1]->get_random_dir();
    }

public:
    shared_ptr<Pdf> p_[2];
};

inline Vec3 random_to_sphere(double radius, double distance_squared) {
    auto r1 = random_double();
    auto r2 = random_double();
    auto z = 1 + r2*(sqrt(1-radius*radius/distance_squared) - 1);

    auto phi = 2*pi*r1;
    auto x = cos(phi)*sqrt(1-z*z);
    auto y = sin(phi)*sqrt(1-z*z);

    return Vec3(x, y, z);
}

class IsotropicPdf : public Pdf {
public:
    IsotropicPdf() {}

    virtual double get_pdf(const Vec3& direction) const override {
        return 0.25 / pi;
    }

    virtual Vec3 get_random_dir() const override {
        return random_in_unit_sphere();
    }
};