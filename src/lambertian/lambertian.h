/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLI_LAMBERTIAN_H_
#define MLI_LAMBERTIAN_H_

#include "../vec/vec.h"
#include "../prng/prng.h"

/*
 * Lambertian cosine law
 */

struct mli_Vec mli_lambertian_cosine_law_draw_direction_wrt_surface_normal(
        struct mli_Prng *prng,
        const struct mli_Vec surface_normal);
struct mli_Vec mli_lambertian_cosine_law_draw_direction_wrt_z(
        struct mli_Prng *prng);
#endif
