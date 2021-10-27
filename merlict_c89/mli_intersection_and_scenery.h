/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLI_INTERSECTION_AND_SCENERY_H_
#define MLI_INTERSECTION_AND_SCENERY_H_

#include "mliIntersectionSurfaceNormal.h"
#include "mliScenery.h"

const struct mliFunc *mli_get_refractive_index_coming_from(
        const struct mliScenery *scenery,
        const struct mliIntersectionSurfaceNormal *isec);
const struct mliFunc *mli_get_refractive_index_going_to(
        const struct mliScenery *scenery,
        const struct mliIntersectionSurfaceNormal *isec);
struct mliSide mli_get_side_going_to(
        const struct mliScenery *scenery,
        const struct mliIntersectionSurfaceNormal *isec);
struct mliSide mli_get_side_coming_from(
        const struct mliScenery *scenery,
        const struct mliIntersectionSurfaceNormal *isec);
#endif
