/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MERLICT_C89_MLISPHERICALCAPHEX_INTERSECTION_H_
#define MERLICT_C89_MLISPHERICALCAPHEX_INTERSECTION_H_

#include <math.h>

#include "mli_math.h"
#include "mliVec.h"
#include "mliRay.h"
#include "mliHomTra.h"
#include "mliHexagonalPrismZ.h"
#include "mliOuterPrismBound.h"
#include "mliIntersection.h"
#include "mli_spherical_cap.h"
#include "mli_from_outside_to_inside.h"

int mliSphericalCapHex_intersection(
        const struct mliSphericalCapHex cap,
        const struct mliHomTraComp local2root_comp,
        const struct mliRay ray,
        struct mliIntersection *intersection);
#endif
