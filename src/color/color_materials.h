/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLI_COLOR_MATERIALS_H_
#define MLI_COLOR_MATERIALS_H_

#include <stdint.h>
#include "../array/array.h"
#include "../vec/vec.h"
#include "../materials/materials.h"

#define MLI_COLORSPECTRUM_SIZE 30
#define MLI_COLORSPECTRUM_WAVELENGTH_START 400e-9
#define MLI_COLORSPECTRUM_WAVELENGTH_STOP 700e-9

struct mli_ColorSpectrumBinEdges {
        float values[MLI_COLORSPECTRUM_SIZE + 1];
};

struct mli_ColorSpectrumBinEdges mli_ColorSpectrumBinEdges_init(void);

struct mli_ColorSpectrum {
        float values[MLI_COLORSPECTRUM_SIZE];
};

struct mli_ColorSpectrum mli_ColorSpectrum_init_zeros(void);

struct mli_ColorSpectrum mli_ColorSpectrum_add(
        const struct mli_ColorSpectrum a,
        const struct mli_ColorSpectrum b);

struct mli_ColorSpectrum mli_ColorSpectrum_exp(
        const struct mli_ColorSpectrum a,
        const double factor);

struct mli_ColorSpectrum mli_ColorSpectrum_multiply(
        const struct mli_ColorSpectrum a,
        const struct mli_ColorSpectrum b);

struct mli_ColorSpectrum mli_ColorSpectrum_multiply_scalar(
        const struct mli_ColorSpectrum a,
        const double factor);

double mli_ColorSpectrum_multiply_and_sum(
        const struct mli_ColorSpectrum *a,
        const struct mli_ColorSpectrum *b);

float mli_ColorSpectrum_sum(const struct mli_ColorSpectrum *self);

void mli_ColorSpectrum_set(struct mli_ColorSpectrum *self, const float value);

int mli_ColorSpectrum_set_radiance_of_black_body_W_per_m2_per_sr(
        struct mli_ColorSpectrum *self,
        const double temperature);

MLI_ARRAY_DEFINITON(mli_ColorSpectrumArray, struct mli_ColorSpectrum)

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
