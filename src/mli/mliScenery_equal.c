/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "mliScenery_equal.h"
#include "../materials/materials_equal.h"
#include "../geometry/geometry_equal.h"
#include "mliAccelerator_equal.h"
#include "mliGeometryToMaterialMap_equal.h"
#include "../chk/chk.h"

int mliScenery_equal(const struct mliScenery *a, const struct mliScenery *b)
{
        chk_msg(mliGeometry_equal(&a->geometry, &b->geometry),
                "Expected geometry to be valid.");
        chk_msg(mli_Materials_equal(&a->materials, &b->materials),
                "Expected materials to be valid.");
        chk_msg(mliAccelerator_equal(&a->accelerator, &b->accelerator),
                "Expected accelerator to be valid.");
        chk_msg(mliGeometryToMaterialMap_equal(&a->geomap, &b->geomap),
                "Expected geomap to be valid.");
        return 1;
chk_error:
        return 0;
}
