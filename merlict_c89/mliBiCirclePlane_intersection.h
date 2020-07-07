/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MERLICT_C89_MLIBICIRCLEPLANE_INTERSECTION_H_
#define MERLICT_C89_MLIBICIRCLEPLANE_INTERSECTION_H_

#include "mli_math.h"
#include "mliVec.h"
#include "mliRay.h"
#include "mliHomTra.h"
#include "mliIntersection.h"
#include "mliBiCirclePlane.h"
#include "mli_xyplane.h"
#include "mli_from_outside_to_inside.h"
#include "mli_dual_circle_prism.h"

int mliBiCirclePlane_intersection(
        const struct mliBiCirclePlane plane,
        const struct mliHomTraComp local2root_comp,
        const struct mliRay ray,
        struct mliIntersection *intersection);
#endif
