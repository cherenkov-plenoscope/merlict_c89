/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "mliIntersectionSurfaceNormal.h"

struct mliIntersectionSurfaceNormal mliIntersectionSurfaceNormal_init(void)
{
        struct mliIntersectionSurfaceNormal isec;
        isec.geometry_id = mliGeometryId_init();
        isec.position = mliVec_set(0.0, 0.0, 0.0);
        isec.surface_normal = mliVec_set(0.0, 0.0, 1.0);
        isec.position_local = mliVec_set(0.0, 0.0, 0.0);
        isec.surface_normal_local = mliVec_set(0.0, 0.0, 1.0);
        isec.distance_of_ray = DBL_MAX;
        isec.from_outside_to_inside = 1;
        return isec;
}
