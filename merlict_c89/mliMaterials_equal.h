/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MERLICT_C89_MLIMATERIALS_EQUAL_H_
#define MERLICT_C89_MLIMATERIALS_EQUAL_H_

#include <math.h>
#include <stdint.h>

#include "mli_debug.h"
#include "mliMaterials.h"

int mliMaterials_equal(
        const struct mliMaterials *a,
        const struct mliMaterials *b);
int _mliMaterials_surfaces_equal(
        const struct mliMaterials *a,
        const struct mliMaterials *b);
int _mliMaterials_media_equal(
        const struct mliMaterials *a,
        const struct mliMaterials *b);
int _mliMaterials_colors_equal(
        const struct mliMaterials *a,
        const struct mliMaterials *b);
int _mliMaterials_functions_equal(
        const struct mliMaterials *a,
        const struct mliMaterials *b);
#endif
