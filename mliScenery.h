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
#include "mliSurfaceProperty.h"

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

void mliScenery_malloc(mliScenery* scenery) {
    /* functions */
    scenery->functions = (mliFunc*)malloc(
        scenery->num_functions*sizeof(mliFunc));

    /* colors */
    scenery->colors = (mliColor*)malloc(
        scenery->num_colors*sizeof(mliColor));

    /* surfaces */
    scenery->surfaces = (mliSurface*)malloc(
        scenery->num_surfaces*sizeof(mliSurface));

    /* meshes */
    scenery->meshes = (mliMesh*)malloc(
        scenery->num_meshes*sizeof(mliMesh));

    scenery->mesh_colors = (uint32_t*)malloc(
        scenery->num_meshes*sizeof(uint32_t));

    scenery->spherical_cap_hex_bound = (mliSphericalCapHeagonal*)malloc(
        scenery->num_spherical_cap_hex_bound*
        sizeof(mliSphericalCapHeagonal));
}

void mliScenery_free(mliScenery *scenery) {
    int i;
    /* functions */
    for (i = 0; i < scenery->num_functions; i++) {
        mliFunc_free(&(scenery->functions[i]));}
    free(scenery->functions);
    scenery->num_functions = 0;

    /* colors */
    free(scenery->colors);
    scenery->num_colors = 0;

    /* surfaces */
    free(scenery->surfaces);
    scenery->num_surfaces = 0;

    /* meshes */
    for (i = 0; i < scenery->num_meshes; i++) {
        mliMesh_free(&(scenery->meshes[i]));}
    scenery->num_meshes = 0u;

    /* mesh_colors */
    free(scenery->mesh_colors);

    /* spherical_cap_hex_bound */
    free(scenery->spherical_cap_hex_bound);
    scenery->num_spherical_cap_hex_bound = 0;
}

#endif
