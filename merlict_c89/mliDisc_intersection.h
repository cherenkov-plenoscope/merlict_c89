/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MERLICT_C89_MLIDISC_INTERSECTION_H_
#define MERLICT_C89_MLIDISC_INTERSECTION_H_

#include "mliVec.h"
#include "mliRay.h"
#include "mliHomTra.h"
#include "mliIntersection.h"
#include "mliDisc.h"
#include "mli_math.h"
#include "mli_xyplane.h"
#include "mli_cylindrical_prism.h"
#include "mli_from_outside_to_inside.h"

int mliDisc_intersection(
        const struct mliDisc disc,
        const struct mliHomTraComp local2root_comp,
        const struct mliRay ray,
        struct mliIntersection *intersection);
#endif
