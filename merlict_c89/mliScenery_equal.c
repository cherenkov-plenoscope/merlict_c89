/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "mliScenery_equal.h"
#include "mliMaterials_equal.h"
#include "mliGeometry_equal.h"
#include "mliAccelerator_equal.h"
#include "mliGeometryToMaterialMap_equal.h"
#include "mli_debug.h"

int mliScenery_equal(const struct mliScenery *a, const struct mliScenery *b)
{
        mli_check(
                mliGeometry_equal(&a->geometry, &b->geometry),
                "Expected geometry to be valid.");
        mli_check(
                mliMaterials_equal(&a->materials, &b->materials),
                "Expected materials to be valid.");
        mli_check(
                mliAccelerator_equal(&a->accelerator, &b->accelerator),
                "Expected accelerator to be valid.");
        mli_check(
                mliGeometryToMaterialMap_equal(&a->geomap, &b->geomap),
                "Expected geomap to be valid.");
        return 1;
error:
        return 0;
}
