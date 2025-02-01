/* Copyright 2018-2024 Sebastian Achim Mueller */
#include "colorObserver.h"
#include "../chk/chk.h"
#include "../func/func.h"
#include "../physics/physics.h"

struct mli_ColorObserver mli_ColorObserver_init(void)
{
        struct mli_ColorObserver out;
        out.x = mli_Func_init();
        out.y = mli_Func_init();
        out.z = mli_Func_init();
        return out;
}

void mli_ColorObserver_free(struct mli_ColorObserver *self)
{
        mli_Func_free(&self->x);
        mli_Func_free(&self->y);
        mli_Func_free(&self->z);
}

int mli_ColorObserver_malloc_cie1931(struct mli_ColorObserver *self)
{
        /*
         * https://en.wikipedia.org/wiki/CIE_1931_color_space
         * The spectral sensitivity of a 'standard observer' as defined by the
         * International Commission on Illumination in 1931.
         *
         * Here the observer is normalized for the largest response of the
         * three channeks x, y, and z to be 1.0.
         */
        mli_ColorObserver_free(self);
        chk(mli_Func_malloc_cie1931_spectral_matching_curve_x(&self->x));
        chk(mli_Func_malloc_cie1931_spectral_matching_curve_y(&self->y));
        chk(mli_Func_malloc_cie1931_spectral_matching_curve_z(&self->z));
        chk_msg(mli_ColorObserver_is_valid(self),
                "Expected ColorObserver spectra to be valid.");
        return 1;
chk_error:
        mli_ColorObserver_free(self);
        return 0;
}

int mli_ColorObserver_is_valid(const struct mli_ColorObserver *self)
{
        uint64_t i;
        for (i = 0; i < self->x.num_points; i++) {
                chk_msg(self->x.y[i] >= 0.0 && self->x.y[i] <= 1.0,
                        "Expected 0.0 <= X <= 1.0.");
        }
        for (i = 0; i < self->y.num_points; i++) {
                chk_msg(self->y.y[i] >= 0.0 && self->y.y[i] <= 1.0,
                        "Expected 0.0 <= Y <= 1.0.");
        }
        for (i = 0; i < self->z.num_points; i++) {
                chk_msg(self->z.y[i] >= 0.0 && self->z.y[i] <= 1.0,
                        "Expected 0.0 <= Z <= 1.0.");
        }
        chk_msg(mli_Func_x_is_strictly_increasing(&self->x),
                "Expected wavelength in X to be strictly "
                "increasing.");
        chk_msg(mli_Func_x_is_strictly_increasing(&self->y),
                "Expected wavelength in Y to be strictly "
                "increasing.");
        chk_msg(mli_Func_x_is_strictly_increasing(&self->z),
                "Expected wavelength in Z to be strictly "
                "increasing.");
        return 1;
chk_error:
        return 0;
}

int mli_ColorObserver_evaluate(
        const struct mli_ColorObserver *self,
        const struct mli_Func *func,
        struct mli_Color *color)
{
        double r, g, b;
        (*color) = mli_Color_set(0.0, 0.0, 0.0);
        chk_msg(func->num_points > 1, "Expected function's num_points > 1");
        chk_msg(mli_Func_fold_numeric_default_closest(&self->x, func, &r),
                "Can't fold red channel.");
        chk_msg(mli_Func_fold_numeric_default_closest(&self->y, func, &g),
                "Can't fold green channel.");
        chk_msg(mli_Func_fold_numeric_default_closest(&self->z, func, &b),
                "Can't fold blue channel.");
        color->r = (float)(r);
        color->g = (float)(g);
        color->b = (float)(b);
        return 1;
chk_error:
        return 0;
}

int mli_Func_malloc_cie1931_spectral_matching_curve_x(struct mli_Func *self)
{
        float X[][2] = {
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
        uint64_t i;
        chk_msg(mli_Func_malloc(self, sizeof(X) / sizeof(X[0])),
                "Can't malloc function from cie1931.X.");
        for (i = 0; i < self->num_points; i++) {
                self->x[i] = X[i][0];
                self->y[i] = X[i][1];
        }
        return 1;
chk_error:
        return 0;
}

int mli_Func_malloc_cie1931_spectral_matching_curve_y(struct mli_Func *self)
{
        float Y[][2] = {
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
        uint64_t i;
        chk_msg(mli_Func_malloc(self, sizeof(Y) / sizeof(Y[0])),
                "Can't malloc function from cie1931.Y.");
        for (i = 0; i < self->num_points; i++) {
                self->x[i] = Y[i][0];
                self->y[i] = Y[i][1];
        }
        return 1;
chk_error:
        return 0;
}

int mli_Func_malloc_cie1931_spectral_matching_curve_z(struct mli_Func *self)
{
        float Z[][2] = {
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
        chk_msg(mli_Func_malloc(self, sizeof(Z) / sizeof(Z[0])),
                "Can't malloc function from cie1931.Z.");
        for (i = 0; i < self->num_points; i++) {
                self->x[i] = Z[i][0];
                self->y[i] = Z[i][1];
        }
        return 1;
chk_error:
        return 0;
}

struct mli_Mat mli_Mat_cie1931_spectral_matching_xyz_to_rgb(void)
{
        struct mli_Mat m;
        m.r00 = +3.240479;
        m.r10 = -1.537150;
        m.r20 = -0.498535;
        m.r01 = -0.969256;
        m.r11 = +1.875991;
        m.r21 = +0.041556;
        m.r02 = +0.055648;
        m.r12 = -0.204043;
        m.r22 = +1.057311;
        return m;
}

struct mli_Mat mli_ColorObserver_rgb_to_xyz(void)
{
        struct mli_Mat m;
        m.r00 = +0.412453;
        m.r10 = +0.357580;
        m.r20 = +0.180423;
        m.r01 = +0.212671;
        m.r11 = +0.715160;
        m.r21 = +0.072169;
        m.r02 = +0.019334;
        m.r12 = +0.119193;
        m.r22 = +0.950227;
        return m;
}

int mli_Func_malloc_spectral_radiance_of_black_body_W_per_m2_per_sr_per_m(
        struct mli_Func *self,
        const double wavelength_start,
        const double wavelength_stop,
        const double temperature,
        const uint64_t num_points)
{
        uint64_t i;
        const double start = wavelength_start;
        const double stop = wavelength_stop;
        const double step = (stop - start) / (double)num_points;
        chk_msg(mli_Func_malloc(self, num_points),
                "Can't malloc function for black body spectrum.");
        for (i = 0; i < self->num_points; i++) {
                const double wavelength = start + step * (double)i;
                const double radiance =
                        mli_physics_plancks_spectral_radiance_law_W_per_m2_per_sr_per_m(
                                wavelength, temperature);
                self->x[i] = wavelength;
                self->y[i] = radiance;
        }
        return 1;
chk_error:
        return 0;
}

int mli_Func_malloc_spectral_irradiance_of_sky_at_sealevel_W_per_m2_per_m(
        struct mli_Func *self)
{
        /*
        Spectral irradiance of the sky at sea level
        -------------------------------------------
        CIE publication Number 85, in year 1989
        - 0m above sea level
        - Relative air mass: 1
        - Water vapor content: 1.42 cm precipitable water
        - Ozone content: 0.34 cm at standard temperature and pressure (STP), 0C,
        1 atmosphere
        - Spectral optical depth of aerosol extinction (at 500 nm): 0.1
        - Ground reflectance: 0.2

        wavelength / nm, spectral irradiance / W m{^-2} (nm){^-1}
        */
        float I[][2] = {
                {200.00, 0.000000},  {308.16, 0.011976},  {316.47, 0.116766},
                {329.76, 0.215569},  {351.37, 0.305389},  {369.65, 0.473054},
                {402.88, 0.544910},  {399.56, 0.874251},  {424.48, 0.970060},
                {434.45, 0.847305},  {449.41, 1.287425},  {485.97, 1.344311},
                {544.13, 1.356287},  {592.32, 1.326347},  {643.83, 1.281437},
                {657.13, 1.211776},  {667.10, 1.289621},  {685.38, 1.262675},
                {687.04, 1.062076},  {698.67, 1.187824},  {716.95, 1.172854},
                {718.61, 1.011178},  {735.23, 1.098004},  {756.83, 1.127944},
                {765.14, 0.436327},  {771.79, 1.077046},  {788.40, 1.059082},
                {790.07, 1.011178},  {810.01, 0.987226},  {814.99, 0.807585},
                {831.61, 0.870459},  {836.60, 0.924351},  {854.87, 0.879441},
                {883.12, 0.870459},  {893.09, 0.825549},  {901.40, 0.630938},
                {924.67, 0.636926},  {934.64, 0.268663},  {961.23, 0.340519},
                {974.52, 0.577046},  {986.15, 0.693812},  {1036.00, 0.648902},
                {1090.84, 0.544112}, {1112.44, 0.403393}, {1115.77, 0.169860},
                {1132.39, 0.133932}, {1153.99, 0.235729}, {1165.62, 0.406387},
                {1220.46, 0.430339}, {1255.35, 0.427345}, {1265.32, 0.370459},
                {1293.57, 0.394411}, {1315.18, 0.304591}, {1330.13, 0.208782},
                {1348.41, 0.098004}, {1353.40, 0.011178}};
        uint64_t i;
        chk_msg(mli_Func_malloc(self, sizeof(I) / sizeof(I[0])),
                "Can't malloc function from Spectrum.");
        for (i = 0; i < self->num_points; i++) {
                self->x[i] = I[i][0] * 1e-9;
                self->y[i] = I[i][1] * 1e9;
        }
        return 1;
chk_error:
        return 0;
}
