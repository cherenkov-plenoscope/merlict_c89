/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MERLICT_C89_MLITRACER_H_
#define MERLICT_C89_MLITRACER_H_

#include "mliCombine.h"
#include "mliIntersection.h"

struct mliColor mli_trace(
        const struct mliCombine *combine,
        const struct mliRay ray);
#endif
