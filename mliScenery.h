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
#include "mliSurface.h"

#define MLI_MESH 1u
#define MLI_SPHERICAL_CAP_HEX 2u
#define MLI_NUM_OBJECT_CLASSES 2u

typedef struct {
    uint32_t num_functions;
    mliFunc *functions;

    uint32_t num_colors;
    mliColor *colors;

    uint32_t num_surfaces;
    mliSurface *surfaces;

    uint32_t num_vertices;
    mliVec *vertices;

    uint32_t num_faces;
    mliFace *faces;
    uint32_t *faces_outer_surfaces;
    uint32_t *faces_inner_surfaces;

    uint32_t num_spherical_cap_hex;
    mliSphericalCapHeagonal *spherical_cap_hex_bound;
    uint32_t *spherical_cap_hex_outer_surfaces;
    uint32_t *spherical_cap_hex_inner_surfaces;
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

    /* vertices */
    scenery->vertices = (mliVec*)malloc(scenery->num_vertices*sizeof(mliVec));

    /* faces */
    scenery->faces = (mliFace*)malloc(scenery->num_faces*sizeof(mliFace));
    scenery->faces_outer_surfaces =
        (uint32_t*)malloc(scenery->num_faces*sizeof(uint32_t));
    scenery->faces_inner_surfaces =
        (uint32_t*)malloc(scenery->num_faces*sizeof(uint32_t));

    /* spherical_cap_hex */
    scenery->spherical_cap_hex_bound = (mliSphericalCapHeagonal*)malloc(
        scenery->num_spherical_cap_hex*
        sizeof(mliSphericalCapHeagonal));
    scenery->spherical_cap_hex_outer_surfaces =
        (uint32_t*)malloc(scenery->num_spherical_cap_hex*sizeof(uint32_t));
    scenery->spherical_cap_hex_inner_surfaces =
        (uint32_t*)malloc(scenery->num_spherical_cap_hex*sizeof(uint32_t));
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

    free(scenery->vertices);
    scenery->num_vertices = 0;

    /* surfaces */
    free(scenery->surfaces);
    scenery->num_surfaces = 0;

    /* faces */
    free(scenery->faces);
    free(scenery->faces_outer_surfaces);
    free(scenery->faces_inner_surfaces);
    scenery->num_faces = 0;

    /* spherical_cap_hex_bound */
    free(scenery->spherical_cap_hex_bound);
    free(scenery->spherical_cap_hex_outer_surfaces);
    free(scenery->spherical_cap_hex_inner_surfaces);
    scenery->num_spherical_cap_hex = 0;
}

#endif
