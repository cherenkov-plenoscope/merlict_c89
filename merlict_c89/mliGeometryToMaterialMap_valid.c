/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "mliGeometryToMaterialMap_valid.h"
#include <stdint.h>
#include "mli_debug.h"

int mliGeometryToMaterialMap_valid(
        const struct mliGeometryToMaterialMap *geomap)
{
        uint32_t i = 0u;
        mli_check(geomap->total_num_boundary_layers >= geomap->num_robjects,
                "Expected at least as many boundary-layer-references as "
                "object-references.");
        for (i = 0u; i < geomap->num_robjects; i++) {
                mli_check(
                        geomap->first_boundary_layer_in_robject[i] <
                        geomap->total_num_boundary_layers,
                        "Expected all position of first_boundary_layer[i] < "
                        "total_num_boundary_layers");
        }
        return 1;
error:
        return 0;
}
