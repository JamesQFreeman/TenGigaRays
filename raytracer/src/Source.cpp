#include <iostream>
#include <fstream>
#include "svpng.inc"
#include "hitable_list.h"
#include "sphere.h"
#include "camera.h"
#include <cstdlib>
#include <random>
#include <cmath>
#include <algorithm>
#include <iterator>
#include <thread>
#include <future>
#include <memory>
#include "material.h"

void draw_png(const char *filename, const uint8_t *img, int w, int h) {
    FILE *fp = fopen(filename, "wb");
    svpng(fp, w, h, img, 0);
    fclose(fp);
}

void test_rgb() {
    auto p   = new unsigned char[200 * 100 * 3];
    auto rgb = p;
    unsigned x, y;
    FILE *fp = fopen("rgbtest.png", "wb");
    for (y = 0; y < 100; y++)
        for (x = 0; x < 200; x++) {
            *p++ = (unsigned)x;
            *p++ = (unsigned)y;
            *p++ = 128;
        }
    svpng(fp, x, y, rgb, 0);
    fclose(fp);
}

vec3 color(const ray &r, hitable *world_p, size_t depth) {
    if (depth > 20)
        return vec3(0, 0, 0);

    hit_record rec;
    if (world_p->hit(r, 0.001, 99999.0, rec)) {
        vec3 light;
        if (rec.mat_ptr->emit(r, rec, light))
            return light;

        ray scattered;
        vec3 attenation;
        if (rec.mat_ptr->scatter(r, rec, attenation, scattered))
            return attenation * color(scattered, world_p, depth + 1);
    }

    return vec3(0.2, 0.2, 0.2);
}

void render_sample(float *workspace, hitable *world, camera *cam, int w, int h) {
    auto p = workspace;
    for (int j = h - 1; j >= 0; j--) {
        for (int i = 0; i < w; i++) {
            float u = float(i + (rand_mirror() + 1.f) / 2.f) / float(w);
            float v = float(j + (rand_mirror() + 1.f) / 2.f) / float(h);

            auto ray   = cam->get_ray(u, v);
            auto pixel = color(ray, world, 0);

            *p++ = pixel.r();
            *p++ = pixel.g();
            *p++ = pixel.b();
        }
    }
}

void draw_canvas(uint8_t *canvas, const float *img, int ssaa, float gamma, int w, int h) {
    for (int i = 0; i < w * h * 3; ++i) {
        canvas[i] = std::min(255, static_cast<int>(255.f * powf(img[i] / ssaa, 1 / gamma)));
    }
}

hitable *create_world() {
    auto world = new hitable_list;

    // box
    world->add_item(new sphere(vec3(0, 1e5 + 10, 0), 1e5, new lightsrc(vec3(3, 3, 3))));            // top
    world->add_item(new sphere(vec3(0, -1e5 - 0.5, -1), 1e5, new lambertian(vec3(0.8, 0.8, 0.8)))); // bottom
    world->add_item(new sphere(vec3(0, 0, -1e5 - 5), 1e5, new lambertian(vec3(0.2, 0.8, 0.2))));    // front
    world->add_item(new sphere(vec3(0, 0, 1e5 + 5), 1e5, new lambertian(vec3(0.8, 0.8, 0.8))));     // back
    world->add_item(new sphere(vec3(-1e5 - 3, 0, 0), 1e5, new lambertian(vec3(0.8, 0.2, 0.2))));    // left
    world->add_item(new sphere(vec3(1e5 + 3, 0, 0), 1e5, new lambertian(vec3(0.2, 0.2, 0.8))));     // right

    // balls
    world->add_item(new sphere(vec3(1, 0, -1), 0.5, new metal(vec3(0.8, 0.3, 0.8), 0.8)));
    world->add_item(new sphere(vec3(0, 0., -1), 0.5, new lambertian(vec3(0.7, 0.3, 0.5))));
    world->add_item(new sphere(vec3(-1, 0, -1), 0.5, new metal(vec3(0.8, 0.8, 0.0), 0.0)));

    return world;
}

void first_projection() {
    // Screen size and a screen buffers
    constexpr int w    = 200;
    constexpr int h    = 100;
    constexpr int SSAA = 100;
    constexpr int thd  = 4;
    static_assert(SSAA % thd == 0, "jobs must be evenly sliced!");

    auto canvas = new unsigned char[w * h * 3];
    auto cam    = new camera(vec3(0, 0, 3), vec3(0, 0, -1), vec3(0, 1, 0), 4, 2, 3);
    auto world  = create_world();

    std::vector<std::future<float *>> future_vec;
    for (int i = 0; i < thd; ++i) {
        auto future = std::async(std::launch::async, [=]() {
            auto buffer    = new float[w * h * 3]{};
            auto workspace = new float[w * h * 3];

            for (int s = 0; s < SSAA / thd; ++s) {
                printf("[worker %d] %d/%d\n", i, s, SSAA / thd);

                render_sample(workspace, world, cam, w, h);
                for (int j = 0; j < w * h * 3; ++j) {
                    buffer[j] += workspace[j];
                }
            }

            return buffer;
        });

        future_vec.push_back(std::move(future));
    }

    auto result = new float[w * h * 3]{};
    for (auto &future : future_vec) {
        future.wait();

        auto buffer = future.get();
        for (int i = 0; i < w * h * 3; ++i) {
            result[i] += buffer[i];
        }
    }

    draw_canvas(canvas, result, SSAA, 2, w, h);
    draw_png("result.png", canvas, w, h);
}

int main() {
    first_projection();
    return 0;
}