/* Copyright 2019 Sebastian Achim Mueller */
#ifndef MERLICT_MLI_PHOTON_SOURCES_H_
#define MERLICT_MLI_PHOTON_SOURCES_H_

#include "mliDynPhoton.h"
#include <math.h>

int mli_photon_source_parallel_towards_z_from_xy_disc(
        struct mliDynPhoton *out_photons,
        const double wavelength,
        const double radius,
        const uint64_t num_photons,
        struct mliMT19937 *prng)
{
        uint64_t i;
        const struct mliVec direction = mliVec_set(0., 0., 1.);
        for (i = 0; i < num_photons; i++) {
                struct mliPhoton ph;
                ph.ray.support = mli_random_position_on_disc(radius, prng);
                ph.ray.direction = direction;
                ph.wavelength = wavelength;
                ph.simulation_truth_id = i;
                mli_c(mliDynPhoton_push_back(out_photons, ph));
        }
        return 1;
error:
        return 0;
}

int point_like_towards_z_opening_angle_num_photons(
        struct mliDynPhoton *out_photons,
        const double wavelength,
        const double opening_angle,
        const uint64_t num_photons,
        struct mliMT19937 *prng)
{
        uint64_t i;
        struct mliRandomUniformRange azimuth =
                mliRandomUniformRange_set(0.0, 2.0 * MLI_PI);
        struct mliRandomZenithRange zenith =
                mliRandomZenithRange_set(0.0, opening_angle);
        for (i = 0; i < num_photons; i++) {
                struct mliVec direction =
                        mli_random_draw_direction_in_zenith_azimuth_range(
                                zenith, azimuth, prng);
                struct mliPhoton ph;
                ph.ray.support = mliVec_set(0., 0., 0.);
                ph.ray.direction = direction;
                ph.wavelength = wavelength;
                ph.simulation_truth_id = i;
                mli_c(mliDynPhoton_push_back(out_photons, ph));
        }
        return 1;
error:
        return 0;
}

#endif
