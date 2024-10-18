/* Copyright 2018-2024 Sebastian Achim Mueller */
#include "mliColorObserver.h"
#include "chk.h"
#include "mliFunc.h"

struct mliColorObserver mliColorObserver_init(void)
{
        struct mliColorObserver colobs;
        colobs.r = mliFunc_init();
        colobs.g = mliFunc_init();
        colobs.b = mliFunc_init();
        return colobs;
}

void mliColorObserver_free(struct mliColorObserver *colobs)
{
        mliFunc_free(&colobs->r);
        mliFunc_free(&colobs->g);
        mliFunc_free(&colobs->b);
}

int mliColorObserver_malloc_cie1931(struct mliColorObserver *colobs)
{
        /*
         * https://en.wikipedia.org/wiki/CIE_1931_color_space
         * The spectral sensitivity of a 'standard observer' as defined by the
         * International Commission on Illumination in 1931.
         *
         * Here the observer is normalized for the largest response of the
         * three channeks x, y, and z to be 1.0.
         */
        float red[][2] = {
                {2.000e-07, 0.000e+00}, {3.800e-07, 0.000e+00},
                {3.850e-07, 2.957e-03}, {3.900e-07, 5.914e-03},
                {3.950e-07, 8.871e-03}, {4.000e-07, 1.454e-02},
                {4.050e-07, 2.021e-02}, {4.100e-07, 3.462e-02},
                {4.150e-07, 5.486e-02}, {4.200e-07, 8.703e-02},
                {4.250e-07, 1.271e-01}, {4.300e-07, 1.582e-01},
                {4.350e-07, 1.844e-01}, {4.400e-07, 1.907e-01},
                {4.450e-07, 1.970e-01}, {4.500e-07, 1.875e-01},
                {4.550e-07, 1.780e-01}, {4.600e-07, 1.579e-01},
                {4.650e-07, 1.352e-01}, {4.700e-07, 1.085e-01},
                {4.750e-07, 8.082e-02}, {4.800e-07, 5.804e-02},
                {4.850e-07, 3.853e-02}, {4.900e-07, 2.404e-02},
                {4.950e-07, 1.708e-02}, {5.000e-07, 1.178e-02},
                {5.050e-07, 1.315e-02}, {5.100e-07, 1.451e-02},
                {5.150e-07, 3.042e-02}, {5.200e-07, 4.633e-02},
                {5.250e-07, 7.357e-02}, {5.300e-07, 1.036e-01},
                {5.350e-07, 1.397e-01}, {5.400e-07, 1.773e-01},
                {5.450e-07, 2.149e-01}, {5.500e-07, 2.590e-01},
                {5.550e-07, 3.076e-01}, {5.600e-07, 3.561e-01},
                {5.650e-07, 4.025e-01}, {5.700e-07, 4.489e-01},
                {5.750e-07, 4.937e-01}, {5.800e-07, 5.324e-01},
                {5.850e-07, 5.622e-01}, {5.900e-07, 5.859e-01},
                {5.950e-07, 5.949e-01}, {6.000e-07, 5.940e-01},
                {6.050e-07, 5.829e-01}, {6.100e-07, 5.566e-01},
                {6.150e-07, 5.216e-01}, {6.200e-07, 4.738e-01},
                {6.250e-07, 4.116e-01}, {6.300e-07, 3.494e-01},
                {6.350e-07, 2.991e-01}, {6.400e-07, 2.488e-01},
                {6.450e-07, 2.023e-01}, {6.500e-07, 1.608e-01},
                {6.550e-07, 1.290e-01}, {6.600e-07, 9.935e-02},
                {6.650e-07, 7.821e-02}, {6.700e-07, 5.707e-02},
                {6.750e-07, 4.652e-02}, {6.800e-07, 3.596e-02},
                {6.850e-07, 2.932e-02}, {6.900e-07, 2.365e-02},
                {6.950e-07, 2.069e-02}, {7.000e-07, 1.955e-02},
                {7.050e-07, 1.841e-02}, {7.100e-07, 1.727e-02},
                {7.150e-07, 1.625e-02}, {7.200e-07, 1.575e-02},
                {7.250e-07, 1.525e-02}, {7.300e-07, 1.475e-02},
                {7.350e-07, 1.425e-02}, {7.400e-07, 1.375e-02},
                {7.450e-07, 1.325e-02}, {7.500e-07, 1.243e-02},
                {7.550e-07, 1.036e-02}, {7.600e-07, 8.290e-03},
                {7.650e-07, 6.217e-03}, {7.700e-07, 4.145e-03},
                {7.750e-07, 2.072e-03}, {7.800e-07, 0.000e+00},
                {1.200e-06, 0.000e+00},
        };

        float green[][2] = {
                {2.000e-07, 0.000e+00}, {3.800e-07, 0.000e+00},
                {3.850e-07, 1.099e-03}, {3.900e-07, 2.198e-03},
                {3.950e-07, 3.296e-03}, {4.000e-07, 4.395e-03},
                {4.050e-07, 5.494e-03}, {4.100e-07, 6.593e-03},
                {4.150e-07, 7.691e-03}, {4.200e-07, 8.676e-03},
                {4.250e-07, 9.633e-03}, {4.300e-07, 1.155e-02},
                {4.350e-07, 1.491e-02}, {4.400e-07, 1.827e-02},
                {4.450e-07, 2.162e-02}, {4.500e-07, 2.498e-02},
                {4.550e-07, 3.303e-02}, {4.600e-07, 4.109e-02},
                {4.650e-07, 4.914e-02}, {4.700e-07, 5.719e-02},
                {4.750e-07, 6.991e-02}, {4.800e-07, 8.575e-02},
                {4.850e-07, 1.047e-01}, {4.900e-07, 1.282e-01},
                {4.950e-07, 1.574e-01}, {5.000e-07, 1.950e-01},
                {5.050e-07, 2.492e-01}, {5.100e-07, 3.075e-01},
                {5.150e-07, 3.631e-01}, {5.200e-07, 4.139e-01},
                {5.250e-07, 4.602e-01}, {5.300e-07, 4.948e-01},
                {5.350e-07, 5.257e-01}, {5.400e-07, 5.422e-01},
                {5.450e-07, 5.588e-01}, {5.500e-07, 5.625e-01},
                {5.550e-07, 5.663e-01}, {5.600e-07, 5.589e-01},
                {5.650e-07, 5.487e-01}, {5.700e-07, 5.311e-01},
                {5.750e-07, 5.086e-01}, {5.800e-07, 4.825e-01},
                {5.850e-07, 4.509e-01}, {5.900e-07, 4.193e-01},
                {5.950e-07, 3.876e-01}, {6.000e-07, 3.543e-01},
                {6.050e-07, 3.198e-01}, {6.100e-07, 2.853e-01},
                {6.150e-07, 2.509e-01}, {6.200e-07, 2.195e-01},
                {6.250e-07, 1.880e-01}, {6.300e-07, 1.566e-01},
                {6.350e-07, 1.272e-01}, {6.400e-07, 1.055e-01},
                {6.450e-07, 8.373e-02}, {6.500e-07, 6.876e-02},
                {6.550e-07, 5.379e-02}, {6.600e-07, 4.365e-02},
                {6.650e-07, 3.472e-02}, {6.700e-07, 2.878e-02},
                {6.750e-07, 2.483e-02}, {6.800e-07, 2.088e-02},
                {6.850e-07, 1.693e-02}, {6.900e-07, 1.582e-02},
                {6.950e-07, 1.542e-02}, {7.000e-07, 1.503e-02},
                {7.050e-07, 1.463e-02}, {7.100e-07, 1.424e-02},
                {7.150e-07, 1.384e-02}, {7.200e-07, 1.345e-02},
                {7.250e-07, 1.305e-02}, {7.300e-07, 1.266e-02},
                {7.350e-07, 1.226e-02}, {7.400e-07, 1.186e-02},
                {7.450e-07, 1.147e-02}, {7.500e-07, 1.079e-02},
                {7.550e-07, 8.994e-03}, {7.600e-07, 7.195e-03},
                {7.650e-07, 5.396e-03}, {7.700e-07, 3.598e-03},
                {7.750e-07, 1.799e-03}, {7.800e-07, 0.000e+00},
                {1.200e-06, 0.000e+00},
        };

        float blue[][2] = {
                {2.000e-07, 0.000e+00}, {3.800e-07, 0.000e+00},
                {3.850e-07, 9.873e-03}, {3.900e-07, 1.595e-02},
                {3.950e-07, 2.536e-02}, {4.000e-07, 4.352e-02},
                {4.050e-07, 7.715e-02}, {4.100e-07, 1.473e-01},
                {4.150e-07, 2.240e-01}, {4.200e-07, 4.372e-01},
                {4.250e-07, 6.496e-01}, {4.300e-07, 8.370e-01},
                {4.350e-07, 9.538e-01}, {4.400e-07, 9.983e-01},
                {4.450e-07, 1.000e+00}, {4.500e-07, 9.912e-01},
                {4.550e-07, 9.622e-01}, {4.600e-07, 9.131e-01},
                {4.650e-07, 7.944e-01}, {4.700e-07, 6.640e-01},
                {4.750e-07, 5.457e-01}, {4.800e-07, 4.268e-01},
                {4.850e-07, 3.338e-01}, {4.900e-07, 2.524e-01},
                {4.950e-07, 1.944e-01}, {5.000e-07, 1.536e-01},
                {5.050e-07, 1.215e-01}, {5.100e-07, 9.421e-02},
                {5.150e-07, 6.725e-02}, {5.200e-07, 4.686e-02},
                {5.250e-07, 3.640e-02}, {5.300e-07, 2.902e-02},
                {5.350e-07, 2.242e-02}, {5.400e-07, 1.802e-02},
                {5.450e-07, 1.509e-02}, {5.500e-07, 1.329e-02},
                {5.550e-07, 1.318e-02}, {5.600e-07, 1.307e-02},
                {5.650e-07, 1.296e-02}, {5.700e-07, 1.284e-02},
                {5.750e-07, 1.273e-02}, {5.800e-07, 1.262e-02},
                {5.850e-07, 1.260e-02}, {5.900e-07, 1.261e-02},
                {5.950e-07, 1.262e-02}, {6.000e-07, 1.263e-02},
                {6.050e-07, 1.264e-02}, {6.100e-07, 1.264e-02},
                {6.150e-07, 1.265e-02}, {6.200e-07, 1.266e-02},
                {6.250e-07, 1.267e-02}, {6.300e-07, 1.267e-02},
                {6.350e-07, 1.268e-02}, {6.400e-07, 1.269e-02},
                {6.450e-07, 1.270e-02}, {6.500e-07, 1.270e-02},
                {6.550e-07, 1.271e-02}, {6.600e-07, 1.272e-02},
                {6.650e-07, 1.273e-02}, {6.700e-07, 1.274e-02},
                {6.750e-07, 1.274e-02}, {6.800e-07, 1.275e-02},
                {6.850e-07, 1.276e-02}, {6.900e-07, 1.277e-02},
                {6.950e-07, 1.277e-02}, {7.000e-07, 1.278e-02},
                {7.050e-07, 1.279e-02}, {7.100e-07, 1.280e-02},
                {7.150e-07, 1.281e-02}, {7.200e-07, 1.281e-02},
                {7.250e-07, 1.282e-02}, {7.300e-07, 1.283e-02},
                {7.350e-07, 1.284e-02}, {7.400e-07, 1.284e-02},
                {7.450e-07, 1.285e-02}, {7.500e-07, 1.244e-02},
                {7.550e-07, 1.037e-02}, {7.600e-07, 8.295e-03},
                {7.650e-07, 6.222e-03}, {7.700e-07, 4.148e-03},
                {7.750e-07, 2.074e-03}, {7.800e-07, 0.000e+00},
                {1.200e-06, 0.000e+00},
        };

        uint64_t i;

        mliColorObserver_free(colobs);

        chk_msg(mliFunc_malloc(&colobs->r, sizeof(red) / sizeof(red[0])),
                "Can't malloc red channel in ColorObserver.");
        for (i = 0; i < colobs->r.num_points; i++) {
                colobs->r.x[i] = red[i][0];
                colobs->r.y[i] = red[i][1];
        }

        chk_msg(mliFunc_malloc(&colobs->g, sizeof(green) / sizeof(green[0])),
                "Can't malloc green channel in ColorObserver.");
        for (i = 0; i < colobs->g.num_points; i++) {
                colobs->g.x[i] = green[i][0];
                colobs->g.y[i] = green[i][1];
        }

        chk_msg(mliFunc_malloc(&colobs->b, sizeof(blue) / sizeof(blue[0])),
                "Can't malloc blue channel in ColorObserver.");
        for (i = 0; i < colobs->b.num_points; i++) {
                colobs->b.x[i] = blue[i][0];
                colobs->b.y[i] = blue[i][1];
        }

        chk_msg(mliColorObserver_is_valid(colobs),
                "Expected ColorObserver spectra to be valid.");
        return 1;
chk_error:
        mliColorObserver_free(colobs);
        return 0;
}

int mliColorObserver_is_valid(const struct mliColorObserver *colobs)
{
        uint64_t i;
        for (i = 0; i < colobs->r.num_points; i++) {
                chk_msg(colobs->r.y[i] >= 0.0 && colobs->r.y[i] <= 1.0,
                        "Expected channel red 0.0 <= value <= 1.0.");
        }
        for (i = 0; i < colobs->g.num_points; i++) {
                chk_msg(colobs->g.y[i] >= 0.0 && colobs->g.y[i] <= 1.0,
                        "Expected channel green 0.0 <= value <= 1.0.");
        }
        for (i = 0; i < colobs->b.num_points; i++) {
                chk_msg(colobs->b.y[i] >= 0.0 && colobs->b.y[i] <= 1.0,
                        "Expected channel blue 0.0 <= value <= 1.0.");
        }
        chk_msg(mliFunc_x_is_strictly_increasing(&colobs->r),
                "Expected wavelength in channel red to be strictly "
                "increasing.");
        chk_msg(mliFunc_x_is_strictly_increasing(&colobs->g),
                "Expected wavelength in channel green to be strictly "
                "increasing.");
        chk_msg(mliFunc_x_is_strictly_increasing(&colobs->b),
                "Expected wavelength in channel blue to be strictly "
                "increasing.");
        return 1;
chk_error:
        return 0;
}

int mliColorObserver_evaluate(
        const struct mliColorObserver *colobs,
        const struct mliFunc *func,
        struct mliColor *color)
{
        double r, g, b;
        (*color) = mliColor_set(0.0, 0.0, 0.0);
        chk_msg(func->num_points > 1, "Expected function's num_points > 1");
        chk_msg(mliFunc_fold_numeric_default_closest(&colobs->r, func, &r),
                "Can't fold red channel.");
        chk_msg(mliFunc_fold_numeric_default_closest(&colobs->g, func, &g),
                "Can't fold green channel.");
        chk_msg(mliFunc_fold_numeric_default_closest(&colobs->b, func, &b),
                "Can't fold blue channel.");
        color->r = (float)(r);
        color->g = (float)(g);
        color->b = (float)(b);
        return 1;
chk_error:
        return 0;
}
