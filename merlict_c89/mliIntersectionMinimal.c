/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "mliIntersectionMinimal.h"

struct mliIntersectionMinimal mliIntersectionMinimal_init(void)
{
        struct mliIntersectionMinimal psec;
        psec.geometry_id = mliGeometryId_init();
        psec.position_local = mliVec_set(0.0, 0.0, 0.0);
        psec.distance_of_ray = DBL_MAX;
        return psec;
}
