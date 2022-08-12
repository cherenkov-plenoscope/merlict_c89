/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "mliIntersection.h"

struct mliIntersection mliIntersection_init(void)
{
        struct mliIntersection psec;
        psec.geometry_id = mliGeometryId_init();
        psec.position_local = mliVec_init(0.0, 0.0, 0.0);
        psec.distance_of_ray = DBL_MAX;
        return psec;
}
