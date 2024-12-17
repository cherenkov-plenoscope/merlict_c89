/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "scenery_equal.h"
#include "../materials/materials_equal.h"
#include "../geometry/geometry_equal.h"
#include "../accelerator/accelerator_equal.h"
#include "../mli/mliGeometryToMaterialMap_equal.h"
#include "../chk/chk.h"

int mli_Scenery_equal(const struct mli_Scenery *a, const struct mli_Scenery *b)
{
        chk_msg(mli_Geometry_equal(&a->geometry, &b->geometry),
                "Expected geometry to be valid.");
        chk_msg(mli_Materials_equal(&a->materials, &b->materials),
                "Expected materials to be valid.");
        chk_msg(mli_Accelerator_equal(&a->accelerator, &b->accelerator),
                "Expected accelerator to be valid.");
        chk_msg(mli_GeometryToMaterialMap_equal(&a->geomap, &b->geomap),
                "Expected geomap to be valid.");
        return 1;
chk_error:
        return 0;
}
