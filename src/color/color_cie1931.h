/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLICOLOR_CIE1931_H_
#define MLICOLOR_CIE1931_H_

#include "../func/func.h"
#include "../mat/mat.h"
#include <stdint.h>

/* Color detection efficiency by wavelength.
 * The color observer has three color channels (x,y,z).
 * The functions x,y,z define how these channels respond to a
 * specific wavelength of light.
 */

int mli_cie1931_spectral_matching_curve_x(struct mli_Func *self);
int mli_cie1931_spectral_matching_curve_y(struct mli_Func *self);
int mli_cie1931_spectral_matching_curve_z(struct mli_Func *self);

struct mli_Mat mli_cie1931_spectral_matching_xyz_to_rgb(void);
struct mli_Mat mli_cie1931_spectral_matching_rgb_to_xyz(void);

int mli_cie1931_spectral_radiance_of_black_body_W_per_m2_per_sr_per_m(
        struct mli_Func *self,
        const double wavelength_start,
        const double wavelength_stop,
        const double temperature,
        const uint64_t num_points);

#endif
