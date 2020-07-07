/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MERLICT_C89_MLISCENERYRESOURCES_VALID_H_
#define MERLICT_C89_MLISCENERYRESOURCES_VALID_H_

#include "mliSceneryResources.h"

int mliSceneryResources_valid(const struct mliSceneryResources *res);
int _mliSceneryResources_valid_surfaces(const struct mliSceneryResources *res);
int _mliSceneryResources_valid_media(const struct mliSceneryResources *res);
#endif
