#pragma once
#include "hitable.h"
#include "material.h"
class sphere : public hitable
{
public:
    sphere() {}
    ~sphere() {}
    sphere(vec3 c, float r, material* m_ptr)
        : center(c), radius(r), mat_ptr(m_ptr){};
    virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const;
    vec3 center;
    float radius;
    material* mat_ptr;
};

bool sphere::hit(const ray& r, float t_min, float t_max, hit_record& rec) const
{
    vec3 ray_o = r.origin_point();
    vec3 ray_d = r.unit_direction();
    vec3 l     = center - ray_o;
    float s    = dot(l, ray_d);
    float l_sq = dot(l, l);
    float r_sq = radius * radius;
    if (s < 0 && l_sq > r_sq) return false;
    float m_sq = l_sq - s * s;
    if (m_sq > r_sq) return false;
    float q = sqrt(r_sq - m_sq);
    if (l_sq > r_sq)
        rec.t = s - q;
    else
        rec.t = s + q;
    rec.p       = r.origin_point() + rec.t * r.unit_direction();
    rec.normal  = (rec.p - center) / radius;
    rec.mat_ptr = this->mat_ptr;
    return true;
}
