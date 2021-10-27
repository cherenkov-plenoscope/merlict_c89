/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLI_PHOTON_SOURCES_H_
#define MLI_PHOTON_SOURCES_H_

#include "mliDynPhoton.h"

int point_like_towards_z_opening_angle_num_photons(
        struct mliDynPhoton *out_photons,
        const double wavelength,
        const double opening_angle,
        const uint64_t num_photons,
        struct mliPrng *prng);
int mli_photon_source_parallel_towards_z_from_xy_disc(
        struct mliDynPhoton *out_photons,
        const double wavelength,
        const double radius,
        const uint64_t num_photons,
        struct mliPrng *prng);
#endif
