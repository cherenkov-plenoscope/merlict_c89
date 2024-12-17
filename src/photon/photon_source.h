/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLI_PHOTON_SOURCE_H_
#define MLI_PHOTON_SOURCE_H_

#include <stdint.h>
#include "photon_vector.h"
#include "../prng/prng.h"

int mli_photon_source_point_like_opening_cone_towards_z(
        struct mli_PhotonVector *out_photons,
        const double wavelength,
        const double opening_angle,
        const uint64_t num_photons,
        struct mli_Prng *prng);
int mli_photon_source_parallel_towards_z_from_xy_disc(
        struct mli_PhotonVector *out_photons,
        const double wavelength,
        const double radius,
        const uint64_t num_photons,
        struct mli_Prng *prng);
#endif
