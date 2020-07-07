/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MERLICT_C89_MLIUSERSCENERY_H_
#define MERLICT_C89_MLIUSERSCENERY_H_

#include <math.h>
#include <stdint.h>

#include "mli_debug.h"
#include "mliScenery.h"
#include "mliSceneryResources.h"
#include "mliFrame.h"
#include "mliMap.h"

struct mliUserScenery {
        struct mliSceneryResources resources;
        struct mliFrame root;
        uint32_t default_medium;

        struct mliMap2 function_names;
        struct mliMap2 color_names;
        struct mliMap2 medium_names;
        struct mliMap2 surface_names;
};

int mliUserScenery_malloc(struct mliUserScenery *uscn);
void mliUserScenery_free(struct mliUserScenery *uscn);
struct mliUserScenery mliUserScenery_init(void);
#endif
