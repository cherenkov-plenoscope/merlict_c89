/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "geometrytomaterialmap_valid.h"
#include <stdint.h>
#include "../chk/chk.h"

mli_bool mli_GeometryToMaterialMap_valid(
        const struct mli_GeometryToMaterialMap *geomap)
{
        uint32_t i = 0u;
        chk_msg(geomap->total_num_boundary_layers >= geomap->num_robjects,
                "Expected at least as many boundary-layer-references as "
                "object-references.");
        for (i = 0u; i < geomap->num_robjects; i++) {
                chk_msg(geomap->first_boundary_layer_in_robject[i] <
                                geomap->total_num_boundary_layers,
                        "Expected all position of first_boundary_layer[i] < "
                        "total_num_boundary_layers");
        }
        return CHK_SUCCESS;
chk_error:
        return CHK_FAIL;
}

mli_bool mli_GeometryToMaterialMap_valid_wrt_Geometry(
        const struct mli_GeometryToMaterialMap *geomap,
        const struct mli_Geometry *geometry)
{
        uint32_t robj = 0u;
        uint32_t total_num_boundary_layers = 0u;
        chk_msg(geomap->num_robjects == geometry->num_robjects,
                "Expected num_robjects to be equal in Geometry and GeoMtlMap.");

        for (robj = 0u; robj < geomap->num_robjects; robj++) {
                const uint32_t obj = geometry->robjects[robj];
                const uint32_t obj_num_materials =
                        geometry->objects[obj].num_materials;
                chk_msg(mli_GeometryToMaterialMap_num_boundary_layers_in_robject(
                                geomap, robj) == obj_num_materials,
                        "Expected robject to have same num boundary-layers.");
                total_num_boundary_layers += obj_num_materials;
        }
        chk_msg(total_num_boundary_layers == geomap->total_num_boundary_layers,
                "Expected total_num_boundary_layers to match the Geometry.");

        return CHK_SUCCESS;
chk_error:
        return CHK_FAIL;
}

mli_bool mli_GeometryToMaterialMap_valid_wrt_Materials(
        const struct mli_GeometryToMaterialMap *geomap,
        const struct mli_Materials *materials)
{
        uint32_t i = 0u;
        for (i = 0u; i < geomap->total_num_boundary_layers; i++) {
                chk_msg(geomap->boundary_layers[i] <
                                materials->boundary_layers.size,
                        "Expected geomap's boundary_layers[i] to refer to "
                        "a valid boundary_layer in Materials.");
        }
        return CHK_SUCCESS;
chk_error:
        return CHK_FAIL;
}
