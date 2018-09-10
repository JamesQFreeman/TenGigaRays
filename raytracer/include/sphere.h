#pragma once
#include "hitable.h"
#include "material.h"
class sphere : public hitable {
public:
    sphere(vec3 c, float r, material *m_ptr)
        : center(c), radius(r), mat_ptr(m_ptr){};

    virtual bool hit(const ray &r, float t_min, float t_max, hit_record &rec) const;
    vec3 center;
    float radius;
    material *mat_ptr;
};

bool sphere::hit(const ray &ray, float t_min, float t_max, hit_record &rec) const {
    // ray origin
    vec3 ray_o = ray.origin_point();
    // ray direction
    vec3 ray_d = ray.unit_direction();
    // ray origin to sphere center
    vec3 l = center - ray_o;

    // length of l's project onto ray's direction
    // NOTE length of the projection decrease as ray points away from center of the sphere
    float s    = dot(l, ray_d);
    float l_sq = dot(l, l);
    float r_sq = radius * radius;
    if (s * s < l_sq - r_sq)
        return false; // ray does not points to sphere

    float m_sq = l_sq - s * s;
    float q    = sqrtf(r_sq - m_sq);
    float t   = l_sq > r_sq ? s - q : s + q;
    
	if (t < t_min || t > t_max)
        return false;

    rec.t       = t;
    rec.p       = ray.origin_point() + rec.t * ray.unit_direction();
    rec.normal  = (rec.p - center) / radius;
    rec.mat_ptr = this->mat_ptr;
    return true;
}
