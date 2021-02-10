/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MERLICT_C89_MLI_INTERSECTION_AND_SCENERY_H_
#define MERLICT_C89_MLI_INTERSECTION_AND_SCENERY_H_

#include <stdint.h>
#include "mliIntersectionSurfaceNormal.h"
#include "mliGeometry.h"

const struct mliFunc *_mli_refractive_index_coming_from(
        const struct mliScenery *scenery,
        const struct mliIntersectionSurfaceNormal *isec);
const struct mliFunc *_mli_refractive_index_going_to(
        const struct mliScenery *scenery,
        const struct mliIntersectionSurfaceNormal *isec);
struct mliSide _mli_side_going_to(
        const struct mliScenery *scenery,
        const struct mliIntersectionSurfaceNormal *isec);
struct mliSide _mli_side_coming_from(
        const struct mliScenery *scenery,
        const struct mliIntersectionSurfaceNormal *isec);
#endif
