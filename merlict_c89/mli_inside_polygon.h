/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MERLICT_C89_MLI_INSIDE_POLYGON_H_
#define MERLICT_C89_MLI_INSIDE_POLYGON_H_

#include <math.h>
#include <stdint.h>

#include "mliFunc.h"

int mli_inside_polygon(
        const struct mliFunc *polygon,
        const double x,
        const double y);
#endif
