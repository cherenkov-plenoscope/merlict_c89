/* Copyright 2019 Sebastian Achim Mueller */
#ifndef MERLICT_MLISCENERY_H_
#define MERLICT_MLISCENERY_H_

#include <math.h>
#include <stdint.h>
#include "mliVec.h"
#include "mliHomTra.h"
#include "mliColor.h"
#include "mliFunc.h"
#include "mliMesh.h"
#include "mliSphericalCap.h"

typedef struct {
    uint32_t type;
    uint32_t idx;
} mliIdx;

typedef struct {
    uint32_t i_color;
    uint32_t i_reflection;
    uint32_t i_refraction;
    uint32_t i_absorbtion;
} mliSurface;

typedef struct {
    uint32_t num_functions;
    uint32_t num_colors;
    uint32_t num_surfaces;
    uint32_t num_meshes;
    uint32_t num_spherical_cap_hex_bound;
} mliSceneryCapacity;

typedef struct {
    uint32_t num_functions;
    mliFunc *functions;

    uint32_t num_colors;
    mliColor *colors;

    uint32_t num_surfaces;
    mliSurface *surfaces;

    uint32_t num_meshes;
    mliMesh *meshes;
    uint32_t *mesh_colors;

    uint32_t num_spherical_cap_hex_bound;
    mliSphericalCapHeagonal *spherical_cap_hex_bound;
} mliScenery;

void mliScenery_init(
    mliScenery* scenery,
    const mliSceneryCapacity capacity) {
    scenery->num_functions = capacity.num_functions;
    scenery->functions = (mliFunc*)malloc(
        scenery->num_functions*sizeof(mliFunc));

    scenery->num_colors = capacity.num_colors;
    scenery->colors = (mliColor*)malloc(
        scenery->num_colors*sizeof(mliColor));

    scenery->num_meshes = capacity.num_meshes;
    scenery->meshes = (mliMesh*)malloc(
        scenery->num_meshes*sizeof(mliMesh));

    scenery->mesh_colors = (uint32_t*)malloc(
        scenery->num_meshes*sizeof(uint32_t));

    scenery->num_spherical_cap_hex_bound =
        capacity.num_spherical_cap_hex_bound;
    scenery->spherical_cap_hex_bound = (mliSphericalCapHeagonal*)malloc(
        scenery->num_spherical_cap_hex_bound*
        sizeof(mliSphericalCapHeagonal));
}


void mliScenery_free(mliScenery *scenery) {
    int i;
    for (i = 0; i < scenery->num_functions; i++) {
        mliFunc_free(&(scenery->functions[i]));
    }
    free(scenery->functions);
    scenery->num_functions = 0;

    free(scenery->colors);
    scenery->num_colors = 0;

    for (i = 0; i < scenery->num_meshes; i++) {
        mliMesh_free(&(scenery->meshes[i]));
    }
    scenery->num_meshes = 0u;
    free(scenery->mesh_colors);

    free(scenery->spherical_cap_hex_bound);
    scenery->num_spherical_cap_hex_bound = 0;
}

#endif
