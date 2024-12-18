/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "intersection.h"
#include <float.h>

struct mli_Intersection mli_Intersection_init(void)
{
        struct mli_Intersection psec;
        psec.geometry_id = mli_GeometryId_init();
        psec.position_local = mli_Vec_init(0.0, 0.0, 0.0);
        psec.distance_of_ray = DBL_MAX;
        return psec;
}
