/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLICOLOROBSERVER_H_
#define MLICOLOROBSERVER_H_

#include "../color/color.h"
#include "../func/func.h"

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

#endif
