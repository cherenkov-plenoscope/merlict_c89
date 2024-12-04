/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLI_LAMBERTIAN_COSINE_LAW_H_
#define MLI_LAMBERTIAN_COSINE_LAW_H_

#include "mliVec.h"
#include "mli_random.h"

struct mliVec mli_draw_lambertian_direction_wrt_surface_normal(
        struct mtl_Prng *prng,
        const struct mliVec surface_normal);
struct mliVec mli_draw_lambertian_direction_wrt_z(struct mtl_Prng *prng);
#endif
