/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLIGEOMETRY_EQUAL_H_
#define MLIGEOMETRY_EQUAL_H_

#include "../bool/bool.h"
#include "geometry.h"

mli_bool mli_Geometry_equal(
        const struct mli_Geometry *a,
        const struct mli_Geometry *b);
mli_bool mli_Geometry_objects_equal(
        const struct mli_Geometry *a,
        const struct mli_Geometry *b);
mli_bool mli_Geometry_object_references_equal(
        const struct mli_Geometry *a,
        const struct mli_Geometry *b);
#endif
