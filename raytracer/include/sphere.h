#pragma once
#include "hitable.h"
#include "material.h"
class sphere : public hitable
{
public:
    sphere(vec3 center, float radius, material *m)
        : center_(center), radius_(radius), material_(m){};

    virtual bool hit(const ray &r, float t_min, float t_max, hit_record &rec) const;

private:
    vec3 center_;
    float radius_;
    material *material_;
};

bool sphere::hit(const ray &ray, float t_min, float t_max, hit_record &rec) const
{
    // ray origin
    vec3 ray_o = ray.origin();
    // ray direction
    vec3 ray_d = ray.unit_direction();
    // ray origin to sphere center
    vec3 l = center_ - ray_o;

    // length of l's project onto ray's direction
    // NOTE length of the projection decrease as ray points away from center of the sphere
    float s    = dot(l, ray_d);
    float l_sq = dot(l, l);
    float r_sq = radius_ * radius_;
    if (s * s < l_sq - r_sq || l_sq < r_sq)
        return false; // ray outside does not points to sphere

    float m_sq = l_sq - s * s;
    float q    = sqrtf(r_sq - m_sq);
    float t    = l_sq > r_sq ? s - q : s + q;

    if (t < t_min || t > t_max)
        return false;

    rec.t       = t;
    rec.p       = ray.origin() + rec.t * ray.unit_direction();
    rec.normal  = (rec.p - center_) / radius_;
    rec.mat_ptr = this->material_;
    return true;
}
