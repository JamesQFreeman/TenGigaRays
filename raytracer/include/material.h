#pragma once
#include "ray.h"
#include "hitable.h"
#include <random>
float rand_() {
    static thread_local auto rnd  = std::mt19937();
    static thread_local auto dist = std::uniform_real_distribution<float>(-1.f, 1.f);
    return dist(rnd);
}

vec3 random_in_unit_sphere() {
    vec3 p;
    do {
        p = vec3(rand_(), rand_(), rand_());
    } while (dot(p, p) >= 1.0);
    return p;
}

vec3 reflect(const vec3 &in, const vec3 &normal) {
    return in - 2 * dot(in, normal) * normal;
}

class material {
public:
    material(){};
    ~material(){};
	virtual bool emit(const ray& r, const hit_record& rec, vec3& color) const
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
	bool emit(const ray& r, const hit_record& rec, vec3& light) const override
	{
        light = light_;
        return true;
	}

private:
    vec3 light_;
};

class lambertian : public material {
public:
    lambertian(const vec3 &a)
        : albedo(a) {}
    virtual bool scatter(const ray &r, const hit_record &rec,
                         vec3 &attenuation, ray &scattered) const {
        auto normal = rec.normal;
        scattered   = ray(rec.p, normal + random_in_unit_sphere());
        attenuation = albedo;
        return true;
    }
    vec3 albedo;
};

class metal : public material {
public:
    metal(const vec3 &a, float f)
        : albedo(a) {
        if (f < 1)
            fuzz = f;
        else
            fuzz = 1;
    }
    virtual bool scatter(const ray &r, const hit_record &rec,
                         vec3 &attenuation, ray &scattered) const {
        auto normal = rec.normal;
        scattered   = ray(rec.p, reflect(r.direction(), rec.normal) +
                                   fuzz * random_in_unit_sphere());
        attenuation = albedo;
        return (dot(scattered.direction(), rec.normal)) > 0;
    }
    float fuzz;
    vec3 albedo;
};
