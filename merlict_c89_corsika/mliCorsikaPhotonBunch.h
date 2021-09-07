/* Copyright 2016 Sebastian A. Mueller, Dominik Neise */
#ifndef MERLICT_C89_CORSIKA_CORSIKAPHOTONBUNCH_H_
#define MERLICT_C89_CORSIKA_CORSIKAPHOTONBUNCH_H_

#include <stdint.h>
#include "../merlict_c89/mliDynArray_template.h"
#include "../merlict_c89/mliPhoton.h"

struct mliCorsikaPhotonBunch {
        /*
        * x in cm
        * y in cm
        * cx
        * cy
        * time in nanoseconds since first interaction.
        * zem
        * photons
        * wavelength is in nanometer negative if scattered ?!
        */
        float x_cm;
        float y_cm;
        float cx_rad;
        float cy_rad;
        float time_ns;
        float z_emission_cm;
        float weight_photons;
        float wavelength_nm;
};

MLIDYNARRAY_DEFINITON(mli, CorsikaPhotonBunch, struct mliCorsikaPhotonBunch)

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