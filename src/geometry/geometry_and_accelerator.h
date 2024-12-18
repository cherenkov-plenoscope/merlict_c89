/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLI_GEOMETRY_AND_ACCELERATOR_H_
#define MLI_GEOMETRY_AND_ACCELERATOR_H_

#include "../geometry/geometry.h"
#include "../accelerator/accelerator.h"

struct mli_GeometryAndAccelerator {
        /*
         * A temporary container to allow access to both geometry and its
         * accelerator using only one pointer.
         */
        const struct mli_Geometry *geometry;
        const struct mli_Accelerator *accelerator;
};

#endif
