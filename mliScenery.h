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

struct mliScenery {
    uint32_t num_functions;
    struct mliFunc *functions;

    uint32_t num_colors;
    struct mliColor *colors;

    uint32_t num_surfaces;
    struct mliSurface *surfaces;

    uint32_t num_vertices;
    struct mliVec *vertices;

    uint32_t num_triangles;
    struct mliFace *triangles;
    struct mliSurfaces *triangles_surfaces;

    uint32_t num_spherical_cap_hex;
    struct mliSphericalCapHex *spherical_cap_hex;
    struct mliSurfaces *spherical_cap_hex_surfaces;
    struct mliHomTraComp *spherical_cap_hex_T;

    uint32_t num_spheres;
    struct mliSphere *spheres;
    struct mliSurfaces *spheres_surfaces;
    struct mliHomTraComp *spheres_T;

    uint32_t num_cylinders;
    struct mliCylinder *cylinders;
    struct mliSurfaces *cylinders_surfaces;
    struct mliHomTraComp *cylinders_T;

    uint32_t num_hexagons;
    struct mliHexagon *hexagons;
    struct mliSurfaces *hexagons_surfaces;
    struct mliHomTraComp *hexagons_T;

    uint32_t num_bicircleplanes;
    struct mliBiCirclePlane *bicircleplanes;
    struct mliSurfaces *bicircleplanes_surfaces;
    struct mliHomTraComp * bicircleplanes_T;

    uint32_t num_discs;
    struct mliDisc *discs;
    struct mliSurfaces *discs_surfaces;
    struct mliHomTraComp *discs_T;
};

struct mliScenery mliScenery_init() {
    struct mliScenery s;
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

void __mliScenery_free_functions(struct mliScenery *scenery) {
    uint64_t i;
    for (i = 0; i < scenery->num_functions; i++) {
        mliFunc_free(&(scenery->functions[i]));
    }
    free(scenery->functions);
    scenery->num_functions = 0;
}

void __mliScenery_free_colors(struct mliScenery *scenery) {
    free(scenery->colors);
    scenery->num_colors = 0;
}
void __mliScenery_free_surfaces(struct mliScenery *scenery) {
    free(scenery->surfaces);
    scenery->num_surfaces = 0;
}

void __mliScenery_free_vertices_and_triangles(struct mliScenery *scenery) {
    free(scenery->vertices);
    scenery->num_vertices = 0;
    free(scenery->triangles);
    free(scenery->triangles_surfaces);
    scenery->num_triangles = 0;
}

void __mliScenery_free_spherical_cap_hex(struct mliScenery *scenery) {
    free(scenery->spherical_cap_hex);
    free(scenery->spherical_cap_hex_surfaces);
    free(scenery->spherical_cap_hex_T);
    scenery->num_spherical_cap_hex = 0;
}

void __mliScenery_free_spheres(struct mliScenery *scenery) {
    free(scenery->spheres);
    free(scenery->spheres_surfaces);
    free(scenery->spheres_T);
    scenery->num_spheres = 0;
}

void __mliScenery_free_cylinders(struct mliScenery *scenery) {
    free(scenery->cylinders);
    free(scenery->cylinders_surfaces);
    free(scenery->cylinders_T);
    scenery->num_cylinders = 0;
}

void __mliScenery_free_hexagons(struct mliScenery *scenery) {
    free(scenery->hexagons);
    free(scenery->hexagons_surfaces);
    free(scenery->hexagons_T);
    scenery->num_hexagons = 0;
}

void __mliScenery_free_bicircleplanes(struct mliScenery *scenery) {
    free(scenery->bicircleplanes);
    free(scenery->bicircleplanes_surfaces);
    free(scenery->bicircleplanes_T);
    scenery->num_bicircleplanes = 0;
}

void __mliScenery_free_discs(struct mliScenery *scenery) {
    free(scenery->discs);
    free(scenery->discs_surfaces);
    free(scenery->discs_T);
    scenery->num_discs = 0;
}

void mliScenery_free(struct mliScenery *scenery) {
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

int __mliScenery_malloc_vertices_and_triangles(struct mliScenery *s) {
    mli_malloc(s->vertices, struct mliVec, s->num_vertices);
    mli_malloc(s->triangles, struct mliFace, s->num_triangles);
    mli_malloc(s->triangles_surfaces, struct mliSurfaces, s->num_triangles);
    return 1;
error:
    return 0;
}

int __mliScenery_malloc_spherical_cap_hex(struct mliScenery *s) {
    mli_malloc(
        s->spherical_cap_hex,
        struct mliSphericalCapHex,
        s->num_spherical_cap_hex);
    mli_malloc(
        s->spherical_cap_hex_surfaces,
        struct mliSurfaces,
        s->num_spherical_cap_hex);
    mli_malloc(
        s->spherical_cap_hex_T,
        struct mliHomTraComp,
        s->num_spherical_cap_hex);
    return 1;
error:
    return 0;
}

int __mliScenery_malloc_spheres(struct mliScenery *s) {
    mli_malloc(s->spheres, struct mliSphere, s->num_spheres);
    mli_malloc(s->spheres_surfaces, struct mliSurfaces, s->num_spheres);
    mli_malloc(s->spheres_T, struct mliHomTraComp, s->num_spheres);
    return 1;
error:
    return 0;
}

int __mliScenery_malloc_cylinders(struct mliScenery *s) {
    mli_malloc(s->cylinders, struct mliCylinder, s->num_cylinders);
    mli_malloc(s->cylinders_surfaces, struct mliSurfaces, s->num_cylinders);
    mli_malloc(s->cylinders_T, struct mliHomTraComp, s->num_cylinders);
    return 1;
error:
    return 0;
}

int __mliScenery_malloc_hexagons(struct mliScenery *s) {
    mli_malloc(s->hexagons, struct mliHexagon, s->num_hexagons);
    mli_malloc(s->hexagons_surfaces, struct mliSurfaces, s->num_hexagons);
    mli_malloc(s->hexagons_T, struct mliHomTraComp, s->num_hexagons);
    return 1;
error:
    return 0;
}

int __mliScenery_malloc_bicircleplane(struct mliScenery *s) {
    mli_malloc(
        s->bicircleplanes,
        struct mliBiCirclePlane,
        s->num_bicircleplanes);
    mli_malloc(
        s->bicircleplanes_surfaces, struct mliSurfaces, s->num_bicircleplanes);
    mli_malloc(
        s->bicircleplanes_T, struct mliHomTraComp, s->num_bicircleplanes);
    return 1;
error:
    return 0;
}

int __mliScenery_malloc_discs(struct mliScenery *s) {
    mli_malloc(s->discs, struct mliDisc, s->num_discs);
    mli_malloc(s->discs_surfaces, struct mliSurfaces, s->num_discs);
    mli_malloc(s->discs_T, struct mliHomTraComp, s->num_discs);
    return 1;
error:
    return 0;
}

int __mliScenery_malloc_functions(struct mliScenery *s) {
    uint64_t i;
    mli_malloc(s->functions, struct mliFunc, s->num_functions);
    for (i = 0; i < s->num_functions; i++) {
        s->functions[i] = mliFunc_init();}
    return 1;
error:
    return 0;
}

int __mliScenery_malloc_colors(struct mliScenery *s) {
    mli_malloc(s->colors, struct mliColor, s->num_colors);
    return 1;
error:
    return 0;
}

int __mliScenery_malloc_surfaces(struct mliScenery *s) {
    mli_malloc(s->surfaces, struct mliSurface, s->num_surfaces);
    return 1;
error:
    return 0;
}

int mliScenery_malloc(struct mliScenery *s) {
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

#endif
