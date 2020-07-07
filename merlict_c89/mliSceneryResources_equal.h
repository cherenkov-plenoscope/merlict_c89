/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MERLICT_C89_MLISCENERYRESOURCES_EQUAL_H_
#define MERLICT_C89_MLISCENERYRESOURCES_EQUAL_H_

#include <math.h>
#include <stdint.h>

#include "mli_debug.h"
#include "mliSceneryResources.h"

int mliSceneryResources_equal(
        const struct mliSceneryResources *a,
        const struct mliSceneryResources *b);
int _mliSceneryResources_surfaces_equal(
        const struct mliSceneryResources *a,
        const struct mliSceneryResources *b);
int _mliSceneryResources_media_equal(
        const struct mliSceneryResources *a,
        const struct mliSceneryResources *b);
int _mliSceneryResources_colors_equal(
        const struct mliSceneryResources *a,
        const struct mliSceneryResources *b);
int _mliSceneryResources_functions_equal(
        const struct mliSceneryResources *a,
        const struct mliSceneryResources *b);
#endif
