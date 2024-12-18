/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLI_INTERSECTION_AND_SCENERY_H_
#define MLI_INTERSECTION_AND_SCENERY_H_

#include "../intersection/intersection_surface_normal.h"
#include "../scenery/scenery.h"

const struct mli_Func *mli_get_refractive_index_coming_from(
        const struct mli_Scenery *scenery,
        const struct mli_IntersectionSurfaceNormal *isec);
const struct mli_Func *mli_get_refractive_index_going_to(
        const struct mli_Scenery *scenery,
        const struct mli_IntersectionSurfaceNormal *isec);
struct mli_boundarylayer_Side mli_get_side_going_to(
        const struct mli_Scenery *scenery,
        const struct mli_IntersectionSurfaceNormal *isec);
struct mli_boundarylayer_Side mli_get_side_coming_from(
        const struct mli_Scenery *scenery,
        const struct mli_IntersectionSurfaceNormal *isec);
#endif
