#pragma once
#include "hitable.h"
#include "material.h"

class aabb : public hitable
{
public:
    aabb(vec3 min, vec3 max, material *mat)
        : min_(min), max_(max), material_(mat)
    {
    }

    // TODO: invest why multiplying with invD does not work
    bool hit(const ray &r, float t_min, float t_max, hit_record &rec) const override
    {
        auto origin = r.origin();
        auto dir    = r.unit_direction();
        // auto invD   = 1.f / dir;

        auto t = std::numeric_limits<float>::max();
        vec3 p;
        vec3 n;

        auto t0 = (min_[0] - origin[0]) / dir[0];
        auto p0 = origin + t0 * dir;
        if (t0 > 0 && t0 < t && p0[1] >= min_[1] && p0[1] <= max_[1] && p0[2] >= min_[2] && p0[2] <= max_[2]) {
            t = t0;
            p = p0;
            n = vec3(-1, 0, 0);
        }
        auto t1 = (max_[0] - origin[0]) / dir[0];
        auto p1 = origin + t1 * dir;
        if (t1 > 0 && t1 < t && p1[1] >= min_[1] && p1[1] <= max_[1] && p1[2] >= min_[2] && p1[2] <= max_[2]) {
            t = t1;
            p = p1;
            n = vec3(1, 0, 0);
        }
        auto t2 = (min_[1] - origin[1]) / dir[1];
        auto p2 = origin + t2 * dir;
        if (t2 > 0 && t2 < t && p2[0] >= min_[0] && p2[0] <= max_[0] && p2[2] >= min_[2] && p2[2] <= max_[2]) {
            t = t2;
            p = p2;
            n = vec3(0, -1, 0);
        }
        auto t3 = (max_[1] - origin[1]) / dir[1];
        auto p3 = origin + t3 * dir;
        if (t3 > 0 && t3 < t && p3[0] >= min_[0] && p3[0] <= max_[0] && p3[2] >= min_[2] && p3[2] <= max_[2]) {
            t = t3;
            p = p3;
            n = vec3(0, 1, 0);
        }
        auto t4 = (min_[2] - origin[2]) / dir[2];
        auto p4 = origin + t4 * dir;
        if (t4 > 0 && t4 < t && p4[0] >= min_[0] && p4[0] <= max_[0] && p4[1] >= min_[1] && p4[1] <= max_[1]) {
            t = t4;
            p = p4;
            n = vec3(0, 0, -1);
        }
        auto t5 = (max_[2] - origin[2]) / dir[2];
        auto p5 = origin + t5 * dir;
        if (t5 > 0 && t5 < t && p5[0] >= min_[0] && p5[0] <= max_[0] && p5[1] >= min_[1] && p5[1] <= max_[1]) {
            t = t5;
            p = p5;
            n = vec3(0, 0, 1);
        }

        if (t < t_min || t >= t_max)
            return false;

        bool origin_outside =
            origin[0] < min_[0] || origin[0] > max_[0] ||
            origin[1] < min_[1] || origin[1] > max_[1] ||
            origin[2] < min_[2] || origin[2] > max_[2];

        rec.t       = t;
        rec.p       = origin + t * dir;
        rec.normal  = origin_outside ? n : -n;
        rec.mat_ptr = material_;
        return true;
    }

private:
    vec3 min_;
    vec3 max_;
    material *material_;
};
