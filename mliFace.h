/* Copyright 2019 Sebastian Achim Mueller */
#ifndef MERLICT_MLIFACE_H_
#define MERLICT_MLIFACE_H_

#include <stdint.h>

struct mliFace{
    uint32_t a;
    uint32_t b;
    uint32_t c;
};

struct mliFace mliFace_set(
    const uint32_t a,
    const uint32_t b,
    const uint32_t c) {
    struct mliFace face;
    face.a = a;
    face.b = b;
    face.c = c;
    return face;}

int mliFace_is_equal (const struct mliFace a, const struct mliFace b) {
    if (a.a != b.a) return 0;
    if (a.b != b.b) return 0;
    if (a.c != b.c) return 0;
    return 1;}

void mliFace_ncpy(
    const struct mliFace *from,
    struct mliFace *to,
    const uint64_t num) {
    uint64_t i;
    for (i = 0; i < num; i++) {
        to[i] = from[i];}}

#endif
