/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLI_GEOMETRY_VALID_H_
#define MLI_GEOMETRY_VALID_H_

#include "../bool/bool.h"
#include "geometry.h"

mli_bool mli_Geometry_valid(const struct mli_Geometry *geometry);
mli_bool mli_Geometry_valid_objects(const struct mli_Geometry *geometry);
mli_bool mli_Geometry_valid_robjects_HomTras(
        const struct mli_Geometry *geometry);
mli_bool mli_Geometry_valid_object_references(
        const struct mli_Geometry *geometry);
#endif
