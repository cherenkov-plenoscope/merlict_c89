/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MERLICT_C89_MLIEDGE_H_
#define MERLICT_C89_MLIEDGE_H_

#include "mliRay.h"

struct mliEdge {
        struct mliRay ray;
        double length;
};

#endif
