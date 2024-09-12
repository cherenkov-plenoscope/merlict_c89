/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLICOLOROBSERVER_H_
#define MLICOLOROBSERVER_H_

#include "mliFunc.h"

struct mliColorObserver {
        /* Color detection efficiency by wavelength.
         * The color observer has three color channels (r,g,b).
         * The functions r, g and b define how these channels respond to a
         * specific wavelength of light.
         */
        struct mliFunc r;
        struct mliFunc g;
        struct mliFunc b;
};

struct mliColorObserver mliColorObserver_init(void);
void mliColorObserver_free(struct mliColorObserver *colobs);
int mliColorObserver_malloc_cie1931(struct mliColorObserver *colobs);
#endif
