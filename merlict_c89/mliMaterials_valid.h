/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MERLICT_C89_MLIMATERIALS_VALID_H_
#define MERLICT_C89_MLIMATERIALS_VALID_H_

#include "mliMaterials.h"

int mliMaterials_valid(const struct mliMaterials *materials);
int mliMaterials_valid_surfaces(const struct mliMaterials *materials);
int mliMaterials_valid_media(const struct mliMaterials *materials);
int mliMaterials_valid_boundary_layers(const struct mliMaterials *materials);
#endif
