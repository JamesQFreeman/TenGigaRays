#pragma once
#include "vec3.h"
class ray {
public:
    ray(const vec3 &o, const vec3 &d) {
        origin = o;
        direct = d;
    }
    vec3 origin_point() const { return origin; }
    vec3 direction() const { return direct; }
    vec3 unit_direction() const { return unit_vector(direct); }
    vec3 point_at_parameter(float t) const {
        return origin + t * direct;
    }
    ray(){};
    ~ray(){};
    vec3 origin;
    vec3 direct;
};
