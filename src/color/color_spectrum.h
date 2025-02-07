/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLI_COLOR_SPECTRUM_H_
#define MLI_COLOR_SPECTRUM_H_

#include "../chk/chk.h"
struct mli_Func;

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

void mli_ColorSpectrum_set_radiance_of_black_body_W_per_m2_per_sr(
        struct mli_ColorSpectrum *self,
        const double temperature);

chk_rc mli_ColorSpectrum_from_func(
        struct mli_ColorSpectrum *self,
        const struct mli_ColorSpectrumBinEdges *wavelength_bin_edges,
        const struct mli_Func *func);

#endif
