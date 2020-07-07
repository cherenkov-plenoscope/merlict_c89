/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MERLICT_C89_MLISCENERY_EQUAL_H_
#define MERLICT_C89_MLISCENERY_EQUAL_H_

#include "mliScenery.h"
#include "mliSceneryResources_equal.h"

int mliScenery_is_equal(const struct mliScenery *a, const struct mliScenery *b);
int _mliScenery_user_ids_equal(
        const struct mliScenery *a,
        const struct mliScenery *b);
int _mliScenery_discs_equal(
        const struct mliScenery *a,
        const struct mliScenery *b);
int _mliScenery_bicircleplanes_equal(
        const struct mliScenery *a,
        const struct mliScenery *b);
int _mliScenery_hexagons_equal(
        const struct mliScenery *a,
        const struct mliScenery *b);
int _mliScenery_cylinders_equal(
        const struct mliScenery *a,
        const struct mliScenery *b);
int _mliScenery_spheres_equal(
        const struct mliScenery *a,
        const struct mliScenery *b);
int _mliScenery_spherical_cap_hex_equal(
        const struct mliScenery *a,
        const struct mliScenery *b);
int _mliScenery_triangles_equal(
        const struct mliScenery *a,
        const struct mliScenery *b);
int _mliScenery_vertices_equal(
        const struct mliScenery *a,
        const struct mliScenery *b);
#endif
