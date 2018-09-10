#pragma once
#include "vec3.h"
class pre_rand {
public:
    pre_rand() {
        generate_rand_01();
        generate_rand_11();
        generate_rand_sphere_list();
    };
    ~pre_rand(){};

    float rand11() {
        // random float from -1 to 1
        if (i > 9999) i = 0;
        return rand_11[i++];
    }

    float rand01() {
        // random float from 0 to 1
        if (k > 9999) k = 0;
        return rand_01[k++];
    }

    vec3 rand_sphere() {
        // random float from 0 to 1
        if (j > 9999) j = 0;
        return rand_in_unit_sphere[j++];
    }

private:
    float rand_() {
        return (-1 + (float(rand()) / (float)RAND_MAX) * 2);
    }

    void generate_rand_11() {
        for (int i = 0; i < 10000; i++) {
            rand_11[i] = rand_();
        }
    }

    void generate_rand_01() {
        for (int i = 0; i < 10000; i++) {
            rand_01[i] = (rand_() + 1.f) / 2;
        }
    }

    void generate_rand_sphere_list() {
        vec3 p;
        for (int i = 0; i < 10000; i++) {
            do {
                p = vec3(rand_(), rand_(), rand_());
            } while (dot(p, p) >= 1.0);
            rand_in_unit_sphere[i] = p;
        }
    }
    int i = 0;
    int j = 0;
    int k = 0;
    vec3 rand_in_unit_sphere[10000];
    float rand_01[10000];
    float rand_11[10000];
};