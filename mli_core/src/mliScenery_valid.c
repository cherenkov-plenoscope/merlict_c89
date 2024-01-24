/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "mliScenery_valid.h"
#include "mliMaterials_valid.h"
#include "mliGeometry_valid.h"
#include "mliAccelerator_valid.h"
#include "mliGeometryToMaterialMap_valid.h"
#include "../../chk_debug/src/chk_debug.h"

int mliScenery_valid(const struct mliScenery *scenery)
{
        /* check in itself */
        chk_msg(mliMaterials_valid(&scenery->materials),
                "Expected materials to be valid.");
        chk_msg(mliGeometry_valid(&scenery->geometry),
                "Expected geometry to be valid.");
        chk_msg(mliAccelerator_valid(&scenery->accelerator),
                "Expected accelerator to be valid");
        chk_msg(mliGeometryToMaterialMap_valid(&scenery->geomap),
                "Expected geometry-to-materials-map to be valid.");

        /* check interplay */
        chk_msg(mliAccelerator_valid_wrt_Geometry(
                        &scenery->accelerator, &scenery->geometry),
                "Expected accelerator to be valid w.r.t. geometry.");
        chk_msg(mliGeometryToMaterialMap_valid_wrt_Geometry(
                        &scenery->geomap, &scenery->geometry),
                "Expected geomap to be valid w.r.t. geometry.");
        chk_msg(mliGeometryToMaterialMap_valid_wrt_Materials(
                        &scenery->geomap, &scenery->materials),
                "Expected geomap to be valid w.r.t. materials.");
        return 1;
error:
        return 0;
}
