/* Copyright 2016 Sebastian A. Mueller, Dominik Neise */
#ifndef MLI_CORSIKA_CORSIKAPHOTONBUNCH_H_
#define MLI_CORSIKA_CORSIKAPHOTONBUNCH_H_

#include <stdint.h>
#include "../../mli/src/mliDynArray.h"
#include "../../mli/src/mliVec.h"
#include "../../mli/src/mliPhoton.h"

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
