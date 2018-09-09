#include <iostream>
#include <fstream>
#include "svpng.inc"
#include "hitable_list.h"
#include "sphere.h"
#include "camera.h"
#include "pre_rand.h"
#include <cstdlib>
#include <random>
#include <cmath>
#include <algorithm>
#include <iterator>
#include <thread>
#include <future>
#include <memory>
#include "material.h"

void draw_png(const char* filename, const uint8_t* img, int w, int h)
{
    FILE* fp;
    fopen_s(&fp, filename, "wb");
    svpng(fp, w, h, img, 0);
    fclose(fp);
}

void test_rgb()
{
    auto p   = new unsigned char[200 * 100 * 3];
    auto rgb = p;
    unsigned x, y;
    FILE* fp;
    fopen_s(&fp, "rgbtest.png", "wb");
    for (y = 0; y < 100; y++)
        for (x = 0; x < 200; x++) {
            *p++ = (unsigned)x;
            *p++ = (unsigned)y;
            *p++ = 128;
        }
    svpng(fp, x, y, rgb, 0);
    fclose(fp);
}

vec3 color(const ray& r, hitable* world_p, size_t depth)
{
    hit_record rec;
    if (world_p->hit(r, 0.001, 99999.0, rec)) {
        ray scattered;
        vec3 attenation;
        if (depth < 50 && rec.mat_ptr->scatter(r, rec, attenation, scattered)) {
            return attenation * color(scattered, world_p, depth + 1);
        }
        else
        {
            return vec3(0, 0, 0);
        }
    }
    else
    {
        auto light_src_material = metal(vec3(0.8, 0.8, 0.0), 0.5);
        if (sphere(vec3(-1, 10., -1), 0.5, &light_src_material).hit(r, 0.001, 99999.f, rec)) {
            return vec3(10, 10, 10);
        }
        else
        {
            float t = 0.5 * (r.unit_direction().y() + 1.0);
            return (1 - t) * vec3(1.0, 1.0, 1.0) + t * vec3(0.5, 0.7, 1.0);
        }

        //float t = 0.5*(r.unit_direction().y() + 1.0);
        //float t = 2*powf(0.5,float(depth))*(r.unit_direction().y() + 1.0);
        //return (1 - t) * vec3(1.0, 1.0, 1.0) + t * vec3(0.5, 0.7, 1.0);
    }
}

void render_sample(float* workspace, hitable_list* world, camera* cam, int w, int h)
{
    auto p = workspace;
    for (int j = h - 1; j >= 0; j--) {
        for (int i = 0; i < w; i++) {
            float u = float(i + (rand_() + 1.f) / 2.f) / float(w);
            float v = float(j + (rand_() + 1.f) / 2.f) / float(h);

            auto ray   = cam->get_ray(u, v);
            auto pixel = color(ray, world, 0);

            *p++ = pixel.r();
            *p++ = pixel.g();
            *p++ = pixel.b();
        }
    }
}

void draw_canvas(uint8_t* canvas, const float* img, int ssaa, float gamma, int w, int h)
{
    for (int i = 0; i < w * h * 3; ++i)
    {
        canvas[i] = std::min(255, static_cast<int>(255.f * powf(img[i] / ssaa, 1 / gamma)));
    }
}

void first_projection()
{
    // Screen size and a screen buffers
    constexpr int w    = 800;
    constexpr int h    = 400;
    constexpr int SSAA = 200;
    constexpr int thd  = 4;
    static_assert(SSAA % thd == 0, "jobs must be evenly sliced!");

    auto canvas = new unsigned char[w * h * 3];
    auto cam    = new camera;
    auto world  = new hitable_list;
    world->add_item(new sphere(vec3(0, -100.5, -1), 100, new lambertian(vec3(0.5, 0.5, 0.5))));
    world->add_item(new sphere(vec3(1, 0, -1), 0.5, new metal(vec3(0.8, 0.8, 0.0), 0.4)));
    world->add_item(new sphere(vec3(0, 0., -1), 0.5, new lambertian(vec3(0.3, 0.8, 0.3))));

    std::vector<std::future<float*>> future_vec;
    for (int i = 0; i < thd; ++i)
    {
        auto future = std::async(std::launch::async, [=]() {
            auto buffer    = new float[w * h * 3]{};
            auto workspace = new float[w * h * 3];
			
			for (int s = 0; s < SSAA / thd; ++s)
            {
                render_sample(workspace, world, cam, w, h);
                for (int j = 0; j < w * h * 3; ++j)
                {
                    buffer[j] += workspace[j];
                }

				printf("[worker %d] %d/%d\n", i, s, SSAA/thd);
            }

			return buffer;
        });

        future_vec.push_back(std::move(future));
    }

    auto result = new float[w * h * 3]{};
    for (auto& future : future_vec)
    {
        future.wait();
        
		auto buffer = future.get();
		for (int i = 0; i < w * h * 3; ++i)
        {
            result[i] += buffer[i];
        }
    }

    draw_canvas(canvas, result, SSAA, 2, w, h);
    draw_png("result.png", canvas, w, h);

    //printf("%f %%\n", static_cast<float>(s) / static_cast<float>(SSAA) * 100.f);
}

int main()
{
    //generate_rand_list();
    //generate_rand_sphere_list();
    //test_rgb();
    first_projection();
    return 0;
}