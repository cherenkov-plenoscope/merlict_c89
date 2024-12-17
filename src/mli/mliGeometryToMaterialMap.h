/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLIGEOMETRYTOMATERIALMAP_H_
#define MLIGEOMETRYTOMATERIALMAP_H_

#include <stdint.h>
#include "mliGeometry.h"
#include "../materials/materials.h"

struct mliGeometryToMaterialMap {
        uint32_t num_robjects;
        uint32_t total_num_boundary_layers;
        uint32_t *boundary_layers;
        uint32_t *first_boundary_layer_in_robject;
};

struct mliGeometryToMaterialMap mliGeometryToMaterialMap_init(void);
int mliGeometryToMaterialMap_malloc(
        struct mliGeometryToMaterialMap *map,
        const uint32_t num_robjects,
        const uint32_t total_num_boundary_layers);
void mliGeometryToMaterialMap_free(struct mliGeometryToMaterialMap *map);

uint32_t mliGeometryToMaterialMap_resolve_idx(
        const struct mliGeometryToMaterialMap *map,
        const uint32_t robject_idx,
        const uint32_t material_idx);

uint32_t mliGeometryToMaterialMap_get(
        const struct mliGeometryToMaterialMap *map,
        const uint32_t robject_idx,
        const uint32_t material_idx);
void mliGeometryToMaterialMap_set(
        const struct mliGeometryToMaterialMap *map,
        const uint32_t robject_idx,
        const uint32_t material_idx,
        const uint32_t boundary_layer_idx);

uint32_t mliGeometryToMaterialMap_num_boundary_layers_in_robject(
        const struct mliGeometryToMaterialMap *map,
        const uint32_t robject_idx);

void mliGeometryToMaterialMap_info_fprint(
        FILE *f,
        const struct mliGeometryToMaterialMap *map);
#endif
