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
#include "mliSurfaces.h"

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
    mliSurfaces *faces_surfaces;

    uint32_t num_spherical_cap_hex;
    mliSphericalCapHeagonal *spherical_cap_hex;
    mliSurfaces *spherical_cap_hex_surfaces;
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
    scenery->faces_surfaces =
        (mliSurfaces*)malloc(scenery->num_faces*sizeof(mliSurfaces));

    /* spherical_cap_hex */
    scenery->spherical_cap_hex = (mliSphericalCapHeagonal*)malloc(
        scenery->num_spherical_cap_hex*
        sizeof(mliSphericalCapHeagonal));
    scenery->spherical_cap_hex_surfaces = (mliSurfaces*)malloc(
        scenery->num_spherical_cap_hex*sizeof(mliSurfaces));
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

    /* vertices */
    free(scenery->vertices);
    scenery->num_vertices = 0;

    /* faces */
    free(scenery->faces);
    free(scenery->faces_surfaces);
    scenery->num_faces = 0;

    /* spherical_cap_hex */
    free(scenery->spherical_cap_hex);
    free(scenery->spherical_cap_hex_surfaces);
    scenery->num_spherical_cap_hex = 0;
}

int mliScenery_write_to_path(const mliScenery *scenery, const char* path) {
    FILE *f;
    int i;
    f = fopen(path, "w");
    if (f == NULL) goto close_failure;
    fprintf(f, "merlict_c89\n");

    /* nums */
    fwrite(&scenery->num_functions, sizeof(uint32_t), 1u, f);
    fwrite(&scenery->num_colors, sizeof(uint32_t), 1u, f);
    fwrite(&scenery->num_surfaces, sizeof(uint32_t), 1u, f);
    fwrite(&scenery->num_vertices, sizeof(uint32_t), 1u, f);
    fwrite(&scenery->num_faces, sizeof(uint32_t), 1u, f);
    fwrite(&scenery->num_spherical_cap_hex, sizeof(uint32_t), 1u, f);

    /* functions */
    for (i = 0; i < scenery->num_functions; i++) {
        mliFunc_fwrite(&scenery->functions[i], f);}

    /* colors */
    fwrite(scenery->colors, sizeof(mliColor), scenery->num_colors, f);

    /* surfaces */
    fwrite(scenery->surfaces, sizeof(mliSurface), scenery->num_surfaces, f);

    /* vertices */
    fwrite(scenery->vertices, sizeof(mliVec), scenery->num_vertices, f);

    /* faces */
    fwrite(scenery->faces, sizeof(mliFace), scenery->num_faces, f);
    fwrite(scenery->faces_surfaces, sizeof(mliSurfaces), scenery->num_faces, f);

    /* spherical_cap_hex */
    fwrite(
        scenery->spherical_cap_hex,
        sizeof(mliSphericalCapHeagonal),
        scenery->num_spherical_cap_hex, f);
    fwrite(
        scenery->spherical_cap_hex_surfaces,
        sizeof(mliSurfaces),
        scenery->num_spherical_cap_hex, f);

    fclose(f);
    return EXIT_SUCCESS;

    close_failure:
    fclose(f);
    return EXIT_FAILURE;}

int mliScenery_read_from_path(mliScenery *scenery, const char* path) {
    FILE *f;
    int i;
    char line[64];
    f = fopen(path, "r");
    if (f == NULL) goto close_failure;
    if (fgets(line, 1024, f) == NULL) goto close_failure;
    if (strcmp(line, "merlict_c89\n") != 0) goto close_failure;

    /* nums */
    fread(&scenery->num_functions, sizeof(uint32_t), 1u, f);
    fread(&scenery->num_colors, sizeof(uint32_t), 1u, f);
    fread(&scenery->num_surfaces, sizeof(uint32_t), 1u, f);
    fread(&scenery->num_vertices, sizeof(uint32_t), 1u, f);
    fread(&scenery->num_faces, sizeof(uint32_t), 1u, f);
    fread(&scenery->num_spherical_cap_hex, sizeof(uint32_t), 1u, f);

    mliScenery_malloc(scenery);

    /* functions */
    for (i = 0; i < scenery->num_functions; i++) {
        mliFunc_fread(&scenery->functions[i], f);}

    /* colors */
    fread(scenery->colors, sizeof(mliColor), scenery->num_colors, f);

    /* surfaces */
    fread(scenery->surfaces, sizeof(mliSurface), scenery->num_surfaces, f);

    /* vertices */
    fread(scenery->vertices, sizeof(mliVec), scenery->num_vertices, f);

    /* faces */
    fread(scenery->faces, sizeof(mliFace), scenery->num_faces, f);
    fread(scenery->faces_surfaces, sizeof(mliSurfaces), scenery->num_faces, f);

    /* spherical_cap_hex */
    fread(
        scenery->spherical_cap_hex,
        sizeof(mliSphericalCapHeagonal),
        scenery->num_spherical_cap_hex, f);
    fread(
        scenery->spherical_cap_hex_surfaces,
        sizeof(mliSurfaces),
        scenery->num_spherical_cap_hex, f);

    fclose(f);
    return EXIT_SUCCESS;

    close_failure:
    fclose(f);
    return EXIT_FAILURE;}

int mliScenery_is_equal(const mliScenery *a, const mliScenery *b) {
    int i;
    if (a->num_functions != b->num_functions) return 0;
    if (a->num_colors != b->num_colors ) return 0;
    if (a->num_surfaces != b->num_surfaces ) return 0;
    if (a->num_vertices != b->num_vertices ) return 0;
    if (a->num_faces != b->num_faces ) return 0;
    if (a->num_spherical_cap_hex != b->num_spherical_cap_hex ) return 0;
    for (i = 0; i < a->num_functions; i++) {
        if (!mliFunc_is_equal(&a->functions[i], &b->functions[i])) return 0;}
    for (i = 0; i < a->num_colors; i++) {
        if (!mliColor_is_equal(&a->colors[i], &b->colors[i])) return 0;}
    for (i = 0; i < a->num_surfaces; i++) {
        if (!mliSurface_is_equal(&a->surfaces[i], &b->surfaces[i])) return 0;}
    for (i = 0; i < a->num_vertices; i++) {
        if (!mliVec_is_equal(&a->vertices[i], &b->vertices[i])) return 0;}
    for (i = 0; i < a->num_faces; i++) {
        if (!mliFace_is_equal(&a->faces[i], &b->faces[i]))
            return 0;
        if (!mliSurfaces_is_equal(&a->faces_surfaces[i], &b->faces_surfaces[i]))
            return 0;}
    for (i = 0; i < a->num_spherical_cap_hex; i++) {
        if (!mliSphericalCapHeagonal_is_equal(
                &a->spherical_cap_hex[i],
                &b->spherical_cap_hex[i]))
            return 0;
        if (!mliSurfaces_is_equal(
                &a->spherical_cap_hex_surfaces[i],
                &b->spherical_cap_hex_surfaces[i]))
            return 0;}
    return 1;}


int mliScenery_valid_surfaces(const mliScenery *scenery) {
    int i;
    for (i = 0; i < scenery->num_surfaces; i++) {
        if (scenery->surfaces[i].color >= scenery->num_colors)
            return 0;
        if (scenery->surfaces[i].reflection >= scenery->num_functions)
            return 0;
        if (scenery->surfaces[i].refraction >= scenery->num_functions)
            return 0;
        if (scenery->surfaces[i].absorbtion >= scenery->num_functions)
            return 0;}
    return 1;}

int mliScenery_valid_faces(const mliScenery *scenery) {
    int i;
    for (i = 0; i < scenery->num_faces; i++) {
        if (scenery->faces[i].a >= scenery->num_vertices)
            return 0;
        if (scenery->faces[i].b >= scenery->num_vertices)
            return 0;
        if (scenery->faces[i].c >= scenery->num_vertices)
            return 0;
        if (scenery->faces_surfaces[i].inner >= scenery->num_surfaces)
            return 0;
        if (scenery->faces_surfaces[i].outer >= scenery->num_surfaces)
            return 0;}
    return 1;}

int mliScenery_valid_spherical_cap_hex(const mliScenery *scenery) {
    int i;
    for (i = 0; i < scenery->num_spherical_cap_hex; i++) {
        if (    scenery->spherical_cap_hex_surfaces[i].inner >=
                scenery->num_surfaces)
            return 0;
        if (    scenery->spherical_cap_hex_surfaces[i].outer >=
                scenery->num_surfaces)
            return 0;
    }
    return 1;}

int mliScenery_valid(const mliScenery *scenery) {
    if (!mliScenery_valid_surfaces(scenery))
        return 0;
    if (!mliScenery_valid_faces(scenery))
        return 0;
    if (!mliScenery_valid_spherical_cap_hex(scenery))
        return 0;
    return 1;}

#endif
