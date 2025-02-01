/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLICOLOROBSERVER_H_
#define MLICOLOROBSERVER_H_

#include "../color/color.h"
#include "../func/func.h"
#include "../mat/mat.h"

struct mli_ColorObserver {
        /* Color detection efficiency by wavelength.
         * The color observer has three color channels (x,y,z).
         * The functions x,y,z define how these channels respond to a
         * specific wavelength of light.
         */
        struct mli_Func x;
        struct mli_Func y;
        struct mli_Func z;
};

struct mli_ColorObserver mli_ColorObserver_init(void);
void mli_ColorObserver_free(struct mli_ColorObserver *self);
int mli_ColorObserver_malloc_cie1931(struct mli_ColorObserver *self);
int mli_ColorObserver_evaluate(
        const struct mli_ColorObserver *self,
        const struct mli_Func *func,
        struct mli_Color *color);
int mli_ColorObserver_is_valid(const struct mli_ColorObserver *self);

int mli_Func_malloc_cie1931_spectral_matching_curve_x(struct mli_Func *self);
int mli_Func_malloc_cie1931_spectral_matching_curve_y(struct mli_Func *self);
int mli_Func_malloc_cie1931_spectral_matching_curve_z(struct mli_Func *self);

struct mli_Mat mli_Mat_cie1931_spectral_matching_xyz_to_rgb(void);
struct mli_Mat mli_ColorObserver_rgb_to_xyz(void);

int mli_Func_malloc_spectral_radiance_of_black_body_W_per_m2_per_sr_per_m(
        struct mli_Func *self,
        const double wavelength_start,
        const double wavelength_stop,
        const double temperature,
        const uint64_t num_points);
int mli_Func_malloc_spectral_irradiance_of_sky_at_sealevel_W_per_m2_per_m(
        struct mli_Func *self);

#endif
