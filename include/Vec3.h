#pragma once

#include <cmath>
#include <iostream>
#include "utils/rt_utils.h"
#include "Sampler.h"

class Vec3 {
public:
    double e_[3];

public:
    // constructor
    Vec3() : e_{0, 0, 0} {}
    Vec3(double e0, double e1, double e2) : e_{e0, e1, e2} {}
    Vec3(double e) : e_{e, e, e} {}

    double x() const { return e_[0]; }
    double y() const { return e_[1]; }
    double z() const { return e_[2]; }

    Vec3 operator-() const { return Vec3(-e_[0], -e_[1], -e_[2]); }
    double operator[](int i) const { return e_[i]; }
    double& operator[](int i) { return e_[i]; }

    Vec3& operator+=(const Vec3 &v) {
        e_[0] += v.e_[0];
        e_[1] += v.e_[1];
        e_[2] += v.e_[2];
        return *this;
    }

    bool operator==(const Vec3 &v) const{
        return (e_[0] == v.e_[0]) &&  (e_[1] == v.e_[1]) &&  (e_[2] == v.e_[2]);
    }

    bool operator!=(const Vec3 &v) const{
        return (e_[0] != v.e_[0]) || (e_[1] != v.e_[1]) || (e_[2] != v.e_[2]);
    }

    Vec3& operator*=(const double t) {
        e_[0] *= t;
        e_[1] *= t;
        e_[2] *= t;
        return *this;
    }

    Vec3& operator*=(const Vec3 &v) {
        e_[0] *= v.e_[0];
        e_[1] *= v.e_[1];
        e_[2] *= v.e_[2];
        return *this;
    }

    Vec3& operator/=(const double t) {
        return *this *= 1/t;
    }

    double length() const {
        return sqrt(length_squared());
    }

    double length_squared() const {
        return e_[0] * e_[0] + e_[1] * e_[1] + e_[2] * e_[2];
    }

    inline static Vec3 random() {
        return Vec3(random_double(), random_double(), random_double());
    }

    inline static Vec3 random(double min, double max) {
        return Vec3(random_double(min,max), random_double(min,max), random_double(min,max));
    }

    bool near_zero() const {
        // Return true if the vector is close to zero in all dimensions.
        const auto s = 1e-8;
        return (fabs(e_[0]) < s) && (fabs(e_[1]) < s) && (fabs(e_[2]) < s);
    }
};

// Type aliases for Vec3
using Point3 = Vec3;   // 3D point
using Color = Vec3;    // RGB Color


// Vec3 Utility Functions
inline std::ostream& operator<<(std::ostream &out, const Vec3 &v) {
    return out << v.e_[0] << ' ' << v.e_[1] << ' ' << v.e_[2];
}

inline Vec3 operator+(const Vec3 &u, const Vec3 &v) {
    return Vec3(u.e_[0] + v.e_[0], u.e_[1] + v.e_[1], u.e_[2] + v.e_[2]);
}

inline Vec3 operator-(const Vec3 &u, const Vec3 &v) {
    return Vec3(u.e_[0] - v.e_[0], u.e_[1] - v.e_[1], u.e_[2] - v.e_[2]);
}

inline Vec3 operator*(const Vec3 &u, const Vec3 &v) {
    return Vec3(u.e_[0] * v.e_[0], u.e_[1] * v.e_[1], u.e_[2] * v.e_[2]);
}

inline Vec3 operator*(double t, const Vec3 &v) {
    return Vec3(t*v.e_[0], t * v.e_[1], t * v.e_[2]);
}

inline Vec3 operator*(const Vec3 &v, double t) {
    return t * v;
}

inline Vec3 operator/(Vec3 v, double t) {
    return (1/t) * v;
}

inline double dot(const Vec3 &u, const Vec3 &v) {
    return u.e_[0] * v.e_[0]
           + u.e_[1] * v.e_[1]
           + u.e_[2] * v.e_[2];
}

inline double abs_dot(const Vec3 &u, const Vec3 &v) {
    return fabs(u.e_[0] * v.e_[0]
           + u.e_[1] * v.e_[1]
           + u.e_[2] * v.e_[2]);
}

inline Vec3 cross(const Vec3 &u, const Vec3 &v) {
    return Vec3(u.e_[1] * v.e_[2] - u.e_[2] * v.e_[1],
                u.e_[2] * v.e_[0] - u.e_[0] * v.e_[2],
                u.e_[0] * v.e_[1] - u.e_[1] * v.e_[0]);
}

inline Vec3 unit_vector(Vec3 v) {
    return v / v.length();
}

Vec3 reflect(const Vec3& v, const Vec3& n);

Vec3 refract(const Vec3& uv, const Vec3& n, double etai_over_etat);

Vec3 random_in_unit_sphere();

Vec3 random_unit_vector();

Vec3 random_in_hemisphere(const Vec3& normal);

Vec3 random_in_unit_disk();