/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MERLICT_C89_MLISCENERY_READ_H_
#define MERLICT_C89_MLISCENERY_READ_H_

#include <math.h>
#include <stdint.h>

#include "mli_debug.h"
#include "mliScenery.h"
#include "mliScenery_write.h"
#include "mliSceneryResources_read.h"

int mliScenery_read_from_path(struct mliScenery *scenery, const char *path);
int mliScenery_read_capacity_from_file(struct mliSceneryCapacity *cap, FILE *f);
int _mliScenery_read_user_ids(struct mliScenery *scenery, FILE *f);
int _mliScenery_read_discs(struct mliScenery *scenery, FILE *f);
int _mliScenery_read_bicircleplanes(struct mliScenery *scenery, FILE *f);
int _mliScenery_read_hexagons(struct mliScenery *scenery, FILE *f);
int _mliScenery_read_cylinders(struct mliScenery *scenery, FILE *f);
int _mliScenery_read_spheres(struct mliScenery *scenery, FILE *f);
int _mliScenery_read_spherical_cap_hex(struct mliScenery *scenery, FILE *f);
int _mliScenery_read_vertices_and_triangles(
        struct mliScenery *scenery,
        FILE *f);
#endif
