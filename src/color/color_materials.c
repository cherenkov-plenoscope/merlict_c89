/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "color_materials.h"
#include <stdlib.h>
#include "../chk/chk.h"
#include "colorObserver.h"

MLI_ARRAY_IMPLEMENTATION(mli_ColorSpectrumArray, struct mli_ColorSpectrum)

struct mli_ColorMaterials mli_ColorMaterials_init(void)
{
        struct mli_ColorMaterials out;
        out.wavelength_bin_edges = mli_ColorSpectrumBinEdges_init();

        out.observer_matching_curve_x = mli_ColorSpectrum_init_zeros();
        out.observer_matching_curve_y = mli_ColorSpectrum_init_zeros();
        out.observer_matching_curve_z = mli_ColorSpectrum_init_zeros();

        out.observer_matching_curve_xyz_to_rgb = mli_Mat_unity();

        out.spectra = mli_ColorSpectrumArray_init();
        return out;
}

void mli_ColorMaterials_free(struct mli_ColorMaterials *self)
{
        mli_ColorSpectrumArray_free(&self->spectra);
        (*self) = mli_ColorMaterials_init();
}

int mli_ColorMaterials_set_observer_cie1931(struct mli_ColorMaterials *self)
{
        struct mli_Func func = mli_Func_init();

        self->observer_matching_curve_xyz_to_rgb =
                mli_Mat_cie1931_spectral_matching_xyz_to_rgb();

        chk(mli_Func_malloc_cie1931_spectral_matching_curve_x(&func));
        chk_msg(mli_ColorSpectrum_from_func(
                        &self->observer_matching_curve_x,
                        &self->wavelength_bin_edges,
                        &func),
                "Failed to resample cie1931.x onto ColorSpectrum.");

        chk(mli_Func_malloc_cie1931_spectral_matching_curve_y(&func));
        chk_msg(mli_ColorSpectrum_from_func(
                        &self->observer_matching_curve_y,
                        &self->wavelength_bin_edges,
                        &func),
                "Failed to resample cie1931.y onto ColorSpectrum.");

        chk(mli_Func_malloc_cie1931_spectral_matching_curve_z(&func));
        chk_msg(mli_ColorSpectrum_from_func(
                        &self->observer_matching_curve_z,
                        &self->wavelength_bin_edges,
                        &func),
                "Failed to resample cie1931.z onto ColorSpectrum.");

        mli_Func_free(&func);
        return 1;
chk_error:
        return 0;
}

int mli_ColorMaterials_malloc(
        struct mli_ColorMaterials *self,
        const uint64_t num_spectra)
{
        mli_ColorMaterials_free(self);
        chk_msg(mli_ColorSpectrumArray_malloc(&self->spectra, num_spectra),
                "Failed to malloc spectra");

        chk_msg(mli_ColorMaterials_set_observer_cie1931(self),
                "Failed to set observer_matching_curves.");
        return 1;
chk_error:
        mli_ColorMaterials_free(self);
        return 0;
}

int mli_ColorMaterials_malloc_from_Materials(
        struct mli_ColorMaterials *self,
        const struct mli_Materials *materials)
{
        uint64_t i;
        chk_msg(mli_ColorMaterials_malloc(self, materials->spectra.size),
                "Can't malloc ColorMaterials from Materials.");

        for (i = 0; i < materials->spectra.size; i++) {
                chk_msg(mli_ColorSpectrum_from_func(
                                &self->spectra.array[i],
                                &self->wavelength_bin_edges,
                                &materials->spectra.array[i].spectrum),
                        "Failed to resample spectrum onto ColorSpectrum.");
        }

        return 1;
chk_error:
        mli_ColorMaterials_free(self);
        return 0;
}

struct mli_Vec mli_ColorMaterials_ColorSpectrum_to_xyz(
        const struct mli_ColorMaterials *self,
        const struct mli_ColorSpectrum *spectrum)
{
        struct mli_Vec observer;
        observer.x = mli_ColorSpectrum_multiply_and_sum(
                &self->observer_matching_curve_x, spectrum);
        observer.y = mli_ColorSpectrum_multiply_and_sum(
                &self->observer_matching_curve_y, spectrum);
        observer.z = mli_ColorSpectrum_multiply_and_sum(
                &self->observer_matching_curve_z, spectrum);
        return observer;
}
