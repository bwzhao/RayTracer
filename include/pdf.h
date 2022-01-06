#pragma once

class pdf {
public:
    virtual ~pdf() {}

    virtual double value(const vec3& direction) const = 0;
    virtual vec3 generate() const = 0;
};

inline vec3 random_cosine_direction() {
    auto r1 = random_double();
    auto r2 = random_double();
    auto z = sqrt(1-r2);

    auto phi = 2*pi*r1;
    auto x = cos(phi)*sqrt(r2);
    auto y = sin(phi)*sqrt(r2);

    return vec3(x, y, z);
}

class cosine_pdf : public pdf {
public:
    cosine_pdf(const vec3& w) { uvw.build_from_w(w); }

    virtual double value(const vec3& direction) const override {
        auto cosine = dot(unit_vector(direction), uvw.w());
        return (cosine <= 0) ? 0 : cosine/pi;
    }

    virtual vec3 generate() const override {
        return uvw.local(random_cosine_direction());
    }

public:
    onb uvw;
};

class hittable_pdf : public pdf {
public:
    hittable_pdf(shared_ptr<hittable> p, const point3& origin) : ptr_(p), o_(origin) {}

    virtual double value(const vec3& direction) const override {
        return ptr_->pdf_value(o_, direction);
    }

    virtual vec3 generate() const override {
        return ptr_->random(o_);
    }

public:
    point3 o_;
    shared_ptr<hittable> ptr_;
};

class mixture_pdf : public pdf {
public:
    mixture_pdf(shared_ptr<pdf> p0, shared_ptr<pdf> p1) {
        p_[0] = p0;
        p_[1] = p1;
    }

    virtual double value(const vec3& direction) const override {
        return 0.5 * p_[0]->value(direction) + 0.5 * p_[1]->value(direction);
    }

    virtual vec3 generate() const override {
        if (random_double() < 0.5)
            return p_[0]->generate();
        else
            return p_[1]->generate();
    }

public:
    shared_ptr<pdf> p_[2];
};

inline vec3 random_to_sphere(double radius, double distance_squared) {
    auto r1 = random_double();
    auto r2 = random_double();
    auto z = 1 + r2*(sqrt(1-radius*radius/distance_squared) - 1);

    auto phi = 2*pi*r1;
    auto x = cos(phi)*sqrt(1-z*z);
    auto y = sin(phi)*sqrt(1-z*z);

    return vec3(x, y, z);
}