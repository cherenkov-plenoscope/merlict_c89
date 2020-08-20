/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MERLICT_C89_MLI_BARYCENTRIC_H_
#define MERLICT_C89_MLI_BARYCENTRIC_H_

#include "mliVec.h"

struct mliVec mli_barycentric(
        const struct mliVec a,
        const struct mliVec b,
        const struct mliVec c,
        const struct mliVec t);
#endif
