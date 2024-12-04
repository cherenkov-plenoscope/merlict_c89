/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLI_RANDOM_H_
#define MLI_RANDOM_H_

#include "../../mtl/src/prng.h"
#include "mliVec.h"

struct mliVec mli_random_position_on_disc(
        const double radius,
        struct mtl_Prng *prng);
struct mliVec mli_random_draw_direction_in_zenith_azimuth_range(
        const struct mtl_prng_ZenithRange zenith,
        const struct mtl_prng_UniformRange azimuth,
        struct mtl_Prng *prng);
struct mliVec mli_random_position_inside_unit_sphere(struct mtl_Prng *prng);
#endif
