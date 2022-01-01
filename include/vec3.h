#pragma once

#include <cmath>
#include <iostream>

class vec3 {
public:
    double e_[3];

public:
    // constructor
    vec3() : e_{0, 0, 0} {}
    vec3(double e0, double e1, double e2) : e_{e0, e1, e2} {}

    double x() const { return e_[0]; }
    double y() const { return e_[1]; }
    double z() const { return e_[2]; }

    vec3 operator-() const { return vec3(-e_[0], -e_[1], -e_[2]); }
    double operator[](int i) const { return e_[i]; }
    double& operator[](int i) { return e_[i]; }

    vec3& operator+=(const vec3 &v) {
        e_[0] += v.e_[0];
        e_[1] += v.e_[1];
        e_[2] += v.e_[2];
        return *this;
    }

    vec3& operator*=(const double t) {
        e_[0] *= t;
        e_[1] *= t;
        e_[2] *= t;
        return *this;
    }

    vec3& operator/=(const double t) {
        return *this *= 1/t;
    }

    double length() const {
        return sqrt(length_squared());
    }

    double length_squared() const {
        return e_[0] * e_[0] + e_[1] * e_[1] + e_[2] * e_[2];
    }
};

// Type aliases for vec3
using point3 = vec3;   // 3D point
using color = vec3;    // RGB color


// vec3 Utility Functions
inline std::ostream& operator<<(std::ostream &out, const vec3 &v) {
    return out << v.e_[0] << ' ' << v.e_[1] << ' ' << v.e_[2];
}

inline vec3 operator+(const vec3 &u, const vec3 &v) {
    return vec3(u.e_[0] + v.e_[0], u.e_[1] + v.e_[1], u.e_[2] + v.e_[2]);
}

inline vec3 operator-(const vec3 &u, const vec3 &v) {
    return vec3(u.e_[0] - v.e_[0], u.e_[1] - v.e_[1], u.e_[2] - v.e_[2]);
}

inline vec3 operator*(const vec3 &u, const vec3 &v) {
    return vec3(u.e_[0] * v.e_[0], u.e_[1] * v.e_[1], u.e_[2] * v.e_[2]);
}

inline vec3 operator*(double t, const vec3 &v) {
    return vec3(t*v.e_[0], t * v.e_[1], t * v.e_[2]);
}

inline vec3 operator*(const vec3 &v, double t) {
    return t * v;
}

inline vec3 operator/(vec3 v, double t) {
    return (1/t) * v;
}

inline double dot(const vec3 &u, const vec3 &v) {
    return u.e_[0] * v.e_[0]
           + u.e_[1] * v.e_[1]
           + u.e_[2] * v.e_[2];
}

inline vec3 cross(const vec3 &u, const vec3 &v) {
    return vec3(u.e_[1] * v.e_[2] - u.e_[2] * v.e_[1],
                u.e_[2] * v.e_[0] - u.e_[0] * v.e_[2],
                u.e_[0] * v.e_[1] - u.e_[1] * v.e_[0]);
}

inline vec3 unit_vector(vec3 v) {
    return v / v.length();
}