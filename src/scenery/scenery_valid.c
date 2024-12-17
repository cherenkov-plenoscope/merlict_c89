/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "scenery_valid.h"
#include "../materials/materials_valid.h"
#include "../geometry/geometry_valid.h"
#include "../accelerator/accelerator_valid.h"
#include "../geometrytomaterialmap/geometrytomaterialmap_valid.h"
#include "../chk/chk.h"

int mli_Scenery_valid(const struct mli_Scenery *self)
{
        /* check in itself */
        chk_msg(mli_Materials_valid(&self->materials),
                "Expected materials to be valid.");
        chk_msg(mli_Geometry_valid(&self->geometry),
                "Expected geometry to be valid.");
        chk_msg(mli_Accelerator_valid(&self->accelerator),
                "Expected accelerator to be valid");
        chk_msg(mli_GeometryToMaterialMap_valid(&self->geomap),
                "Expected geometry-to-materials-map to be valid.");

        /* check interplay */
        chk_msg(mli_Accelerator_valid_wrt_Geometry(
                        &self->accelerator, &self->geometry),
                "Expected accelerator to be valid w.r.t. geometry.");
        chk_msg(mli_GeometryToMaterialMap_valid_wrt_Geometry(
                        &self->geomap, &self->geometry),
                "Expected geomap to be valid w.r.t. geometry.");
        chk_msg(mli_GeometryToMaterialMap_valid_wrt_Materials(
                        &self->geomap, &self->materials),
                "Expected geomap to be valid w.r.t. materials.");
        return 1;
chk_error:
        return 0;
}
