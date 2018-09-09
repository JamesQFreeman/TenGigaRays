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
#include "material.h"



errno_t draw_png(const char filename[], unsigned char* img, unsigned w, unsigned h) {
	errno_t err;
	FILE *fp;
	err = fopen_s(&fp, filename, "wb");
	svpng(fp, w, h, img, 0);
	fclose(fp);
	return err;
}

void test_rgb(void) {
	auto p = new unsigned char[200 * 100 * 3];
	auto rgb = p;
	unsigned x, y;
	errno_t err;
	FILE *fp;
	err = fopen_s(&fp,"rgbtest.png", "wb");
	for (y = 0;y < 100;y++) 
		for (x = 0;x < 200;x++) {
			*p++ = (unsigned)x;
			*p++ = (unsigned)y;
			*p++ = 128;
		}
	svpng(fp, x, y, rgb, 0);
	fclose(fp);	
}



vec3 color(const ray &r, hitable *world_p, size_t depth) {
	hit_record rec;
	if (world_p->hit(r, 0.001, 99999.0, rec)) {
		ray scattered;
		vec3 attenation;
		if (depth < 50 && rec.mat_ptr->scatter(r,rec,attenation,scattered)) {
			return attenation * color(scattered, world_p, depth + 1);
		}
		else {
			return vec3(0, 0, 0);
		}

	}
	else {
		if (sphere(vec3(-1, 3, -1), 0.5, new metal(vec3(0.8, 0.8, 0.0), 0.5)).hit(r, 0.001, 99999.f, rec)) {
			return vec3(0.999, 0.999, 0.999);
		}
		else return vec3(0.1, 0.1, 0.1);
		//float t = 0.5*(r.unit_direction().y() + 1.0);
		//float t = 2*powf(0.5,float(depth))*(r.unit_direction().y() + 1.0);
		//return (1 - t)*vec3(1.0, 1.0, 1.0) + t * vec3(0.5, 0.7, 1.0);
	}

}

errno_t first_projection() {
	// Screen size and a screen buffers
	size_t w = 800;
	size_t h = 400;
	size_t SSAA = 64;
	auto p = new unsigned char[w * h * 3], rgb = p;
	hitable_list world, *world_p = &world;
	world.add_item(new sphere(vec3(0, -100.5, -1), 100, new lambertian(vec3(0.5,0.5,0.5))));
	world.add_item(new sphere(vec3(1, 0, -1), 0.5, new metal(vec3(0.8, 0.8, 0.0),0.4)));
	world.add_item(new sphere(vec3(0, 0.01, -1), 0.5, new lambertian(vec3(0.3, 0.8, 0.3))));
	camera cam;
	for (int j = h - 1; j >= 0; j--) {
		for (int i = 0; i < w; i++) {
			vec3 pixel(0, 0, 0);
			for (int s = 0; s < SSAA; s++) {
				float u = float(i + (rand_()+1.f)/2.f) / float(w);
				float v = float(j + (rand_()+1.f)/2.f) / float(h);
				ray r = cam.get_ray(u, v);
				pixel += color(r, world_p,0);
			}
			pixel /= float(SSAA);
			pixel = vec3(sqrt(pixel.r()), sqrt(pixel.g()), sqrt(pixel.b()));
			unsigned char rd = int(255.99*pixel.r());
			unsigned char gn = int(255.99*pixel.g());
			unsigned char bl = int(255.99*pixel.b());
			*p++ = rd;
			*p++ = gn;
			*p++ = bl;
			printf("%f %%\n", float(p - rgb) / float(w * h * 3) * 100);
		}
	}
	return draw_png("proj_test.png", rgb, w, h);
}

int main() {
	//generate_rand_list();
	//generate_rand_sphere_list();
	//test_rgb();
	first_projection();
	return 0;
}