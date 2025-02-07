/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLI_GEOMETRYTOMATERIALMAP_H_
#define MLI_GEOMETRYTOMATERIALMAP_H_

#include <stdint.h>
#include "../chk/chk.h"
#include "../geometry/geometry.h"
#include "../materials/materials.h"

struct mli_GeometryToMaterialMap {
        uint32_t num_robjects;
        uint32_t total_num_boundary_layers;
        uint32_t *boundary_layers;
        uint32_t *first_boundary_layer_in_robject;
};

struct mli_GeometryToMaterialMap mli_GeometryToMaterialMap_init(void);
chk_rc mli_GeometryToMaterialMap_malloc(
        struct mli_GeometryToMaterialMap *map,
        const uint32_t num_robjects,
        const uint32_t total_num_boundary_layers);
void mli_GeometryToMaterialMap_free(struct mli_GeometryToMaterialMap *map);

uint32_t mli_GeometryToMaterialMap_resolve_idx(
        const struct mli_GeometryToMaterialMap *map,
        const uint32_t robject_idx,
        const uint32_t material_idx);

uint32_t mli_GeometryToMaterialMap_get(
        const struct mli_GeometryToMaterialMap *map,
        const uint32_t robject_idx,
        const uint32_t material_idx);
void mli_GeometryToMaterialMap_set(
        const struct mli_GeometryToMaterialMap *map,
        const uint32_t robject_idx,
        const uint32_t material_idx,
        const uint32_t boundary_layer_idx);

uint32_t mli_GeometryToMaterialMap_num_boundary_layers_in_robject(
        const struct mli_GeometryToMaterialMap *map,
        const uint32_t robject_idx);

void mli_GeometryToMaterialMap_info_fprint(
        FILE *f,
        const struct mli_GeometryToMaterialMap *map);
#endif
