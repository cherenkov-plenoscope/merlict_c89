/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLI_COLOR_MATERIALS_H_
#define MLI_COLOR_MATERIALS_H_

#include <stdint.h>
#include "../array/array.h"
#include "../array/float_array.h"
#include "../materials/materials.h"

MLI_ARRAY_DEFINITON(mli_ColorSpectrumArray, struct mli_FloatArray)

struct mli_ColorMaterials {
        struct mli_FloatArray wavelength_bin_edges;
        struct mli_ColorSpectrumArray spectra;
};

struct mli_ColorMaterials mli_ColorMaterials_init(void);
int mli_ColorMaterials_malloc(
        struct mli_ColorMaterials *self,
        const uint64_t num_spectra);

int mli_ColorMaterials_malloc_from_Materials(
        struct mli_ColorMaterials *self,
        const struct mli_Materials *materials);

void mli_ColorMaterials_free(struct mli_ColorMaterials *self);
#endif
