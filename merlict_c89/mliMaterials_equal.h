/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MERLICT_C89_MLIMATERIALS_EQUAL_H_
#define MERLICT_C89_MLIMATERIALS_EQUAL_H_

#include "mliMaterials.h"

int mliMaterials_equal(
        const struct mliMaterials *a,
        const struct mliMaterials *b);
int mliMaterials_surfaces_equal(
        const struct mliMaterials *a,
        const struct mliMaterials *b);
int mliMaterials_media_equal(
        const struct mliMaterials *a,
        const struct mliMaterials *b);
int mliMaterials_boundary_layers_equal(
        const struct mliMaterials *a,
        const struct mliMaterials *b);
#endif
