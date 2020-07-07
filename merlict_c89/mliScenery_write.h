/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MERLICT_C89_MLISCENERY_WRITE_H_
#define MERLICT_C89_MLISCENERY_WRITE_H_

#include <math.h>
#include <stdint.h>

#include "mli_debug.h"
#include "mliScenery.h"
#include "mliSceneryResources_write.h"

int mliScenery_write_to_path(
        const struct mliScenery *scenery,
        const char *path);
int _mliScenery_write_user_ids(const struct mliScenery *scenery, FILE *f);
int mliScenery_write_capacity_to_file(
        const struct mliScenery *scenery,
        FILE *f);
int _mliScenery_write_discs(const struct mliScenery *scenery, FILE *f);
int _mliScenery_write_bicircleplanes(const struct mliScenery *scenery, FILE *f);
int _mliScenery_write_hexagons(const struct mliScenery *scenery, FILE *f);
int _mliScenery_write_cylinders(const struct mliScenery *scenery, FILE *f);
int _mliScenery_write_spheres(const struct mliScenery *scenery, FILE *f);
int _mliScenery_write_spherical_cap_hex(
        const struct mliScenery *scenery,
        FILE *f);
int _mliScenery_write_vertices_and_triangles(
        const struct mliScenery *scenery,
        FILE *f);
#endif
