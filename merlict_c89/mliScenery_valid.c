/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "mliScenery_valid.h"
#include "mliMaterials_valid.h"
#include "mliGeometry_valid.h"
#include "mliGeometryToMaterialMap_valid.h"
#include "mli_debug.h"

int mliScenery_valid(const struct mliScenery *scenery)
{
        mli_check(
                mliMaterials_valid(&scenery->materials),
                "Expected materials to be valid.");
        mli_check(
                mliGeometry_valid(&scenery->geometry),
                "Expected geometry to be valid.");
        mli_check(
                mliGeometryToMaterialMap_valid(&scenery->geomap),
                "Expected geometry-to-materials-map to be valid.");
        return 1;
error:
        return 0;
}
