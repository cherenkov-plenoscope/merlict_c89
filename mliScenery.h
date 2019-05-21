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
#include "mliSphericalCapHex.h"
#include "mliCylinder.h"
#include "mliHexagon.h"
#include "mliBiCirclePlane.h"
#include "mliSurface.h"
#include "mliSurfaces.h"

#define MLI_TRIANGLE 0u
#define MLI_SPHERICAL_CAP_HEX 1u
#define MLI_SPHERE 2u
#define MLI_CYLINDER 3u
#define MLI_HEXAGON 4u
#define MLI_BICIRCLEPLANE 5u

typedef struct {
    uint32_t num_functions;
    mliFunc *functions;

    uint32_t num_colors;
    mliColor *colors;

    uint32_t num_surfaces;
    mliSurface *surfaces;

    uint32_t num_vertices;
    mliVec *vertices;

    uint32_t num_triangles;
    mliFace *triangles;
    mliSurfaces *triangles_surfaces;

    uint32_t num_spherical_cap_hex;
    mliSphericalCapHex *spherical_cap_hex;
    mliSurfaces *spherical_cap_hex_surfaces;
    mliHomTraComp* spherical_cap_hex_T;

    uint32_t num_spheres;
    double *spheres;
    mliSurfaces *spheres_surfaces;
    mliHomTraComp* spheres_T;

    uint32_t num_cylinders;
    mliCylinder *cylinders;
    mliSurfaces *cylinders_surfaces;
    mliHomTraComp* cylinders_T;

    uint32_t num_hexagons;
    mliHexagon *hexagons;
    mliSurfaces *hexagons_surfaces;
    mliHomTraComp* hexagons_T;

    uint32_t num_bicircleplanes;
    mliBiCirclePlane *bicircleplanes;
    mliSurfaces *bicircleplanes_surfaces;
    mliHomTraComp* bicircleplanes_T;
} mliScenery;

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
}

void __mliScenery_malloc_vertices_and_triangles(mliScenery* scenery) {
    scenery->vertices = (mliVec*)malloc(scenery->num_vertices*sizeof(mliVec));
    scenery->triangles = (mliFace*)malloc(
        scenery->num_triangles*sizeof(mliFace));
    scenery->triangles_surfaces =
        (mliSurfaces*)malloc(scenery->num_triangles*sizeof(mliSurfaces));
}

void __mliScenery_malloc_spherical_cap_hex(mliScenery* scenery) {
    scenery->spherical_cap_hex = (mliSphericalCapHex*)malloc(
        scenery->num_spherical_cap_hex*
        sizeof(mliSphericalCapHex));
    scenery->spherical_cap_hex_surfaces = (mliSurfaces*)malloc(
        scenery->num_spherical_cap_hex*sizeof(mliSurfaces));
    scenery->spherical_cap_hex_T = (mliHomTraComp*)malloc(
        scenery->num_spherical_cap_hex*sizeof(mliHomTraComp));
}

void __mliScenery_malloc_spheres(mliScenery* scenery) {
    scenery->spheres = (double*)malloc(scenery->num_spheres*sizeof(double));
    scenery->spheres_surfaces = (mliSurfaces*)malloc(
        scenery->num_spheres*sizeof(mliSurfaces));
    scenery->spheres_T = (mliHomTraComp*)malloc(
        scenery->num_spheres*sizeof(mliHomTraComp));
}

void __mliScenery_malloc_cylinders(mliScenery* scenery) {
    scenery->cylinders = (mliCylinder*)malloc(
        scenery->num_cylinders*sizeof(mliCylinder));
    scenery->cylinders_surfaces = (mliSurfaces*)malloc(
        scenery->num_cylinders*sizeof(mliSurfaces));
    scenery->cylinders_T = (mliHomTraComp*)malloc(
        scenery->num_cylinders*sizeof(mliHomTraComp));
}

void __mliScenery_malloc_hexagons(mliScenery* scenery) {
    scenery->hexagons = (mliHexagon*)malloc(
        scenery->num_hexagons*sizeof(mliHexagon));
    scenery->hexagons_surfaces = (mliSurfaces*)malloc(
        scenery->num_hexagons*sizeof(mliSurfaces));
    scenery->hexagons_T = (mliHomTraComp*)malloc(
        scenery->num_hexagons*sizeof(mliHomTraComp));
}

void __mliScenery_malloc_bicircleplane(mliScenery* scenery) {
    const uint32_t num = scenery->num_bicircleplanes;
    scenery->bicircleplanes =
        (mliBiCirclePlane*)malloc(num*sizeof(mliBiCirclePlane));
    scenery->bicircleplanes_surfaces =
        (mliSurfaces*)malloc(num*sizeof(mliSurfaces));
    scenery->bicircleplanes_T =
        (mliHomTraComp*)malloc(num*sizeof(mliHomTraComp));
}

void __mliScenery_malloc_functions(mliScenery* scenery) {
    scenery->functions = (mliFunc*)malloc(
        scenery->num_functions*sizeof(mliFunc));
}

void __mliScenery_malloc_colors(mliScenery* scenery) {
    scenery->colors = (mliColor*)malloc(
        scenery->num_colors*sizeof(mliColor));
}

void __mliScenery_malloc_surfaces(mliScenery* scenery) {
    scenery->surfaces = (mliSurface*)malloc(
        scenery->num_surfaces*sizeof(mliSurface));
}

void mliScenery_malloc(mliScenery* scenery) {
    __mliScenery_malloc_functions(scenery);
    __mliScenery_malloc_colors(scenery);
    __mliScenery_malloc_surfaces(scenery);
    __mliScenery_malloc_vertices_and_triangles(scenery);
    __mliScenery_malloc_spherical_cap_hex(scenery);
    __mliScenery_malloc_spheres(scenery);
    __mliScenery_malloc_cylinders(scenery);
    __mliScenery_malloc_hexagons(scenery);
    __mliScenery_malloc_bicircleplane(scenery);
}

void __mliScenery_write_spherical_cap_hex(const mliScenery *scenery, FILE *f) {
    fwrite(
        scenery->spherical_cap_hex,
        sizeof(mliSphericalCapHex),
        scenery->num_spherical_cap_hex,
        f);
    fwrite(
        scenery->spherical_cap_hex_surfaces,
        sizeof(mliSurfaces),
        scenery->num_spherical_cap_hex,
        f);
    fwrite(
        scenery->spherical_cap_hex_T,
        sizeof(mliHomTraComp),
        scenery->num_spherical_cap_hex,
        f);
}

void __mliScenery_write_spheres(const mliScenery *scenery, FILE *f) {
    fwrite(scenery->spheres, sizeof(double), scenery->num_spheres, f);
    fwrite(
        scenery->spheres_surfaces,
        sizeof(mliSurfaces),
        scenery->num_spheres,
        f);
    fwrite(
        scenery->spheres_T,
        sizeof(mliHomTraComp),
        scenery->num_spheres,
        f);
}

void __mliScenery_write_cylinders(const mliScenery *scenery, FILE *f) {
    fwrite(scenery->cylinders, sizeof(mliCylinder), scenery->num_cylinders, f);
    fwrite(
        scenery->cylinders_surfaces,
        sizeof(mliSurfaces),
        scenery->num_cylinders,
        f);
    fwrite(
        scenery->cylinders_T,
        sizeof(mliHomTraComp),
        scenery->num_cylinders,
        f);
}

void __mliScenery_write_hexagons(const mliScenery *scenery, FILE *f) {
    fwrite(scenery->hexagons, sizeof(mliHexagon), scenery->num_hexagons, f);
    fwrite(
        scenery->hexagons_surfaces,
        sizeof(mliSurfaces),
        scenery->num_hexagons,
        f);
    fwrite(
        scenery->hexagons_T,
        sizeof(mliHomTraComp),
        scenery->num_hexagons,
        f);
}

void __mliScenery_write_bicircleplanes(const mliScenery *scenery, FILE *f) {
    fwrite(
        scenery->bicircleplanes,
        sizeof(mliBiCirclePlane),
        scenery->num_bicircleplanes,
        f);
    fwrite(
        scenery->bicircleplanes_surfaces,
        sizeof(mliSurfaces),
        scenery->num_bicircleplanes,
        f);
    fwrite(
        scenery->bicircleplanes_T,
        sizeof(mliHomTraComp),
        scenery->num_bicircleplanes,
        f);
}

int mliScenery_write_to_path(const mliScenery *scenery, const char* path) {
    FILE *f;
    uint64_t i;
    f = fopen(path, "w");
    if (f == NULL) goto close_failure;
    fprintf(f, "merlict_c89\n");

    /* nums */
    fwrite(&scenery->num_functions, sizeof(uint32_t), 1u, f);
    fwrite(&scenery->num_colors, sizeof(uint32_t), 1u, f);
    fwrite(&scenery->num_surfaces, sizeof(uint32_t), 1u, f);
    fwrite(&scenery->num_vertices, sizeof(uint32_t), 1u, f);
    fwrite(&scenery->num_triangles, sizeof(uint32_t), 1u, f);
    fwrite(&scenery->num_spherical_cap_hex, sizeof(uint32_t), 1u, f);
    fwrite(&scenery->num_spheres, sizeof(uint32_t), 1u, f);
    fwrite(&scenery->num_cylinders, sizeof(uint32_t), 1u, f);
    fwrite(&scenery->num_hexagons, sizeof(uint32_t), 1u, f);
    fwrite(&scenery->num_bicircleplanes, sizeof(uint32_t), 1u, f);

    /* functions */
    for (i = 0; i < scenery->num_functions; i++) {
        mliFunc_fwrite(&scenery->functions[i], f);}

    /* colors */
    fwrite(scenery->colors, sizeof(mliColor), scenery->num_colors, f);

    /* surfaces */
    fwrite(scenery->surfaces, sizeof(mliSurface), scenery->num_surfaces, f);

    /* vertices */
    fwrite(scenery->vertices, sizeof(mliVec), scenery->num_vertices, f);

    /* triangles */
    fwrite(scenery->triangles, sizeof(mliFace), scenery->num_triangles, f);
    fwrite(
        scenery->triangles_surfaces,
        sizeof(mliSurfaces),
        scenery->num_triangles,
        f);

    __mliScenery_write_spherical_cap_hex(scenery, f);
    __mliScenery_write_spheres(scenery, f);
    __mliScenery_write_cylinders(scenery, f);
    __mliScenery_write_hexagons(scenery, f);
    __mliScenery_write_bicircleplanes(scenery, f);

    fclose(f);
    return 1;

    close_failure:
    fclose(f);
    return 0;}


void __mliScenery_read_vertices_and_triangles(mliScenery *scenery, FILE* f) {
    fread(scenery->vertices, sizeof(mliVec), scenery->num_vertices, f);
    fread(scenery->triangles, sizeof(mliFace), scenery->num_triangles, f);
    fread(
        scenery->triangles_surfaces,
        sizeof(mliSurfaces),
        scenery->num_triangles,
        f);
}

void __mliScenery_read_spherical_cap_hex(mliScenery *scenery, FILE* f) {
    fread(
        scenery->spherical_cap_hex,
        sizeof(mliSphericalCapHex),
        scenery->num_spherical_cap_hex, f);
    fread(
        scenery->spherical_cap_hex_surfaces,
        sizeof(mliSurfaces),
        scenery->num_spherical_cap_hex, f);
    fread(
        scenery->spherical_cap_hex_T,
        sizeof(mliHomTraComp),
        scenery->num_spherical_cap_hex, f);
}

void __mliScenery_read_spheres(mliScenery *scenery, FILE* f) {
    fread(
        scenery->spheres,
        sizeof(double),
        scenery->num_spheres, f);
    fread(
        scenery->spheres_surfaces,
        sizeof(mliSurfaces),
        scenery->num_spheres, f);
    fread(
        scenery->spheres_T,
        sizeof(mliHomTraComp),
        scenery->num_spheres, f);
}

void __mliScenery_read_cylinders(mliScenery *scenery, FILE* f) {
    fread(
        scenery->cylinders,
        sizeof(mliCylinder),
        scenery->num_cylinders, f);
    fread(
        scenery->cylinders_surfaces,
        sizeof(mliSurfaces),
        scenery->num_cylinders, f);
    fread(
        scenery->cylinders_T,
        sizeof(mliHomTraComp),
        scenery->num_cylinders, f);
}

void __mliScenery_read_hexagons(mliScenery *scenery, FILE* f) {
    fread(
        scenery->hexagons,
        sizeof(mliHexagon),
        scenery->num_hexagons, f);
    fread(
        scenery->hexagons_surfaces,
        sizeof(mliSurfaces),
        scenery->num_hexagons, f);
    fread(
        scenery->hexagons_T,
        sizeof(mliHomTraComp),
        scenery->num_hexagons, f);
}

void __mliScenery_read_bicircleplanes(mliScenery *scenery, FILE* f) {
    fread(
        scenery->bicircleplanes,
        sizeof(mliBiCirclePlane),
        scenery->num_bicircleplanes,
        f);
    fread(
        scenery->bicircleplanes_surfaces,
        sizeof(mliSurfaces),
        scenery->num_bicircleplanes,
        f);
    fread(
        scenery->bicircleplanes_T,
        sizeof(mliHomTraComp),
        scenery->num_bicircleplanes,
        f);
}

int mliScenery_read_from_path(mliScenery *scenery, const char* path) {
    FILE *f;
    uint64_t i;
    char line[64];
    f = fopen(path, "r");
    if (f == NULL) return 0;
    if (fgets(line, 1024, f) == NULL) goto close_failure;
    if (strcmp(line, "merlict_c89\n") != 0) goto close_failure;

    /* nums */
    fread(&scenery->num_functions, sizeof(uint32_t), 1u, f);
    fread(&scenery->num_colors, sizeof(uint32_t), 1u, f);
    fread(&scenery->num_surfaces, sizeof(uint32_t), 1u, f);
    fread(&scenery->num_vertices, sizeof(uint32_t), 1u, f);
    fread(&scenery->num_triangles, sizeof(uint32_t), 1u, f);
    fread(&scenery->num_spherical_cap_hex, sizeof(uint32_t), 1u, f);
    fread(&scenery->num_spheres, sizeof(uint32_t), 1u, f);
    fread(&scenery->num_cylinders, sizeof(uint32_t), 1u, f);
    fread(&scenery->num_hexagons, sizeof(uint32_t), 1u, f);
    fread(&scenery->num_bicircleplanes, sizeof(uint32_t), 1u, f);

    mliScenery_malloc(scenery);

    /* functions */
    for (i = 0; i < scenery->num_functions; i++) {
        mliFunc_fread(&scenery->functions[i], f);}

    /* colors */
    fread(scenery->colors, sizeof(mliColor), scenery->num_colors, f);

    /* surfaces */
    fread(scenery->surfaces, sizeof(mliSurface), scenery->num_surfaces, f);

    __mliScenery_read_vertices_and_triangles(scenery, f);
    __mliScenery_read_spherical_cap_hex(scenery, f);
    __mliScenery_read_spheres(scenery, f);
    __mliScenery_read_cylinders(scenery, f);
    __mliScenery_read_hexagons(scenery, f);
    __mliScenery_read_bicircleplanes(scenery, f);

    fclose(f);
    return 1;

    close_failure:
    fclose(f);
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
        if (a->spheres[i] != b->spheres[i])
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
    return 1;}


int mliScenery_valid_surfaces(const mliScenery *scenery) {
    uint64_t i;
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
    return 1;}

#endif
