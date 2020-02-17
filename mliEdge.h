/* Copyright 2019 Sebastian Achim Mueller */
#ifndef MERLICT_MLIEDGE_H_
#define MERLICT_MLIEDGE_H_

#include "mliRay.h"

struct mliEdge {
    struct mliRay ray;
    double length;
};

#endif
