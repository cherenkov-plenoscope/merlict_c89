/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "color_materials.h"
#include <stdlib.h>
#include "../chk/chk.h"

MLI_ARRAY_IMPLEMENTATION_FREE(
        mli_ColorSpectrumArray,
        struct mli_FloatArray,
        mli_FloatArray_free)

struct mli_ColorMaterials mli_ColorMaterials_init(void)
{
        struct mli_ColorMaterials out;
        out.wavelength_bin_edges = mli_FloatArray_init();
        out.spectra = mli_ColorSpectrumArray_init();
        return out;
}

void mli_ColorMaterials_free(struct mli_ColorMaterials *self)
{
        mli_FloatArray_free(&self->wavelength_bin_edges);
        mli_ColorSpectrumArray_free(&self->spectra);
        (*self) = mli_ColorMaterials_init();
}

int mli_ColorMaterials_malloc(
        struct mli_ColorMaterials *self,
        const uint64_t num_spectra)
{
        const uint64_t num_bins = 30;
        uint64_t iii;
        float start = 400e-9;
        float stop = 700e-9;
        float step = (stop - start) / (float)num_bins;
        mli_ColorMaterials_free(self);
        chk_msg(mli_FloatArray_malloc(
                        &self->wavelength_bin_edges, num_bins + 1),
                "Failed to malloc wavelength_bin_edges");
        for (iii = 0; iii < num_bins + 1; iii++) {
                self->wavelength_bin_edges.array[iii] =
                        start + (float)iii * step;
        }

        chk_msg(mli_ColorSpectrumArray_malloc(&self->spectra, num_spectra),
                "Failed to malloc spectra");
        for (iii = 0; iii < self->spectra.size; iii++) {
                self->spectra.array[iii] = mli_FloatArray_init();
                chk(mli_FloatArray_malloc(&self->spectra.array[iii], num_bins));
        }

        return 1;
chk_error:
        mli_ColorMaterials_free(self);
        return 0;
}

int mli_ColorMaterials_malloc_from_Materials(
        struct mli_ColorMaterials *self,
        const struct mli_Materials *materials)
{
        uint64_t iii, www, uuu;
        const uint64_t num_samples = 100;
        chk_msg(mli_ColorMaterials_malloc(self, materials->spectra.size),
                "Can't malloc ColorMaterials from Materials.");

        for (iii = 0; iii < materials->spectra.size; iii++) {
                for (www = 0; www < self->wavelength_bin_edges.size - 1;
                     www++) {
                        const float start =
                                self->wavelength_bin_edges.array[www];
                        const float stop =
                                self->wavelength_bin_edges.array[www + 1];
                        const float step = (stop - start) / (float)num_samples;

                        float val_mean = 0.0;
                        for (uuu = 0; uuu < num_samples; uuu++) {
                                const float wavelength =
                                        start + (float)uuu * step;
                                double val = 0.0;
                                chk_msg(mli_Func_evaluate(
                                                &materials->spectra.array[iii]
                                                         .spectrum,
                                                wavelength,
                                                &val),
                                        "Can't evaluate spectrum.");
                                val_mean += (float)val;
                        }
                        val_mean = val_mean / (float)num_samples;
                        self->spectra.array[iii].array[www] = val_mean;
                }
        }

        return 1;
chk_error:
        mli_ColorMaterials_free(self);
        return 0;
}
