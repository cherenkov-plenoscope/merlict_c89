/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MERLICT_C89_MLIGEOMETRY_H_
#define MERLICT_C89_MLIGEOMETRY_H_

#include <stdint.h>
#include "mliHomTra.h"
#include "mliMaterials.h"
#include "mliName.h"

struct mliGeometry {
        uint32_t num_objects;
        struct mliObject *objects;
        struct mliName *object_names;

        uint32_t num_robjects;
        uint32_t *robjects;
        uint32_t *robject_ids;
        struct mliHomTraComp *robject2root;
};

int mliGeometry_malloc(
        struct mliGeometry *geometry,
        const uint32_t num_objects,
        const uint32_t num_robjects);
int mliGeometry_malloc_references(
        struct mliGeometry *geometry,
        const uint32_t num_robjects);
int mliGeometry_malloc_objects(
        struct mliGeometry *geometry,
        const uint32_t num_objects);

void mliGeometry_free(struct mliGeometry *geometry);
void mliGeometry_free_objects(struct mliGeometry *geometry);
void mliGeometry_free_references(struct mliGeometry *geometry);

struct mliGeometry mliGeometry_init(void);
void mliGeometry_init_objects(struct mliGeometry *geometry);
void mliGeometry_init_references(struct mliGeometry *geometry);

void mliGeometry_info_fprint(FILE *f, const struct mliGeometry *geometry);
struct mliBoundaryLayer mliGeometry_object_surfaces(
        const struct mliGeometry *geometry,
        const uint32_t object_idx);

#endif
