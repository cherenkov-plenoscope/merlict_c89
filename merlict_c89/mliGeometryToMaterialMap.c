/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "mliGeometryToMaterialMap.h"
#include <math.h>
#include <stdint.h>
#include "mli_debug.h"

struct mliGeometryToMaterialMap mliGeometryToMaterialMap_init(void)
{
        struct mliGeometryToMaterialMap map;
        map.num_robjects = 0u;
        map.total_num_boundary_layers = 0u;
        map.boundary_layers = NULL;
        map.first_boundary_layer_in_robject = NULL;
        return map;
}

void mliGeometryToMaterialMap_free(struct mliGeometryToMaterialMap *map)
{
        free(map->first_boundary_layer_in_robject);
        free(map->boundary_layers);
        (*map) = mliGeometryToMaterialMap_init();
}

int mliGeometryToMaterialMap_malloc(
        struct mliGeometryToMaterialMap *map,
        const uint32_t num_robjects,
        const uint32_t total_num_boundary_layers)
{
        mliGeometryToMaterialMap_free(map);
        map->num_robjects = num_robjects;
        map->total_num_boundary_layers = total_num_boundary_layers;

        chk_malloc(
                map->boundary_layers, uint32_t, map->total_num_boundary_layers);
        chk_malloc(
                map->first_boundary_layer_in_robject,
                uint32_t,
                map->num_robjects);
        MLI_ZEROS(map->first_boundary_layer_in_robject, map->num_robjects);
        MLI_ZEROS(map->boundary_layers, map->total_num_boundary_layers);
        return 1;
error:
        return 0;
}

uint32_t _mliGeometryToMaterialMap_resolve_idx(
        const struct mliGeometryToMaterialMap *map,
        const uint32_t robject_idx,
        const uint32_t material_idx)
{
        uint32_t idx = map->first_boundary_layer_in_robject[robject_idx];
        idx += material_idx;
        return idx;
}

uint32_t mliGeometryToMaterialMap_get(
        const struct mliGeometryToMaterialMap *map,
        const uint32_t robject_idx,
        const uint32_t material_idx)
{
        return map->boundary_layers[_mliGeometryToMaterialMap_resolve_idx(
                map, robject_idx, material_idx)];
}

void mliGeometryToMaterialMap_set(
        const struct mliGeometryToMaterialMap *map,
        const uint32_t robject_idx,
        const uint32_t material_idx,
        const uint32_t boundary_layer_idx)
{
        map->boundary_layers[_mliGeometryToMaterialMap_resolve_idx(
                map, robject_idx, material_idx)] = boundary_layer_idx;
}

uint32_t mliGeometryToMaterialMap_num_boundary_layers_in_robject(
        const struct mliGeometryToMaterialMap *map,
        const uint32_t robject_idx)
{
        const uint32_t start =
                _mliGeometryToMaterialMap_resolve_idx(map, robject_idx, 0u);
        uint32_t end = start;
        if (robject_idx + 1 < map->num_robjects) {
                end = _mliGeometryToMaterialMap_resolve_idx(
                        map, robject_idx + 1, 0u);
        } else {
                end = map->total_num_boundary_layers;
        }
        return end - start;
}

void mliGeometryToMaterialMap_info_fprint(
        FILE *f,
        const struct mliGeometryToMaterialMap *map)
{
        uint32_t robj, bdl, bdl_start, num_bdls;
        fprintf(f, "Geometry to material map:\n");
        fprintf(f, "%*s", 4, "");
        fprintf(f, " ref    boundary-layers\n");
        fprintf(f, "%*s", 4, "");
        fprintf(f, "--------------------------------\n");
        for (robj = 0u; robj < map->num_robjects; robj++) {
                bdl_start =
                        _mliGeometryToMaterialMap_resolve_idx(map, robj, 0u);
                num_bdls =
                        mliGeometryToMaterialMap_num_boundary_layers_in_robject(
                                map, robj);
                fprintf(f, "%*s", 4, "");
                fprintf(f, "% 4d  [", robj);
                for (bdl = 0u; bdl < num_bdls; bdl++) {
                        fprintf(f,
                                "% 2d, ",
                                map->boundary_layers[bdl_start + bdl]);
                }
                fprintf(f, "]\n");
        }
}
