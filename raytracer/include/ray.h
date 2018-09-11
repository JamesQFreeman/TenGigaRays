#pragma once
#include "math_utils.h"
class ray
{
public:
    ray() {}
    ray(const vec3 &origin, const vec3 &direction)
    {
        origin_    = origin;
        direction_ = direction;
    }
    vec3 origin() const { return origin_; }
    vec3 direction() const { return direction_; }
    vec3 unit_direction() const { return direction_.unit_vector(); }
    vec3 point_at_parameter(float t) const
    {
        return origin_ + t * direction_;
    }

    vec3 origin_;
    vec3 direction_;
};
