#pragma once
#include "ray.h"
class camera {
public:
    camera() {
        lower_left_corner = vec3(-2, -1, -1);
        width             = vec3(4, 0, 0);
        height            = vec3(0, 2, 0);
        origin            = vec3(0, 0, 2);
    };

    ray get_ray(float u, float v) {
        vec3 relative_point = lower_left_corner +
                              u * width + v * height;
        return ray(origin, relative_point);
    }

    vec3 origin;
    vec3 lower_left_corner;
    vec3 width;
    vec3 height;

    ~camera(){};
};
