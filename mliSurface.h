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

#endif
