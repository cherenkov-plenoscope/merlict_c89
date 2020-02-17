/* Copyright 2019 Sebastian Achim Mueller */
#ifndef MERLICT_MLISCENERY_H_
#define MERLICT_MLISCENERY_H_

#include <math.h>
#include <stdint.h>
#include "mli_debug.h"
#include "mliVec.h"
#include "mliHomTra.h"
#include "mliColor.h"
#include "mliFunc.h"
#include "mliMesh.h"
#include "mliSphericalCapHex.h"
#include "mliSphere.h"
#include "mliCylinder.h"
#include "mliHexagon.h"
#include "mliBiCirclePlane.h"
#include "mliDisc.h"
#include "mliSurface.h"
#include "mliSurfaces.h"

#define MLI_TRIANGLE 0u
#define MLI_SPHERICAL_CAP_HEX 1u
#define MLI_SPHERE 2u
#define MLI_CYLINDER 3u
#define MLI_HEXAGON 4u
#define MLI_BICIRCLEPLANE 5u
#define MLI_DISC 6u

typedef struct {
    uint32_t num_functions;
    mliFunc *functions;

    uint32_t num_colors;
    mliColor *colors;

    uint32_t num_surfaces;
    mliSurface *surfaces;

    uint32_t num_vertices;
    struct mliVec *vertices;

    uint32_t num_triangles;
    mliFace *triangles;
    mliSurfaces *triangles_surfaces;

    uint32_t num_spherical_cap_hex;
    mliSphericalCapHex *spherical_cap_hex;
    mliSurfaces *spherical_cap_hex_surfaces;
    struct mliHomTraComp *spherical_cap_hex_T;

    uint32_t num_spheres;
    mliSphere *spheres;
    mliSurfaces *spheres_surfaces;
    struct mliHomTraComp *spheres_T;

    uint32_t num_cylinders;
    mliCylinder *cylinders;
    mliSurfaces *cylinders_surfaces;
    struct mliHomTraComp *cylinders_T;

    uint32_t num_hexagons;
    mliHexagon *hexagons;
    mliSurfaces *hexagons_surfaces;
    struct mliHomTraComp *hexagons_T;

    uint32_t num_bicircleplanes;
    mliBiCirclePlane *bicircleplanes;
    mliSurfaces *bicircleplanes_surfaces;
    struct mliHomTraComp * bicircleplanes_T;

    uint32_t num_discs;
    mliDisc *discs;
    mliSurfaces *discs_surfaces;
    struct mliHomTraComp *discs_T;
} mliScenery;

mliScenery mliScenery_init() {
    mliScenery s;
    s.num_functions = 0u;
    s.functions = NULL;

    s.num_colors = 0u;
    s.colors = NULL;

    s.num_surfaces = 0u;
    s.surfaces = NULL;

    s.num_vertices = 0u;
    s.vertices = NULL;

    s.num_triangles = 0u;
    s.triangles = NULL;
    s.triangles_surfaces = NULL;

    s.num_spherical_cap_hex = 0u;
    s.spherical_cap_hex = NULL;
    s.spherical_cap_hex_surfaces = NULL;
    s.spherical_cap_hex_T = NULL;

    s.num_spheres = 0u;
    s.spheres = NULL;
    s.spheres_surfaces = NULL;
    s.spheres_T = NULL;

    s.num_cylinders = 0u;
    s.cylinders = NULL;
    s.cylinders_surfaces = NULL;
    s.cylinders_T = NULL;

    s.num_hexagons = 0u;
    s.hexagons = NULL;
    s.hexagons_surfaces = NULL;
    s.hexagons_T = NULL;

    s.num_bicircleplanes = 0u;
    s.bicircleplanes = NULL;
    s.bicircleplanes_surfaces = NULL;
    s.bicircleplanes_T = NULL;

    s.num_discs = 0u;
    s.discs = NULL;
    s.discs_surfaces = NULL;
    s.discs_T = NULL;
    return s;}

void __mliScenery_free_functions(mliScenery *scenery) {
    uint64_t i;
    for (i = 0; i < scenery->num_functions; i++) {
        mliFunc_free(&(scenery->functions[i]));
    }
    free(scenery->functions);
    scenery->num_functions = 0;
}

void __mliScenery_free_colors(mliScenery *scenery) {
    free(scenery->colors);
    scenery->num_colors = 0;
}
void __mliScenery_free_surfaces(mliScenery *scenery) {
    free(scenery->surfaces);
    scenery->num_surfaces = 0;
}

void __mliScenery_free_vertices_and_triangles(mliScenery *scenery) {
    free(scenery->vertices);
    scenery->num_vertices = 0;
    free(scenery->triangles);
    free(scenery->triangles_surfaces);
    scenery->num_triangles = 0;
}

void __mliScenery_free_spherical_cap_hex(mliScenery *scenery) {
    free(scenery->spherical_cap_hex);
    free(scenery->spherical_cap_hex_surfaces);
    free(scenery->spherical_cap_hex_T);
    scenery->num_spherical_cap_hex = 0;
}

void __mliScenery_free_spheres(mliScenery *scenery) {
    free(scenery->spheres);
    free(scenery->spheres_surfaces);
    free(scenery->spheres_T);
    scenery->num_spheres = 0;
}

void __mliScenery_free_cylinders(mliScenery *scenery) {
    free(scenery->cylinders);
    free(scenery->cylinders_surfaces);
    free(scenery->cylinders_T);
    scenery->num_cylinders = 0;
}

void __mliScenery_free_hexagons(mliScenery *scenery) {
    free(scenery->hexagons);
    free(scenery->hexagons_surfaces);
    free(scenery->hexagons_T);
    scenery->num_hexagons = 0;
}

void __mliScenery_free_bicircleplanes(mliScenery *scenery) {
    free(scenery->bicircleplanes);
    free(scenery->bicircleplanes_surfaces);
    free(scenery->bicircleplanes_T);
    scenery->num_bicircleplanes = 0;
}

void __mliScenery_free_discs(mliScenery *scenery) {
    free(scenery->discs);
    free(scenery->discs_surfaces);
    free(scenery->discs_T);
    scenery->num_discs = 0;
}

void mliScenery_free(mliScenery *scenery) {
    __mliScenery_free_functions(scenery);
    __mliScenery_free_colors(scenery);
    __mliScenery_free_surfaces(scenery);
    __mliScenery_free_vertices_and_triangles(scenery);
    __mliScenery_free_spherical_cap_hex(scenery);
    __mliScenery_free_spheres(scenery);
    __mliScenery_free_cylinders(scenery);
    __mliScenery_free_hexagons(scenery);
    __mliScenery_free_bicircleplanes(scenery);
    __mliScenery_free_discs(scenery);
}

int __mliScenery_malloc_vertices_and_triangles(mliScenery* s) {
    mli_malloc(s->vertices, struct mliVec, s->num_vertices);
    mli_malloc(s->triangles, mliFace, s->num_triangles);
    mli_malloc(s->triangles_surfaces, mliSurfaces, s->num_triangles);
    return 1;
error:
    return 0;
}

int __mliScenery_malloc_spherical_cap_hex(mliScenery* s) {
    mli_malloc(
        s->spherical_cap_hex, mliSphericalCapHex, s->num_spherical_cap_hex);
    mli_malloc(
        s->spherical_cap_hex_surfaces, mliSurfaces, s->num_spherical_cap_hex);
    mli_malloc(
        s->spherical_cap_hex_T, struct mliHomTraComp, s->num_spherical_cap_hex);
    return 1;
error:
    return 0;
}

int __mliScenery_malloc_spheres(mliScenery* s) {
    mli_malloc(s->spheres, mliSphere, s->num_spheres);
    mli_malloc(s->spheres_surfaces, mliSurfaces, s->num_spheres);
    mli_malloc(s->spheres_T, struct mliHomTraComp, s->num_spheres);
    return 1;
error:
    return 0;
}

int __mliScenery_malloc_cylinders(mliScenery* s) {
    mli_malloc(s->cylinders, mliCylinder, s->num_cylinders);
    mli_malloc(s->cylinders_surfaces, mliSurfaces, s->num_cylinders);
    mli_malloc(s->cylinders_T, struct mliHomTraComp, s->num_cylinders);
    return 1;
error:
    return 0;
}

int __mliScenery_malloc_hexagons(mliScenery* s) {
    mli_malloc(s->hexagons, mliHexagon, s->num_hexagons);
    mli_malloc(s->hexagons_surfaces, mliSurfaces, s->num_hexagons);
    mli_malloc(s->hexagons_T, struct mliHomTraComp, s->num_hexagons);
    return 1;
error:
    return 0;
}

int __mliScenery_malloc_bicircleplane(mliScenery* s) {
    mli_malloc(s->bicircleplanes, mliBiCirclePlane, s->num_bicircleplanes);
    mli_malloc(s->bicircleplanes_surfaces, mliSurfaces, s->num_bicircleplanes);
    mli_malloc(
        s->bicircleplanes_T, struct mliHomTraComp, s->num_bicircleplanes);
    return 1;
error:
    return 0;
}

int __mliScenery_malloc_discs(mliScenery* s) {
    mli_malloc(s->discs, mliDisc, s->num_discs);
    mli_malloc(s->discs_surfaces, mliSurfaces, s->num_discs);
    mli_malloc(s->discs_T, struct mliHomTraComp, s->num_discs);
    return 1;
error:
    return 0;
}

int __mliScenery_malloc_functions(mliScenery* s) {
    uint64_t i;
    mli_malloc(s->functions, mliFunc, s->num_functions);
    for (i = 0; i < s->num_functions; i++) {
        s->functions[i] = mliFunc_init();}
    return 1;
error:
    return 0;
}

int __mliScenery_malloc_colors(mliScenery* s) {
    mli_malloc(s->colors, mliColor, s->num_colors);
    return 1;
error:
    return 0;
}

int __mliScenery_malloc_surfaces(mliScenery* s) {
    mli_malloc(s->surfaces, mliSurface, s->num_surfaces);
    return 1;
error:
    return 0;
}

int mliScenery_malloc(mliScenery* s) {
    mli_check_mem(__mliScenery_malloc_functions(s));
    mli_check_mem(__mliScenery_malloc_colors(s));
    mli_check_mem(__mliScenery_malloc_surfaces(s));
    mli_check_mem(__mliScenery_malloc_vertices_and_triangles(s));
    mli_check_mem(__mliScenery_malloc_spherical_cap_hex(s));
    mli_check_mem(__mliScenery_malloc_spheres(s));
    mli_check_mem(__mliScenery_malloc_cylinders(s));
    mli_check_mem(__mliScenery_malloc_hexagons(s));
    mli_check_mem(__mliScenery_malloc_bicircleplane(s));
    mli_check_mem(__mliScenery_malloc_discs(s));
    return 1;
error:
    mliScenery_free(s);
    return 0;
}

int __mliScenery_write_spherical_cap_hex(const mliScenery *scenery, FILE *f) {
    mli_fwrite(
        scenery->spherical_cap_hex,
        sizeof(mliSphericalCapHex),
        scenery->num_spherical_cap_hex,
        f);
    mli_fwrite(
        scenery->spherical_cap_hex_surfaces,
        sizeof(mliSurfaces),
        scenery->num_spherical_cap_hex,
        f);
    mli_fwrite(
        scenery->spherical_cap_hex_T,
        sizeof(struct mliHomTraComp),
        scenery->num_spherical_cap_hex,
        f);
    return 1;
error:
    return 0;
}

int __mliScenery_write_spheres(const mliScenery *scenery, FILE *f) {
    mli_fwrite(scenery->spheres, sizeof(double), scenery->num_spheres, f);
    mli_fwrite(
        scenery->spheres_surfaces,
        sizeof(mliSurfaces),
        scenery->num_spheres,
        f);
    mli_fwrite(
        scenery->spheres_T,
        sizeof(struct mliHomTraComp),
        scenery->num_spheres,
        f);
    return 1;
error:
    return 0;
}

int __mliScenery_write_cylinders(const mliScenery *scenery, FILE *f) {
    mli_fwrite(
        scenery->cylinders,
        sizeof(mliCylinder),
        scenery->num_cylinders,
        f);
    mli_fwrite(
        scenery->cylinders_surfaces,
        sizeof(mliSurfaces),
        scenery->num_cylinders,
        f);
    mli_fwrite(
        scenery->cylinders_T,
        sizeof(struct mliHomTraComp),
        scenery->num_cylinders,
        f);
    return 1;
error:
    return 0;
}

int __mliScenery_write_hexagons(const mliScenery *scenery, FILE *f) {
    mli_fwrite(scenery->hexagons, sizeof(mliHexagon), scenery->num_hexagons, f);
    mli_fwrite(
        scenery->hexagons_surfaces,
        sizeof(mliSurfaces),
        scenery->num_hexagons,
        f);
    mli_fwrite(
        scenery->hexagons_T,
        sizeof(struct mliHomTraComp),
        scenery->num_hexagons,
        f);
    return 1;
error:
    return 0;
}

int __mliScenery_write_bicircleplanes(const mliScenery *scenery, FILE *f) {
    mli_fwrite(
        scenery->bicircleplanes,
        sizeof(mliBiCirclePlane),
        scenery->num_bicircleplanes,
        f);
    mli_fwrite(
        scenery->bicircleplanes_surfaces,
        sizeof(mliSurfaces),
        scenery->num_bicircleplanes,
        f);
    mli_fwrite(
        scenery->bicircleplanes_T,
        sizeof(struct mliHomTraComp),
        scenery->num_bicircleplanes,
        f);
    return 1;
error:
    return 0;
}

int __mliScenery_write_discs(const mliScenery *scenery, FILE *f) {
    mli_fwrite(
        scenery->discs,
        sizeof(mliDisc),
        scenery->num_discs,
        f);
    mli_fwrite(
        scenery->discs_surfaces,
        sizeof(mliSurfaces),
        scenery->num_discs,
        f);
    mli_fwrite(
        scenery->discs_T,
        sizeof(struct mliHomTraComp),
        scenery->num_discs,
        f);
    return 1;
error:
    return 0;
}

#define MLI_SCENERY_MAGIC 43180u

int mliScenery_write_to_path(const mliScenery *scenery, const char* path) {
    FILE *f;
    uint64_t i;
    uint64_t magic = MLI_SCENERY_MAGIC;
    f = fopen(path, "w");
    mli_check(f != NULL, "Can not open Scenery-file for writing.");

    /* magic identifier */
    mli_fwrite(&magic, sizeof(uint64_t), 1u, f);

    /* nums */
    mli_fwrite(&scenery->num_functions, sizeof(uint32_t), 1u, f);
    mli_fwrite(&scenery->num_colors, sizeof(uint32_t), 1u, f);
    mli_fwrite(&scenery->num_surfaces, sizeof(uint32_t), 1u, f);
    mli_fwrite(&scenery->num_vertices, sizeof(uint32_t), 1u, f);
    mli_fwrite(&scenery->num_triangles, sizeof(uint32_t), 1u, f);
    mli_fwrite(&scenery->num_spherical_cap_hex, sizeof(uint32_t), 1u, f);
    mli_fwrite(&scenery->num_spheres, sizeof(uint32_t), 1u, f);
    mli_fwrite(&scenery->num_cylinders, sizeof(uint32_t), 1u, f);
    mli_fwrite(&scenery->num_hexagons, sizeof(uint32_t), 1u, f);
    mli_fwrite(&scenery->num_bicircleplanes, sizeof(uint32_t), 1u, f);
    mli_fwrite(&scenery->num_discs, sizeof(uint32_t), 1u, f);

    /* functions */
    for (i = 0; i < scenery->num_functions; i++) {
        mliFunc_fwrite(&scenery->functions[i], f);}

    /* colors */
    mli_fwrite(scenery->colors, sizeof(mliColor), scenery->num_colors, f);

    /* surfaces */
    mli_fwrite(scenery->surfaces, sizeof(mliSurface), scenery->num_surfaces, f);

    /* vertices */
    mli_fwrite(
        scenery->vertices, sizeof(struct mliVec), scenery->num_vertices, f);

    /* triangles */
    mli_fwrite(scenery->triangles, sizeof(mliFace), scenery->num_triangles, f);
    mli_fwrite(
        scenery->triangles_surfaces,
        sizeof(mliSurfaces),
        scenery->num_triangles,
        f);

    mli_c(__mliScenery_write_spherical_cap_hex(scenery, f));
    mli_c(__mliScenery_write_spheres(scenery, f));
    mli_c(__mliScenery_write_cylinders(scenery, f));
    mli_c(__mliScenery_write_hexagons(scenery, f));
    mli_c(__mliScenery_write_bicircleplanes(scenery, f));
    mli_c(__mliScenery_write_discs(scenery, f));

    fclose(f);
    return 1;
error:
    if (f != NULL) {
        fclose(f);
    }
    return 0;}


int __mliScenery_read_vertices_and_triangles(mliScenery *scenery, FILE* f) {
    mli_fread(
        scenery->vertices, sizeof(struct mliVec), scenery->num_vertices, f);
    mli_fread(scenery->triangles, sizeof(mliFace), scenery->num_triangles, f);
    mli_fread(
        scenery->triangles_surfaces,
        sizeof(mliSurfaces),
        scenery->num_triangles,
        f);
    return 1;
error:
    return 0;
}

int __mliScenery_read_spherical_cap_hex(mliScenery *scenery, FILE* f) {
    mli_fread(
        scenery->spherical_cap_hex,
        sizeof(mliSphericalCapHex),
        scenery->num_spherical_cap_hex, f);
    mli_fread(
        scenery->spherical_cap_hex_surfaces,
        sizeof(mliSurfaces),
        scenery->num_spherical_cap_hex, f);
    mli_fread(
        scenery->spherical_cap_hex_T,
        sizeof(struct mliHomTraComp),
        scenery->num_spherical_cap_hex, f);
    return 1;
error:
    return 0;
}

int __mliScenery_read_spheres(mliScenery *scenery, FILE* f) {
    mli_fread(
        scenery->spheres,
        sizeof(double),
        scenery->num_spheres, f);
    mli_fread(
        scenery->spheres_surfaces,
        sizeof(mliSurfaces),
        scenery->num_spheres, f);
    mli_fread(
        scenery->spheres_T,
        sizeof(struct mliHomTraComp),
        scenery->num_spheres, f);
    return 1;
error:
    return 0;
}

int __mliScenery_read_cylinders(mliScenery *scenery, FILE* f) {
    mli_fread(
        scenery->cylinders,
        sizeof(mliCylinder),
        scenery->num_cylinders, f);
    mli_fread(
        scenery->cylinders_surfaces,
        sizeof(mliSurfaces),
        scenery->num_cylinders, f);
    mli_fread(
        scenery->cylinders_T,
        sizeof(struct mliHomTraComp),
        scenery->num_cylinders, f);
    return 1;
error:
    return 0;
}

int __mliScenery_read_hexagons(mliScenery *scenery, FILE* f) {
    mli_fread(
        scenery->hexagons,
        sizeof(mliHexagon),
        scenery->num_hexagons, f);
    mli_fread(
        scenery->hexagons_surfaces,
        sizeof(mliSurfaces),
        scenery->num_hexagons, f);
    mli_fread(
        scenery->hexagons_T,
        sizeof(struct mliHomTraComp),
        scenery->num_hexagons, f);
    return 1;
error:
    return 0;
}

int __mliScenery_read_bicircleplanes(mliScenery *scenery, FILE* f) {
    mli_fread(
        scenery->bicircleplanes,
        sizeof(mliBiCirclePlane),
        scenery->num_bicircleplanes,
        f);
    mli_fread(
        scenery->bicircleplanes_surfaces,
        sizeof(mliSurfaces),
        scenery->num_bicircleplanes,
        f);
    mli_fread(
        scenery->bicircleplanes_T,
        sizeof(struct mliHomTraComp),
        scenery->num_bicircleplanes,
        f);
    return 1;
error:
    return 0;
}

int __mliScenery_read_discs(mliScenery *scenery, FILE* f) {
    mli_fread(
        scenery->discs,
        sizeof(mliDisc),
        scenery->num_discs,
        f);
    mli_fread(
        scenery->discs_surfaces,
        sizeof(mliSurfaces),
        scenery->num_discs,
        f);
    mli_fread(
        scenery->discs_T,
        sizeof(struct mliHomTraComp),
        scenery->num_discs,
        f);
    return 1;
error:
    return 0;
}

int mliScenery_read_from_path(mliScenery *scenery, const char* path) {
    FILE *f;
    uint64_t i;
    uint64_t magic = 0u;
    f = fopen(path, "r");
    mli_check(f != NULL, "Can not open Scenery-file for reading.")
    mli_fread(&magic, sizeof(uint64_t), 1u, f);
    mli_check(magic == MLI_SCENERY_MAGIC, "Expected magic identifier.'");

    /* nums */
    mli_fread(&scenery->num_functions, sizeof(uint32_t), 1u, f);
    mli_fread(&scenery->num_colors, sizeof(uint32_t), 1u, f);
    mli_fread(&scenery->num_surfaces, sizeof(uint32_t), 1u, f);
    mli_fread(&scenery->num_vertices, sizeof(uint32_t), 1u, f);
    mli_fread(&scenery->num_triangles, sizeof(uint32_t), 1u, f);
    mli_fread(&scenery->num_spherical_cap_hex, sizeof(uint32_t), 1u, f);
    mli_fread(&scenery->num_spheres, sizeof(uint32_t), 1u, f);
    mli_fread(&scenery->num_cylinders, sizeof(uint32_t), 1u, f);
    mli_fread(&scenery->num_hexagons, sizeof(uint32_t), 1u, f);
    mli_fread(&scenery->num_bicircleplanes, sizeof(uint32_t), 1u, f);
    mli_fread(&scenery->num_discs, sizeof(uint32_t), 1u, f);

    mli_check_mem(mliScenery_malloc(scenery));

    /* functions */
    for (i = 0; i < scenery->num_functions; i++) {
        mli_c(mliFunc_malloc_from_file(&scenery->functions[i], f));}

    /* colors */
    mli_fread(scenery->colors, sizeof(mliColor), scenery->num_colors, f);

    /* surfaces */
    mli_fread(scenery->surfaces, sizeof(mliSurface), scenery->num_surfaces, f);

    mli_c(__mliScenery_read_vertices_and_triangles(scenery, f));
    mli_c(__mliScenery_read_spherical_cap_hex(scenery, f));
    mli_c(__mliScenery_read_spheres(scenery, f));
    mli_c(__mliScenery_read_cylinders(scenery, f));
    mli_c(__mliScenery_read_hexagons(scenery, f));
    mli_c(__mliScenery_read_bicircleplanes(scenery, f));
    mli_c(__mliScenery_read_discs(scenery, f));

    fclose(f);
    return 1;
error:
    if (f != NULL) {
        fclose(f);
    }
    mliScenery_free(scenery);
    return 0;}

int mliScenery_is_equal(const mliScenery *a, const mliScenery *b) {
    uint64_t i;
    if (a->num_functions != b->num_functions) return 0;
    if (a->num_colors != b->num_colors ) return 0;
    if (a->num_surfaces != b->num_surfaces ) return 0;
    if (a->num_vertices != b->num_vertices ) return 0;
    if (a->num_triangles != b->num_triangles ) return 0;
    if (a->num_spherical_cap_hex != b->num_spherical_cap_hex ) return 0;
    if (a->num_spheres != b->num_spheres) return 0;
    if (a->num_cylinders != b->num_cylinders) return 0;
    if (a->num_hexagons != b->num_hexagons) return 0;
    if (a->num_bicircleplanes != b->num_bicircleplanes) return 0;
    if (a->num_discs != b->num_discs) return 0;

    for (i = 0; i < a->num_functions; i++) {
        if (!mliFunc_is_equal(a->functions[i], b->functions[i])) return 0;}
    for (i = 0; i < a->num_colors; i++) {
        if (!mliColor_is_equal(a->colors[i], b->colors[i])) return 0;}
    for (i = 0; i < a->num_surfaces; i++) {
        if (!mliSurface_is_equal(a->surfaces[i], b->surfaces[i])) return 0;}
    for (i = 0; i < a->num_vertices; i++) {
        if (!mliVec_is_equal(a->vertices[i], b->vertices[i])) return 0;}
    for (i = 0; i < a->num_triangles; i++) {
        if (!mliFace_is_equal(a->triangles[i], b->triangles[i]))
            return 0;
        if (!mliSurfaces_is_equal(
            a->triangles_surfaces[i],
            b->triangles_surfaces[i]))
            return 0;
    }
    for (i = 0; i < a->num_spherical_cap_hex; i++) {
        if (!mliSphericalCapHex_is_equal(
                a->spherical_cap_hex[i],
                b->spherical_cap_hex[i]))
            return 0;
        if (!mliSurfaces_is_equal(
                a->spherical_cap_hex_surfaces[i],
                b->spherical_cap_hex_surfaces[i]))
            return 0;
        if(!mliHomTraComp_is_equal(
            a->spherical_cap_hex_T[i],
            b->spherical_cap_hex_T[i]))
            return 0;
    }
    for (i = 0; i < a->num_spheres; i++) {
        if (!mliSphere_is_equal(a->spheres[i], b->spheres[i]))
            return 0;
        if (!mliSurfaces_is_equal(
                a->spheres_surfaces[i],
                b->spheres_surfaces[i]))
            return 0;
        if(!mliHomTraComp_is_equal(
            a->spheres_T[i],
            b->spheres_T[i]))
            return 0;
    }
    for (i = 0; i < a->num_cylinders; i++) {
        if (!mliCylinder_is_equal(a->cylinders[i], b->cylinders[i]))
            return 0;
        if (!mliSurfaces_is_equal(
                a->cylinders_surfaces[i],
                b->cylinders_surfaces[i]))
            return 0;
        if(!mliHomTraComp_is_equal(
            a->cylinders_T[i],
            b->cylinders_T[i]))
            return 0;
    }
    for (i = 0; i < a->num_hexagons; i++) {
        if (!mliHexagon_is_equal(a->hexagons[i], b->hexagons[i]))
            return 0;
        if (!mliSurfaces_is_equal(
                a->hexagons_surfaces[i],
                b->hexagons_surfaces[i]))
            return 0;
        if(!mliHomTraComp_is_equal(
            a->hexagons_T[i],
            b->hexagons_T[i]))
            return 0;
    }
    for (i = 0; i < a->num_bicircleplanes; i++) {
        if (!mliBiCirclePlane_is_equal(
            a->bicircleplanes[i],
            b->bicircleplanes[i])
        ) {
            return 0;
        }
        if (!mliSurfaces_is_equal(
                a->bicircleplanes_surfaces[i],
                b->bicircleplanes_surfaces[i]))
            return 0;
        if(!mliHomTraComp_is_equal(
            a->bicircleplanes_T[i],
            b->bicircleplanes_T[i]))
            return 0;
    }
    for (i = 0; i < a->num_discs; i++) {
        if (!mliDisc_is_equal(a->discs[i], b->discs[i])) {
            return 0;
        }
        if (!mliSurfaces_is_equal(a->discs_surfaces[i], b->discs_surfaces[i]))
            return 0;
        if(!mliHomTraComp_is_equal(a->discs_T[i], b->discs_T[i]))
            return 0;
    }
    return 1;}


int mliScenery_valid_surfaces(const mliScenery *scenery) {
    uint64_t i;
    for (i = 0; i < scenery->num_surfaces; i++) {
        switch (scenery->surfaces[i].material) {
            case MLI_MATERIAL_PHONG: break;
            case MLI_MATERIAL_TRANSPARENT: break;
            default: return 0; break;}
        if (scenery->surfaces[i].color >= scenery->num_colors)
            return 0;
        if (scenery->surfaces[i].medium_refraction >= scenery->num_functions)
            return 0;
        if (scenery->surfaces[i].medium_refraction >= scenery->num_functions)
            return 0;
        if (scenery->surfaces[i].boundary_layer_specular_reflection >=
            scenery->num_functions)
            return 0;
        if (scenery->surfaces[i].boundary_layer_diffuse_reflection >=
            scenery->num_functions)
            return 0;
    }
    return 1;}

int mliScenery_valid_triangles(const mliScenery *scenery) {
    uint64_t i;
    for (i = 0; i < scenery->num_triangles; i++) {
        if (scenery->triangles[i].a >= scenery->num_vertices)
            return 0;
        if (scenery->triangles[i].b >= scenery->num_vertices)
            return 0;
        if (scenery->triangles[i].c >= scenery->num_vertices)
            return 0;
        if (scenery->triangles_surfaces[i].inner >= scenery->num_surfaces)
            return 0;
        if (scenery->triangles_surfaces[i].outer >= scenery->num_surfaces)
            return 0;}
    return 1;}

int mliScenery_valid_spherical_cap_hex(const mliScenery *scenery) {
    uint64_t i;
    for (i = 0; i < scenery->num_spherical_cap_hex; i++) {
        if (    scenery->spherical_cap_hex_surfaces[i].inner >=
                scenery->num_surfaces)
            return 0;
        if (    scenery->spherical_cap_hex_surfaces[i].outer >=
                scenery->num_surfaces)
            return 0;
    }
    return 1;}

int mliScenery_valid_spheres(const mliScenery *scenery) {
    uint64_t i;
    for (i = 0; i < scenery->num_spheres; i++) {
        if (    scenery->spheres_surfaces[i].inner >=
                scenery->num_surfaces)
            return 0;
        if (    scenery->spheres_surfaces[i].outer >=
                scenery->num_surfaces)
            return 0;
    }
    return 1;}

int mliScenery_valid_cylinders(const mliScenery *scenery) {
    uint64_t i;
    for (i = 0; i < scenery->num_cylinders; i++) {
        if (    scenery->cylinders_surfaces[i].inner >=
                scenery->num_surfaces)
            return 0;
        if (    scenery->cylinders_surfaces[i].outer >=
                scenery->num_surfaces)
            return 0;
    }
    return 1;}

int mliScenery_valid_hexagons(const mliScenery *scenery) {
    uint64_t i;
    for (i = 0; i < scenery->num_hexagons; i++) {
        if (scenery->hexagons_surfaces[i].inner >= scenery->num_surfaces) {
            return 0;
        }
        if (scenery->hexagons_surfaces[i].outer >= scenery->num_surfaces) {
            return 0;
        }
    }
    return 1;}

int mliScenery_valid_bicircleplanes(const mliScenery *scenery) {
    uint64_t i;
    for (i = 0; i < scenery->num_bicircleplanes; i++) {
        if (scenery->bicircleplanes_surfaces[i].inner >=
            scenery->num_surfaces
        ) {
            return 0;
        }
        if (scenery->bicircleplanes_surfaces[i].outer >=
            scenery->num_surfaces
        ) {
            return 0;
        }
        if (!mliBiCirclePlane_is_valid(scenery->bicircleplanes[i])) {
            return 0;
        }
    }
    return 1;}

int mliScenery_valid_disc(const mliScenery *scenery) {
    uint64_t i;
    for (i = 0; i < scenery->num_discs; i++) {
        if (scenery->discs_surfaces[i].inner >=
            scenery->num_surfaces
        ) {
            return 0;
        }
        if (scenery->discs_surfaces[i].outer >=
            scenery->num_surfaces
        ) {
            return 0;
        }
        if (!mliDisc_is_valid(scenery->discs[i])) {
            return 0;
        }
    }
    return 1;}

int mliScenery_valid(const mliScenery *scenery) {
    if (!mliScenery_valid_surfaces(scenery))
        return 0;
    if (!mliScenery_valid_triangles(scenery))
        return 0;
    if (!mliScenery_valid_spherical_cap_hex(scenery))
        return 0;
    if (!mliScenery_valid_spheres(scenery))
        return 0;
    if (!mliScenery_valid_cylinders(scenery))
        return 0;
    if (!mliScenery_valid_hexagons(scenery))
        return 0;
    if (!mliScenery_valid_bicircleplanes(scenery))
        return 0;
    if (!mliScenery_valid_disc(scenery))
        return 0;
    return 1;}

#endif
