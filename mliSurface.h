/* Copyright 2019 Sebastian Achim Mueller */
#ifndef MERLICT_MLISURFACE_H_
#define MERLICT_MLISURFACE_H_

#include <stdint.h>

typedef struct {
    uint32_t color;
    uint32_t reflection;
    uint32_t refraction;
    uint32_t absorbtion;
} mliSurface;

int mliSurface_is_equal(const mliSurface a, const mliSurface b) {
    if (a.color != b.color) return 0;
    if (a.reflection != b.reflection) return 0;
    if (a.refraction != b.refraction) return 0;
    if (a.absorbtion != b.absorbtion) return 0;
    return 1;}

#endif
