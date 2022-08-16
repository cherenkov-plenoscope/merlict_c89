/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLIGEOMETRYANDACCELERATOR_H_
#define MLIGEOMETRYANDACCELERATOR_H_

#include "mliGeometry.h"
#include "mliAccelerator.h"

struct mliGeometryAndAccelerator {
        /*
         * A temporary container to allow access to both geometry and its
         * accelerator using only one pointer.
         */
        const struct mliGeometry *geometry;
        const struct mliAccelerator *accelerator;
};

#endif
