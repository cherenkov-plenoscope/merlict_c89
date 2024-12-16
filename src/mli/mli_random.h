/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLI_RANDOM_H_
#define MLI_RANDOM_H_

#include "../prng/prng.h"
#include "../vec/vec.h"

struct mli_Vec mli_random_position_on_disc(
        const double radius,
        struct mli_Prng *prng);
struct mli_Vec mli_random_draw_direction_in_zenith_azimuth_range(
        const struct mli_prng_ZenithRange zenith,
        const struct mli_prng_UniformRange azimuth,
        struct mli_Prng *prng);
struct mli_Vec mli_random_position_inside_unit_sphere(struct mli_Prng *prng);
#endif
