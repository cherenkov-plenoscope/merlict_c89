/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MERLICT_C89_mliIntersectionSurfaceNormal_H_
#define MERLICT_C89_mliIntersectionSurfaceNormal_H_

#include "mliVec.h"
#include "mliGeometryId.h"

struct mliIntersectionSurfaceNormal {
        struct mliGeometryId geometry_id;
        struct mliVec position;
        struct mliVec surface_normal;
        struct mliVec position_local;
        struct mliVec surface_normal_local;
        double distance_of_ray;
        int from_outside_to_inside;
};

struct mliIntersectionSurfaceNormal mliIntersectionSurfaceNormal_init(void);

#endif
