/* Copyright 2018-2024 Sebastian Achim Mueller */
#include "color_spectrum.h"
#include "../chk/chk.h"
#include "../math/math.h"
#include "../func/func.h"
#include "../physics/physics.h"

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

        return CHK_SUCCESS;
chk_error:
        return CHK_FAIL;
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
