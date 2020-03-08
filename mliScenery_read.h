/* Copyright 2019 Sebastian Achim Mueller */
#ifndef MERLICT_MLISCENERY_READ_H_
#define MERLICT_MLISCENERY_READ_H_

#include <math.h>
#include <stdint.h>
#include "mli_debug.h"
#include "mliScenery.h"
#include "mliScenery_write.h"

int __mliScenery_read_vertices_and_triangles(
        struct mliScenery *scenery,
        FILE* f)
{
        mli_fread(
                scenery->vertices, sizeof(struct mliVec), scenery->num_vertices, f);
        mli_fread(
                scenery->triangles, sizeof(struct mliFace), scenery->num_triangles, f);
        mli_fread(
                scenery->triangles_surfaces,
                sizeof(struct mliSurfaces),
                scenery->num_triangles,
                f);
        return 1;
error:
        return 0;
}

int __mliScenery_read_spherical_cap_hex(struct mliScenery *scenery, FILE* f) {
        mli_fread(
                scenery->spherical_cap_hex,
                sizeof(struct mliSphericalCapHex),
                scenery->num_spherical_cap_hex, f);
        mli_fread(
                scenery->spherical_cap_hex_surfaces,
                sizeof(struct mliSurfaces),
                scenery->num_spherical_cap_hex, f);
        mli_fread(
                scenery->spherical_cap_hex_T,
                sizeof(struct mliHomTraComp),
                scenery->num_spherical_cap_hex, f);
        return 1;
error:
        return 0;
}

int __mliScenery_read_spheres(struct mliScenery *scenery, FILE* f) {
        mli_fread(
                scenery->spheres,
                sizeof(double),
                scenery->num_spheres, f);
        mli_fread(
                scenery->spheres_surfaces,
                sizeof(struct mliSurfaces),
                scenery->num_spheres, f);
        mli_fread(
                scenery->spheres_T,
                sizeof(struct mliHomTraComp),
                scenery->num_spheres, f);
        return 1;
error:
        return 0;
}

int __mliScenery_read_cylinders(struct mliScenery *scenery, FILE* f) {
        mli_fread(
                scenery->cylinders,
                sizeof(struct mliCylinder),
                scenery->num_cylinders, f);
        mli_fread(
                scenery->cylinders_surfaces,
                sizeof(struct mliSurfaces),
                scenery->num_cylinders, f);
        mli_fread(
                scenery->cylinders_T,
                sizeof(struct mliHomTraComp),
                scenery->num_cylinders, f);
        return 1;
error:
        return 0;
}

int __mliScenery_read_hexagons(struct mliScenery *scenery, FILE* f) {
        mli_fread(
                scenery->hexagons,
                sizeof(struct mliHexagon),
                scenery->num_hexagons, f);
        mli_fread(
                scenery->hexagons_surfaces,
                sizeof(struct mliSurfaces),
                scenery->num_hexagons, f);
        mli_fread(
                scenery->hexagons_T,
                sizeof(struct mliHomTraComp),
                scenery->num_hexagons, f);
        return 1;
error:
        return 0;
}

int __mliScenery_read_bicircleplanes(struct mliScenery *scenery, FILE* f) {
        mli_fread(
                scenery->bicircleplanes,
                sizeof(struct mliBiCirclePlane),
                scenery->num_bicircleplanes,
                f);
        mli_fread(
                scenery->bicircleplanes_surfaces,
                sizeof(struct mliSurfaces),
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

int __mliScenery_read_discs(struct mliScenery *scenery, FILE* f) {
        mli_fread(
                scenery->discs,
                sizeof(struct mliDisc),
                scenery->num_discs,
                f);
        mli_fread(
                scenery->discs_surfaces,
                sizeof(struct mliSurfaces),
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

int mliScenery_read_from_path(struct mliScenery *scenery, const char* path) {
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
        mli_fread(scenery->colors, sizeof(struct mliColor), scenery->num_colors, f);

        /* surfaces */
        mli_fread(
                scenery->surfaces, sizeof(struct mliSurface), scenery->num_surfaces, f);

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

#endif
