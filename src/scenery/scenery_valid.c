/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "scenery_valid.h"
#include "../materials/materials_valid.h"
#include "../geometry/geometry_valid.h"
#include "../accelerator/accelerator_valid.h"
#include "../mli/mliGeometryToMaterialMap_valid.h"
#include "../chk/chk.h"

int mliScenery_valid(const struct mliScenery *scenery)
{
        /* check in itself */
        chk_msg(mli_Materials_valid(&scenery->materials),
                "Expected materials to be valid.");
        chk_msg(mli_Geometry_valid(&scenery->geometry),
                "Expected geometry to be valid.");
        chk_msg(mli_Accelerator_valid(&scenery->accelerator),
                "Expected accelerator to be valid");
        chk_msg(mli_GeometryToMaterialMap_valid(&scenery->geomap),
                "Expected geometry-to-materials-map to be valid.");

        /* check interplay */
        chk_msg(mli_Accelerator_valid_wrt_Geometry(
                        &scenery->accelerator, &scenery->geometry),
                "Expected accelerator to be valid w.r.t. geometry.");
        chk_msg(mli_GeometryToMaterialMap_valid_wrt_Geometry(
                        &scenery->geomap, &scenery->geometry),
                "Expected geomap to be valid w.r.t. geometry.");
        chk_msg(mli_GeometryToMaterialMap_valid_wrt_Materials(
                        &scenery->geomap, &scenery->materials),
                "Expected geomap to be valid w.r.t. materials.");
        return 1;
chk_error:
        return 0;
}
