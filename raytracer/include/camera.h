#pragma once
#include "ray.h"
class camera
{
public:
    camera(vec3 lookfrom, vec3 lookat, vec3 lookup, float width, float height,
           float focus)
        : origin_(lookfrom), lookat_(lookat),
          lookup_(lookup), width_(width), height_(height),
          focus_(focus)
    {
        vp_center = focus * (lookat - lookfrom).unit_vector();
        vp_left   = width * cross(lookat, lookup.unit_vector());
        vp_up     = height * lookup.unit_vector();
    }

    ray get_ray(float u, float v)
    {
        return ray(origin_, vp_center + (u - 0.5) * vp_left + (v - 0.5) * vp_up);
    }

    vec3 origin_;
    vec3 lookat_;
    vec3 lookup_;
    float width_;
    float height_;
    float focus_;

    vec3 vp_center;
    vec3 vp_left;
    vec3 vp_up;
};
