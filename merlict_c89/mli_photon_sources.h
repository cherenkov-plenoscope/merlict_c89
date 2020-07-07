/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MERLICT_C89_MLI_PHOTON_SOURCES_H_
#define MERLICT_C89_MLI_PHOTON_SOURCES_H_

#include <math.h>

#include "mliDynPhoton.h"

int point_like_towards_z_opening_angle_num_photons(
        struct mliDynPhoton *out_photons,
        const double wavelength,
        const double opening_angle,
        const uint64_t num_photons,
        struct mliMT19937 *prng);
int mli_photon_source_parallel_towards_z_from_xy_disc(
        struct mliDynPhoton *out_photons,
        const double wavelength,
        const double radius,
        const uint64_t num_photons,
        struct mliMT19937 *prng);
#endif
