/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLIINTERSECTION_H_
#define MLIINTERSECTION_H_

#include "../vec/vec.h"
#include "mliGeometryId.h"

struct mliIntersection {
        struct mliGeometryId geometry_id;
        struct mli_Vec position_local;
        double distance_of_ray;
};

struct mliIntersection mliIntersection_init(void);
#endif
