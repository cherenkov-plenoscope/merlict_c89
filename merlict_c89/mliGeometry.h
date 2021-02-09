/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MERLICT_C89_MLIGEOMETRY_H_
#define MERLICT_C89_MLIGEOMETRY_H_

#include <stdint.h>
#include "mliHomTra.h"
#include "mliBoundaryLayer.h"
#include "mliMaterials.h"

struct mliGeometry {
        uint32_t num_objects;
        struct mliObject *objects;

        uint32_t num_robjects;
        uint32_t *robjects;
        uint32_t *robject_ids;
        struct mliBoundaryLayer *robject_boundary_layers;
        struct mliHomTraComp *robject2root;
};

int mliGeometry_malloc(
        struct mliGeometry *scenery,
        const uint32_t num_objects,
        const uint32_t num_robjects);
void mliGeometry_free(struct mliGeometry *scenery);
struct mliGeometry mliGeometry_init(void);
void mliGeometry_info_fprint(FILE *f, const struct mliGeometry *scenery);
struct mliBoundaryLayer mliGeometry_object_surfaces(
        const struct mliGeometry *scenery,
        const uint32_t object_idx);
#endif
