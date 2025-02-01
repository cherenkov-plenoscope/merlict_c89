/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "color_materials.h"
#include <stdlib.h>
#include "../chk/chk.h"
#include "colorObserver.h"

MLI_ARRAY_IMPLEMENTATION(mli_ColorSpectrumArray, struct mli_ColorSpectrum)

struct mli_ColorSpectrumBinEdges mli_ColorSpectrumBinEdges_init(void)
{
        uint64_t i;
        const uint64_t size = MLI_COLORSPECTRUM_SIZE;
        const float start = MLI_COLORSPECTRUM_WAVELENGTH_START;
        const float stop = MLI_COLORSPECTRUM_WAVELENGTH_STOP;
        const float step = (stop - start) / (float)size;
        struct mli_ColorSpectrumBinEdges edges;
        for (i = 0; i < size + 1; i++) {
                edges.values[i] = start + (float)i * step;
        }
        return edges;
}

struct mli_ColorSpectrum mli_ColorSpectrum_init_zeros(void)
{
        struct mli_ColorSpectrum cs;
        MLI_MATH_ARRAY_SET(cs.values, 0.0, MLI_COLORSPECTRUM_SIZE);
        return cs;
}

void mli_ColorSpectrum_set(struct mli_ColorSpectrum *self, const float value)
{
        MLI_MATH_ARRAY_SET(self->values, value, MLI_COLORSPECTRUM_SIZE);
}

struct mli_ColorSpectrum mli_ColorSpectrum_add(
        const struct mli_ColorSpectrum a,
        const struct mli_ColorSpectrum b)
{
        uint64_t i;
        struct mli_ColorSpectrum c;
        for (i = 0; i < MLI_COLORSPECTRUM_SIZE; i++) {
                c.values[i] = a.values[i] + b.values[i];
        }
        return c;
}

struct mli_ColorSpectrum mli_ColorSpectrum_multiply(
        const struct mli_ColorSpectrum a,
        const struct mli_ColorSpectrum b)
{
        uint64_t i;
        struct mli_ColorSpectrum c;
        for (i = 0; i < MLI_COLORSPECTRUM_SIZE; i++) {
                c.values[i] = a.values[i] * b.values[i];
        }
        return c;
}

struct mli_ColorSpectrum mli_ColorSpectrum_multiply_scalar(
        const struct mli_ColorSpectrum a,
        const double factor)
{
        uint64_t i;
        struct mli_ColorSpectrum c;
        for (i = 0; i < MLI_COLORSPECTRUM_SIZE; i++) {
                c.values[i] = a.values[i] * factor;
        }
        return c;
}

struct mli_ColorSpectrum mli_ColorSpectrum_exp(
        const struct mli_ColorSpectrum a,
        const double factor)
{
        uint64_t i;
        struct mli_ColorSpectrum c;
        for (i = 0; i < MLI_COLORSPECTRUM_SIZE; i++) {
                c.values[i] = exp(a.values[i] * factor);
        }
        return c;
}

double mli_ColorSpectrum_multiply_and_sum(
        const struct mli_ColorSpectrum *a,
        const struct mli_ColorSpectrum *b)
{
        uint64_t i;
        double value = 0.0;
        for (i = 0; i < MLI_COLORSPECTRUM_SIZE; i++) {
                value += a->values[i] * b->values[i];
        }
        return value;
}

float mli_ColorSpectrum_sum(const struct mli_ColorSpectrum *self)
{
        uint64_t i;
        float c = 0.0;
        for (i = 0; i < MLI_COLORSPECTRUM_SIZE; i++) {
                c += self->values[i];
        }
        return c;
}

int mli_ColorSpectrum_from_func(
        struct mli_ColorSpectrum *self,
        const struct mli_ColorSpectrumBinEdges *wavelength_bin_edges,
        const struct mli_Func *func)
{
        const uint64_t NUM_SAMPLES = 100;
        uint64_t i, u;

        for (i = 0; i < MLI_COLORSPECTRUM_SIZE; i++) {
                const double start = wavelength_bin_edges->values[i];
                const double stop = wavelength_bin_edges->values[i + 1];
                const double step = (stop - start) / (double)NUM_SAMPLES;

                double val_mean = 0.0;
                for (u = 0; u < NUM_SAMPLES; u++) {
                        const double wavelength = start + (float)u * step;
                        double val = 0.0;
                        chk_msg(mli_Func_evaluate(func, wavelength, &val),
                                "Can't evaluate spectrum.");
                        val_mean += val;
                }
                val_mean = val_mean / (double)NUM_SAMPLES;
                self->values[i] = val_mean;
        }

        return 1;
chk_error:
        return 0;
}

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

int mli_ColorSpectrum_set_radiance_of_black_body_W_per_m2_per_sr(
        struct mli_ColorSpectrum *self,
        const double temperature)
{
        struct mli_ColorSpectrumBinEdges edges =
                mli_ColorSpectrumBinEdges_init();
        const uint64_t NUM_SAMPLES = 100;
        uint64_t i, u;

        for (i = 0; i < MLI_COLORSPECTRUM_SIZE; i++) {
                const double start = edges.values[i];
                const double stop = edges.values[i + 1];
                const double step = (stop - start) / (double)NUM_SAMPLES;

                double average_radiance = 0.0;

                for (u = 0; u < NUM_SAMPLES; u++) {
                        const double wavelength = start + (float)u * step;
                        const double radiance =
                                mli_physics_plancks_spectral_radiance_law_W_per_m2_per_sr_per_m(
                                        wavelength, temperature);
                        average_radiance += radiance;
                }
                average_radiance /= (double)NUM_SAMPLES;

                self->values[i] = average_radiance * step;
        }
        return 1;
}
