/* Copyright 2019 Sebastian Achim Mueller */
#ifndef MERLICT_MLISURFACES_H_
#define MERLICT_MLISURFACES_H_

#include <stdint.h>

typedef struct {
    uint32_t inner;
    uint32_t outer;
} mliSurfaces;

int mliSurfaces_is_equal(const mliSurfaces* a, const mliSurfaces *b) {
    if (a->inner != b->inner) return 0;
    if (a->outer != b->outer) return 0;
    return 1;}

#endif
