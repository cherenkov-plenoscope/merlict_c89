/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLICOLOROBSERVER_H_
#define MLICOLOROBSERVER_H_

#include "../color/color.h"
#include "../func/func.h"

struct mli_ColorObserver {
        /* Color detection efficiency by wavelength.
         * The color observer has three color channels (r,g,b).
         * The functions r, g and b define how these channels respond to a
         * specific wavelength of light.
         */
        struct mli_Func r;
        struct mli_Func g;
        struct mli_Func b;
};

struct mli_ColorObserver mli_ColorObserver_init(void);
void mli_ColorObserver_free(struct mli_ColorObserver *colobs);
int mli_ColorObserver_malloc_cie1931(struct mli_ColorObserver *colobs);
int mli_ColorObserver_evaluate(
        const struct mli_ColorObserver *colobs,
        const struct mli_Func *func,
        struct mli_Color *color);
int mli_ColorObserver_is_valid(const struct mli_ColorObserver *colobs);
#endif
