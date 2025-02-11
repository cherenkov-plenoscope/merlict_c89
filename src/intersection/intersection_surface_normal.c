/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "intersection_surface_normal.h"
#include <float.h>

struct mli_IntersectionSurfaceNormal mli_IntersectionSurfaceNormal_init(void)
{
        struct mli_IntersectionSurfaceNormal isec;
        isec.geometry_id = mli_GeometryId_init();
        isec.position = mli_Vec_set(0.0, 0.0, 0.0);
        isec.surface_normal = mli_Vec_set(0.0, 0.0, 1.0);
        isec.position_local = mli_Vec_set(0.0, 0.0, 0.0);
        isec.surface_normal_local = mli_Vec_set(0.0, 0.0, 1.0);
        isec.distance_of_ray = DBL_MAX;
        isec.from_outside_to_inside = 1;
        return isec;
}
