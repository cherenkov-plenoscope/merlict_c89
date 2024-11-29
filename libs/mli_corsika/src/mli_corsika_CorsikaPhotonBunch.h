/* Copyright 2016 Sebastian A. Mueller, Dominik Neise */
#ifndef MLI_CORSIKA_CORSIKAPHOTONBUNCH_H_
#define MLI_CORSIKA_CORSIKAPHOTONBUNCH_H_

#include <stdint.h>
#include "../../mtl/src/vector.h"
#include "../../mli/src/mliVec.h"
#include "../../mli/src/mliPhoton.h"

struct mliCorsikaPhotonBunch {
        /*
         * x in cm
         * y in cm
         * ux = sin(theta) * cos(phi)
         * vy = sin(theta) * sin(phi)
         * time in nanoseconds since first interaction.
         * zem
         * photons
         * wavelength is in nanometer negative if scattered ?!
         *
         * KIT-CORSIKA coordinate-system
         *                /\ z-axis                                            *
         *                |                                                    *
         *                |\ p                                                 *
         *                | \ a                                                *
         *                |  \ r                                               *
         *                |   \ t                                              *
         *                |    \ i                                             *
         *                |     \ c                                            *
         *                |      \ l                                           *
         *                |       \ e                                          *
         *                |        \                                           *
         *                |  theta  \ m                                        *
         *                |       ___\ o                                       *
         *                |___----    \ m      ___                             *
         *                |            \ e       /| y-axis (west)              *
         *                |             \ n    /                               *
         *                |              \ t /                                 *
         *                |               \/u                                  *
         *                |              / \ m                                 *
         *                |            /    \                                  *
         *                |          /       \                                 *
         *                |        /__________\                                *
         *                |      /      ___---/                                *
         *                |    /   __---    /                                  *
         *                |  /__--- phi \ /                                    *
         * _______________|/--__________/______\ x-axis (north)                *
         *               /|                    /                               *
         *             /  |                                                    *
         *           /    |                                                    *
         *         /                                                           *
         *                                                                     *
         *                                                                     *
         * Extensive Air Shower Simulation with CORSIKA, Figure 1, page 114
         * (Version 7.6400 from December 27, 2017)
         *
         * Direction-cosines:
         *      ux = sin(theta) * cos(phi)
         *      vy = sin(theta) * sin(phi)
         * The zenith-angle theta opens relative to the negative z-axis.
         * It is the momentum of the Cherenkov-photon, which is pointing
         * down towards the observation-plane.
         */
        float x_cm;
        float y_cm;
        float ux;
        float vy;
        float time_ns;
        float z_emission_cm;
        float weight_photons;
        float wavelength_nm;
};

MTL_VECTOR_DEFINITON(mliDynCorsikaPhotonBunch, struct mliCorsikaPhotonBunch)

void mliCorsikaPhotonBunch_set_from_raw(
        struct mliCorsikaPhotonBunch *bunch,
        const float *raw);
void mliCorsikaPhotonBunch_to_raw(
        const struct mliCorsikaPhotonBunch *bunch,
        float *raw);

struct mliPhoton mliCorsikaPhotonBunch_to_merlict_photon(
        const struct mliCorsikaPhotonBunch bunch,
        const double production_distance_offset,
        const int64_t id);

struct mliVec mli_corsika_photon_direction_of_motion(
        const struct mliCorsikaPhotonBunch bunch);

struct mliVec mli_corsika_photon_support_on_observation_level(
        const struct mliCorsikaPhotonBunch bunch);

double mli_corsika_photon_wavelength(const struct mliCorsikaPhotonBunch bunch);

double mli_corsika_photon_emission_height(
        const struct mliCorsikaPhotonBunch bunch);

double mli_corsika_photon_relative_arrival_time_on_observation_level(
        const struct mliCorsikaPhotonBunch bunch);

#endif
