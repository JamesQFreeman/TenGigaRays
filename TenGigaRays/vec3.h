#pragma once
#include <cmath>
#include <iostream>
class vec3 {
public:
    vec3(){};
    vec3(float e0, float e1, float e2) {
        e[0] = e0;
        e[1] = e1;
        e[2] = e2;
    }
    float r() const { return e[0]; }
    float g() const { return e[1]; }
    float b() const { return e[2]; }
    float x() const { return e[0]; }
    float y() const { return e[1]; }
    float z() const { return e[2]; }

    const vec3 &operator+() const { return *this; }
    vec3 operator-() const { return vec3(-e[0], -e[1], -e[2]); }
    float operator[](int i) const { return e[i]; }
    float &operator[](int i) { return e[i]; };

    vec3 &operator+=(const vec3 &v2);
    vec3 &operator-=(const vec3 &v2);
    vec3 &operator*=(const vec3 &v2);
    vec3 &operator/=(const vec3 &v2);
    vec3 &operator*=(const float c);
    vec3 &operator/=(const float c);

    float length() const {
        return sqrt(e[0] * e[0] + e[1] * e[1] + e[2] * e[2]);
    }
    float squared_length() const {
        return e[0] * e[0] + e[1] * e[1] + e[2] * e[2];
    }
    void make_unit_vector();
    ~vec3(){};
    float e[3];
};

inline std::istream &operator>>(std::istream &is, vec3 &v) {
    is >> v.e[0] >> v.e[1] >> v.e[2];
    return is;
}

inline std::ostream &operator<<(std::ostream &os, const vec3 &v) {
    os << v.e[0] << v.e[1] << v.e[2];
    return os;
}

inline void vec3::make_unit_vector() {
    float k = 1.0 / (e[0] * e[0] + e[1] * e[1] + e[2] * e[2]);
    e[0] /= k;
    e[1] /= k;
    e[2] /= k;
}

inline vec3 operator+(const vec3 &v1, const vec3 &v2) {
    return vec3(v1.e[0] + v2.e[0], v1.e[1] + v2.e[1], v1.e[2] + v2.e[2]);
}

inline vec3 operator-(const vec3 &v1, const vec3 &v2) {
    return vec3(v1.e[0] - v2.e[0], v1.e[1] - v2.e[1], v1.e[2] - v2.e[2]);
}

inline vec3 operator*(const vec3 &v1, const vec3 &v2) {
    return vec3(v1.e[0] * v2.e[0], v1.e[1] * v2.e[1], v1.e[2] * v2.e[2]);
}

inline vec3 operator/(const vec3 &v1, const vec3 &v2) {
    return vec3(v1.e[0] / v2.e[0], v1.e[1] / v2.e[1], v1.e[2] / v2.e[2]);
}

inline vec3 operator*(float t, const vec3 &v2) {
    return vec3(t * v2.e[0], t * v2.e[1], t * v2.e[2]);
}

inline vec3 operator*(const vec3 &v1, float t) {
    return vec3(v1.e[0] * t, v1.e[1] * t, v1.e[2] * t);
}

inline vec3 operator/(const vec3 &v1, float t) {
    return vec3(v1.e[0] / t, v1.e[1] / t, v1.e[2] / t);
}

inline float dot(const vec3 &v1, const vec3 &v2) {
    return v1.e[0] * v2.e[0] + v1.e[1] * v2.e[1] + v1.e[2] * v2.e[2];
}

inline vec3 cross(const vec3 &v0, const vec3 &v1) {
    return vec3(v0.e[1] * v1.e[2] - v0.e[2] * v1.e[1],
                v0.e[2] * v1.e[0] - v0.e[0] * v1.e[2],
                v0.e[0] * v1.e[1] - v0.e[1] * v1.e[0]);
}

inline vec3 &vec3::operator+=(const vec3 &v) {
    e[0] += v.e[0];
    e[1] += v.e[1];
    e[2] += v.e[2];
    return *this;
}

inline vec3 &vec3::operator-=(const vec3 &v) {
    e[0] -= v.e[0];
    e[1] -= v.e[1];
    e[2] -= v.e[2];
    return *this;
}

inline vec3 &vec3::operator*=(const vec3 &v) {
    e[0] *= v.e[0];
    e[1] *= v.e[1];
    e[2] *= v.e[2];
    return *this;
}

inline vec3 &vec3::operator/=(const vec3 &v) {
    e[0] /= v.e[0];
    e[1] /= v.e[1];
    e[2] /= v.e[2];
    return *this;
}

inline vec3 &vec3::operator*=(const float c) {
    e[0] *= c;
    e[1] *= c;
    e[2] *= c;
    return *this;
}

inline vec3 &vec3::operator/=(const float c) {
    e[0] /= c;
    e[1] /= c;
    e[2] /= c;
    return *this;
}

inline vec3 unit_vector(vec3 v) {
    return v / v.length();
}
