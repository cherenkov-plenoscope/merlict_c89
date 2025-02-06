/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "geometrytomaterialmap.h"
#include <math.h>
#include <stdint.h>
#include <stdlib.h>
#include "../chk/chk.h"
#include "../math/math.h"

struct mli_GeometryToMaterialMap mli_GeometryToMaterialMap_init(void)
{
        struct mli_GeometryToMaterialMap map;
        map.num_robjects = 0u;
        map.total_num_boundary_layers = 0u;
        map.boundary_layers = NULL;
        map.first_boundary_layer_in_robject = NULL;
        return map;
}

void mli_GeometryToMaterialMap_free(struct mli_GeometryToMaterialMap *map)
{
        free(map->first_boundary_layer_in_robject);
        free(map->boundary_layers);
        (*map) = mli_GeometryToMaterialMap_init();
}

int mli_GeometryToMaterialMap_malloc(
        struct mli_GeometryToMaterialMap *map,
        const uint32_t num_robjects,
        const uint32_t total_num_boundary_layers)
{
        mli_GeometryToMaterialMap_free(map);
        map->num_robjects = num_robjects;
        map->total_num_boundary_layers = total_num_boundary_layers;

        chk_malloc(
                map->boundary_layers, uint32_t, map->total_num_boundary_layers);
        chk_malloc(
                map->first_boundary_layer_in_robject,
                uint32_t,
                map->num_robjects);
        MLI_MATH_ARRAY_SET(
                map->first_boundary_layer_in_robject, 0, map->num_robjects);
        MLI_MATH_ARRAY_SET(
                map->boundary_layers, 0, map->total_num_boundary_layers);
        return CHK_SUCCESS;
chk_error:
        return CHK_FAIL;
}

uint32_t mli_GeometryToMaterialMap_resolve_idx(
        const struct mli_GeometryToMaterialMap *map,
        const uint32_t robject_idx,
        const uint32_t material_idx)
{
        uint32_t idx = map->first_boundary_layer_in_robject[robject_idx];
        idx += material_idx;
        return idx;
}

uint32_t mli_GeometryToMaterialMap_get(
        const struct mli_GeometryToMaterialMap *map,
        const uint32_t robject_idx,
        const uint32_t material_idx)
{
        return map->boundary_layers[mli_GeometryToMaterialMap_resolve_idx(
                map, robject_idx, material_idx)];
}

void mli_GeometryToMaterialMap_set(
        const struct mli_GeometryToMaterialMap *map,
        const uint32_t robject_idx,
        const uint32_t material_idx,
        const uint32_t boundary_layer_idx)
{
        map->boundary_layers[mli_GeometryToMaterialMap_resolve_idx(
                map, robject_idx, material_idx)] = boundary_layer_idx;
}

uint32_t mli_GeometryToMaterialMap_num_boundary_layers_in_robject(
        const struct mli_GeometryToMaterialMap *map,
        const uint32_t robject_idx)
{
        const uint32_t start =
                mli_GeometryToMaterialMap_resolve_idx(map, robject_idx, 0u);
        uint32_t end = start;
        if (robject_idx + 1 < map->num_robjects) {
                end = mli_GeometryToMaterialMap_resolve_idx(
                        map, robject_idx + 1, 0u);
        } else {
                end = map->total_num_boundary_layers;
        }
        return end - start;
}

void mli_GeometryToMaterialMap_info_fprint(
        FILE *f,
        const struct mli_GeometryToMaterialMap *map)
{
        uint32_t robj, bdl, bdl_start, num_bdls, i;
        fprintf(f, "    geometry to material map\n");
        fprintf(f, "    ");
        for (i = 0; i < 70; i++) {
                fprintf(f, "-");
        }
        fprintf(f, "\n");
        fprintf(f, "    ");
        fprintf(f, "%5s ", "ref");
        fprintf(f, "%24s ", "boundary-layers");
        fprintf(f, "\n");
        fprintf(f, "    ");
        for (i = 0; i < 70; i++) {
                fprintf(f, "-");
        }
        fprintf(f, "\n");

        for (robj = 0u; robj < map->num_robjects; robj++) {
                bdl_start =
                        mli_GeometryToMaterialMap_resolve_idx(map, robj, 0u);
                num_bdls =
                        mli_GeometryToMaterialMap_num_boundary_layers_in_robject(
                                map, robj);
                fprintf(f, "    ");
                fprintf(f, "% 5d ", robj);
                fprintf(f, "  [");
                for (bdl = 0u; bdl < num_bdls; bdl++) {
                        fprintf(f,
                                "%d,",
                                map->boundary_layers[bdl_start + bdl]);
                }
                fprintf(f, "]\n");
        }
}
