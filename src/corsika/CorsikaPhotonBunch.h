/* Copyright 2016 Sebastian A. Mueller, Dominik Neise */
#ifndef MLI_CORSIKA_CORSIKAPHOTONBUNCH_H_
#define MLI_CORSIKA_CORSIKAPHOTONBUNCH_H_

#include <stdint.h>
#include "../vector/vector.h"
#include "../vec/vec.h"
#include "../photon/photon.h"

struct mli_corsika_PhotonBunch {
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

MLI_VECTOR_DEFINITON(mliDynCorsikaPhotonBunch, struct mli_corsika_PhotonBunch)

void mli_corsika_PhotonBunch_set_from_raw(
        struct mli_corsika_PhotonBunch *bunch,
        const float *raw);
void mli_corsika_PhotonBunch_to_raw(
        const struct mli_corsika_PhotonBunch *bunch,
        float *raw);

struct mli_Photon mli_corsika_PhotonBunch_to_merlict_photon(
        const struct mli_corsika_PhotonBunch bunch,
        const double production_distance_offset,
        const int64_t id);

struct mli_Vec mli_corsika_photon_direction_of_motion(
        const struct mli_corsika_PhotonBunch bunch);

struct mli_Vec mli_corsika_photon_support_on_observation_level(
        const struct mli_corsika_PhotonBunch bunch);

double mli_corsika_photon_wavelength(
        const struct mli_corsika_PhotonBunch bunch);

double mli_corsika_photon_emission_height(
        const struct mli_corsika_PhotonBunch bunch);

double mli_corsika_photon_relative_arrival_time_on_observation_level(
        const struct mli_corsika_PhotonBunch bunch);

#endif
