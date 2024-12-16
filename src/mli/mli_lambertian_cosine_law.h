/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLI_LAMBERTIAN_COSINE_LAW_H_
#define MLI_LAMBERTIAN_COSINE_LAW_H_

#include "../vec/vec.h"
#include "mli_random.h"

struct mli_Vec mli_draw_lambertian_direction_wrt_surface_normal(
        struct mli_Prng *prng,
        const struct mli_Vec surface_normal);
struct mli_Vec mli_draw_lambertian_direction_wrt_z(struct mli_Prng *prng);
#endif
