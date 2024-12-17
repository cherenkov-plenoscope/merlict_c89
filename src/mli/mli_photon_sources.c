/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "mli_photon_sources.h"
#include "../chk/chk.h"
#include "../vec/vec_random.h"
#include "../math/math.h"

int mli_photon_source_parallel_towards_z_from_xy_disc(
        struct mliDynPhoton *out_photons,
        const double wavelength,
        const double radius,
        const uint64_t num_photons,
        struct mli_Prng *prng)
{
        uint64_t i;
        const struct mli_Vec direction = mli_Vec_init(0., 0., 1.);
        for (i = 0; i < num_photons; i++) {
                struct mliPhoton ph;
                ph.ray.support = mli_random_position_on_disc(radius, prng);
                ph.ray.direction = direction;
                ph.wavelength = wavelength;
                ph.id = i;
                chk(mliDynPhoton_push_back(out_photons, ph));
        }
        return 1;
chk_error:
        return 0;
}

int mli_photon_source_point_like_opening_cone_towards_z(
        struct mliDynPhoton *out_photons,
        const double wavelength,
        const double opening_angle,
        const uint64_t num_photons,
        struct mli_Prng *prng)
{
        uint64_t i;
        struct mli_prng_UniformRange azimuth =
                mli_prng_UniformRange_set(0.0, 2.0 * MLI_MATH_PI);
        struct mli_prng_ZenithRange zenith =
                mli_prng_ZenithRange_set(0.0, opening_angle);
        for (i = 0; i < num_photons; i++) {
                struct mli_Vec direction =
                        mli_random_draw_direction_in_zenith_azimuth_range(
                                zenith, azimuth, prng);
                struct mliPhoton ph;
                ph.ray.support = mli_Vec_init(0., 0., 0.);
                ph.ray.direction = direction;
                ph.wavelength = wavelength;
                ph.id = i;
                chk(mliDynPhoton_push_back(out_photons, ph));
        }
        return 1;
chk_error:
        return 0;
}
