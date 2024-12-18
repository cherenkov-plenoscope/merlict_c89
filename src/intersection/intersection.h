/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLI_INTERSECTION_H_
#define MLI_INTERSECTION_H_

#include "../vec/vec.h"
#include "../geometry/geometry_id.h"

struct mli_Intersection {
        struct mli_GeometryId geometry_id;
        struct mli_Vec position_local;
        double distance_of_ray;
};

struct mli_Intersection mli_Intersection_init(void);
#endif
