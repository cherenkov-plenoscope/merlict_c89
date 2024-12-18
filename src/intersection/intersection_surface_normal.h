/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLI_INTERSECTION_SURFACE_NORMAL_H_
#define MLI_INTERSECTION_SURFACE_NORMAL_H_

#include <stdint.h>
#include "../vec/vec.h"
#include "../geometry/geometry_id.h"

struct mliIntersectionSurfaceNormal {
        struct mli_GeometryId geometry_id;
        struct mli_Vec position;
        struct mli_Vec surface_normal;
        struct mli_Vec position_local;
        struct mli_Vec surface_normal_local;
        double distance_of_ray;
        int64_t from_outside_to_inside;
};

struct mliIntersectionSurfaceNormal mliIntersectionSurfaceNormal_init(void);

#endif
