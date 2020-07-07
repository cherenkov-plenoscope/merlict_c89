/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MERLICT_C89_MLISCENERY_VALID_H_
#define MERLICT_C89_MLISCENERY_VALID_H_

#include "mliScenery.h"
#include "mliSceneryResources_valid.h"

int mliScenery_valid(const struct mliScenery *scenery);
int _mliScenery_valid_user_ids(const struct mliScenery *scenery);
int _mliScenery_valid_disc(const struct mliScenery *scenery);
int _mliScenery_valid_bicircleplanes(const struct mliScenery *scenery);
int _mliScenery_valid_hexagons(const struct mliScenery *scenery);
int _mliScenery_valid_cylinders(const struct mliScenery *scenery);
int _mliScenery_valid_spheres(const struct mliScenery *scenery);
int _mliScenery_valid_spherical_cap_hex(const struct mliScenery *scenery);
int _mliScenery_valid_triangles(const struct mliScenery *scenery);
#endif
