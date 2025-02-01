/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLI_COLOR_MATERIALS_H_
#define MLI_COLOR_MATERIALS_H_

#include <stdint.h>
#include "../mat/mat.h"
#include "../materials/materials.h"
#include "color_spectrum.h"
#include "color_spectrum_array.h"

struct mli_ColorMaterials {
        struct mli_ColorSpectrumBinEdges wavelength_bin_edges;

        struct mli_ColorSpectrum observer_matching_curve_x;
        struct mli_ColorSpectrum observer_matching_curve_y;
        struct mli_ColorSpectrum observer_matching_curve_z;

        struct mli_Mat observer_matching_curve_xyz_to_rgb;

        struct mli_ColorSpectrumArray spectra;
};

struct mli_Vec mli_ColorMaterials_ColorSpectrum_to_xyz(
        const struct mli_ColorMaterials *self,
        const struct mli_ColorSpectrum *spectrum);

struct mli_ColorMaterials mli_ColorMaterials_init(void);
int mli_ColorMaterials_malloc(
        struct mli_ColorMaterials *self,
        const uint64_t num_spectra);
int mli_ColorMaterials_set_observer_cie1931(struct mli_ColorMaterials *self);

int mli_ColorMaterials_malloc_from_Materials(
        struct mli_ColorMaterials *self,
        const struct mli_Materials *materials);

void mli_ColorMaterials_free(struct mli_ColorMaterials *self);
#endif
