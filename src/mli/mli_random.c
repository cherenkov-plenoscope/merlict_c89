/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "mli_random.h"
#include <math.h>
#include <assert.h>
#include "../math/math.h"

/*
        direction
        ==========
*/
struct mliVec mli_random_draw_direction_in_zenith_azimuth_range(
        const struct mli_prng_ZenithRange zenith,
        const struct mli_prng_UniformRange azimuth,
        struct mli_Prng *prng)
{
        const double az = mli_prng_draw_uniform(azimuth, prng);
        const double zd = mli_prng_draw_zenith(zenith, prng);
        const double sin_zd = sin(zd);
        return mliVec_init(sin_zd * cos(az), sin_zd * sin(az), cos(zd));
}

struct mliVec mli_random_position_on_disc(
        const double radius,
        struct mli_Prng *prng)
{
        const double r = sqrt(mli_Prng_uniform(prng)) * radius;
        const double azimuth = mli_Prng_uniform(prng) * MLI_MATH_2PI;
        return mliVec_init(r * cos(azimuth), r * sin(azimuth), 0.0);
}

struct mliVec mli_random_position_inside_unit_sphere(struct mli_Prng *prng)
{
        /* rejection sampling */
        struct mliVec pos;
        do {
                pos.x = -1.0 + 2.0 * mli_Prng_uniform(prng);
                pos.y = -1.0 + 2.0 * mli_Prng_uniform(prng);
                pos.z = -1.0 + 2.0 * mli_Prng_uniform(prng);
        } while ((pos.x * pos.x + pos.y * pos.y + pos.z * pos.z) > 1.0);
        return pos;
}
