/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MERLICT_C89_MLI_LAMBERTIAN_COSINE_LAW_H_
#define MERLICT_C89_MLI_LAMBERTIAN_COSINE_LAW_H_

#include "mliVec.h"

struct mliVec mli_draw_lambertian_direction_wrt_surface_normal(
        struct mliMT19937 *prng,
        const struct mliVec surface_normal);
struct mliVec mli_draw_lambertian_direction_wrt_z(struct mliMT19937 *prng);
#endif
