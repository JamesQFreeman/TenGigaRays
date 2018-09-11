#pragma once
#include "ray.h"
#include "hitable.h"

vec3 random_in_unit_sphere()
{
    vec3 p;
    do {
        p = vec3(rand_mirror(), rand_mirror(), rand_mirror());
    } while (dot(p, p) >= 1.0);
    return p;
}

float schlick(float cosine, float ref_idx)
{
    float r0 = (1 - ref_idx) / (1 + ref_idx);
    r0       = r0 * r0;
    return r0 + (1 - r0) * pow((1 - cosine), 5);
}

bool refract(const vec3 &ray_in, const vec3 &normal, float &k, vec3 refracted)
{
    vec3 unit_in       = ray_in.unit_vector();
    float dt           = dot(unit_in, normal);
    float discriminant = 1.f - k * k * (1 - dt * dt);
    if (discriminant > 0) {
        refracted = k * (unit_in - normal * dt) - normal * sqrt(discriminant);
        return true;
    } else {
        return false;
    }
}

vec3 reflect(const vec3 &in, const vec3 &normal)
{
    return in - 2 * dot(in, normal) * normal;
}

class material
{
public:
    material(){};
    ~material(){};
    virtual bool emit(const ray &r, const hit_record &rec, vec3 &color) const
    {
        return false;
    }

    virtual bool scatter(const ray &r, const hit_record &rec,
                         vec3 &attenuation, ray &scattered) const
    {
        return false;
    }
};

class lightsrc : public material
{
public:
    lightsrc(const vec3 &light)
        : light_(light) {}
    bool emit(const ray &r, const hit_record &rec, vec3 &light) const override
    {
        light = light_;
        return true;
    }

private:
    vec3 light_;
};

class lambertian : public material
{
public:
    lambertian(const vec3 &a)
        : albedo(a) {}
    virtual bool scatter(const ray &r, const hit_record &rec,
                         vec3 &attenuation, ray &scattered) const
    {
        auto normal = rec.normal;
        scattered   = ray(rec.p, normal + random_in_unit_sphere());
        attenuation = albedo;
        return true;
    }
    vec3 albedo;
};

class metal : public material
{
public:
    metal(const vec3 &a, float f)
        : albedo(a)
    {
        if (f < 1)
            fuzz = f;
        else
            fuzz = 1;
    }
    virtual bool scatter(const ray &r, const hit_record &rec,
                         vec3 &attenuation, ray &scattered) const
    {
        auto normal = rec.normal;
        scattered   = ray(rec.p, reflect(r.direction(), rec.normal) +
                                   fuzz * random_in_unit_sphere());
        attenuation = albedo;
        return (dot(scattered.direction(), rec.normal)) > 0;
    }
    float fuzz;
    vec3 albedo;
};

class dielectric : public material
{
public:
    dielectric(float ri)
        : ref_idx(ri) {}
    virtual bool scatter(const ray &r_in, const hit_record &rec, vec3 &attenuation, ray &scattered) const
    {
        vec3 outward_normal;
        vec3 reflected = reflect(r_in.direction(), rec.normal);
        float ni_over_nt;
        attenuation = vec3(1.0, 1.0, 1.0);
        vec3 refracted;
        float reflect_prob;
        float cos;
        if (dot(r_in.direction(), rec.normal) > 0) {
            outward_normal = -rec.normal;
            ni_over_nt     = ref_idx;
            cos            = dot(r_in.direction(), rec.normal) / r_in.direction().length();
            cos            = sqrt(1 - ref_idx * ref_idx * (1 - cos * cos));
        } else {
            outward_normal = rec.normal;
            ni_over_nt     = 1.0 / ref_idx;
            cos            = -dot(r_in.direction(), rec.normal) / r_in.direction().length();
        }
        if (refract(r_in.direction(), outward_normal, ni_over_nt, refracted))
            reflect_prob = schlick(cos, ref_idx);
        else
            reflect_prob = 1.0;
        if (rand_mirror() + 1 < reflect_prob * 2)
            scattered = ray(rec.p, reflected);
        else
            scattered = ray(rec.p, refracted);
        return true;
    }

    float ref_idx;
};