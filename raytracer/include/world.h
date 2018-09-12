#pragma once
#include "hitable.h"
#include "hitable_list.h"
#include "material.h"
#include "aabb.h"
#include <memory>
#include <deque>
#include <algorithm>

class world
{
public:
    world() = default;

    bool hit(const ray &r, hit_record &rec) const
    {
        return collection_.hit(r, 0.001f, 999999.f, rec);
    }

    material *create_lightsrc(vec3 light)
    {
        return create_material<lightsrc>(light);
    }
    material *create_lambertian(vec3 color)
    {
        return create_material<lambertian>(color);
    }
    material *create_metal(const vec3 &color, float f)
    {
        return create_material<metal>(color, f);
    }
    material *create_dielectric(float f)
    {
        return create_material<dielectric>(f);
    }

    void add_sphere(float radius, vec3 center, material *mat)
    {
        add_hitable<sphere>(center, radius, mat);
    }
    void add_aabb(vec3 min, vec3 max, material *mat)
    {
        add_hitable<aabb>(min, max, mat);
    }

private:
    template <typename TMat, typename... TArgs>
    material *create_material(TArgs &&... args)
    {
        materials_.push_back(std::make_unique<TMat>(std::forward<TArgs>(args)...));
        return materials_.back().get();
    }
    template<typename TObj, typename ...TArgs>
    void add_hitable(TArgs&& ...args)
    {
        objects_.push_back(std::make_unique<TObj>(std::forward<TArgs>(args)...));
        collection_.add_item(objects_.back().get());
    }

    hitable_list collection_;

    std::deque<std::unique_ptr<hitable>> objects_;
    std::deque<std::unique_ptr<material>> materials_;
};