/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLI_INTERSECTION_H_
#define MLI_INTERSECTION_H_

#include "../vec/vec.h"
#include "../geometry/geometry_id.h"

struct mliIntersection {
        struct mli_GeometryId geometry_id;
        struct mli_Vec position_local;
        double distance_of_ray;
};

struct mliIntersection mliIntersection_init(void);
#endif
