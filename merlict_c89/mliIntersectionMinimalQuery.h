/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MERLICT_C89_MLIINTERSECTIONMINIMALQUERY_H_
#define MERLICT_C89_MLIINTERSECTIONMINIMALQUERY_H_

#include "mliVec.h"
#include "mliGeometryId.h"

struct mliIntersectionMinimalQuery {
        struct mliGeometryId geometry_id;
        struct mliVec position_local;
        double distance_of_ray;
};

struct mliIntersectionMinimalQuery mliIntersectionMinimalQuery_init(void);
#endif
