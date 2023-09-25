/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "mliGeometryToMaterialMap_equal.h"
#include <stdint.h>
#include "chk_debug.h"

int mliGeometryToMaterialMap_equal(
        const struct mliGeometryToMaterialMap *a,
        const struct mliGeometryToMaterialMap *b)
{
        uint32_t i = 0u;
        chk_msg(a->num_robjects == b->num_robjects,
                "Expected num_robjects to be equal.");
        chk_msg(a->total_num_boundary_layers == b->total_num_boundary_layers,
                "Expected total_num_boundary_layers to be equal.");
        for (i = 0u; i < a->total_num_boundary_layers; i++) {
                chk_msg(a->boundary_layers[i] == b->boundary_layers[i],
                        "Expected all boundary_layers to be equal.");
        }
        for (i = 0u; i < a->num_robjects; i++) {
                chk_msg(a->first_boundary_layer_in_robject[i] ==
                                b->first_boundary_layer_in_robject[i],
                        "Expected all first_boundary_layer_in_robject "
                        "to be equal.");
        }
        return 1;
error:
        return 0;
}
