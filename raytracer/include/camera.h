#pragma once
#include "ray.h"
class camera
{
public:
    camera(vec3 origin, vec3 lookat, vec3 lookup, float width, float height,
           float focus)
        : origin(origin), lookat(unit_vector(lookat)),
          lookup(unit_vector(lookup)), width(width), height(height),
          focus(focus)
    {
        vp_center = focus * lookat;
        vp_left   = cross(lookat, lookup) * width;
        vp_up     = unit_vector(lookup) * height;
    }

    ray get_ray(float u, float v)
    {
        return ray(origin, vp_center + (u - 0.5) * vp_left + (v - 0.5) * vp_up);
    }

    vec3 origin;
    vec3 lookat;
    vec3 lookup;
    float width;
    float height;
    float focus;

    vec3 vp_center;
    vec3 vp_left;
    vec3 vp_up;
};
