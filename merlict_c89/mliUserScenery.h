/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MERLICT_C89_MLIUSERSCENERY_H_
#define MERLICT_C89_MLIUSERSCENERY_H_

#include <math.h>
#include <stdint.h>

#include "mli_debug.h"
#include "mliSceneryResources.h"
#include "mliFrame.h"
#include "mliDynMap.h"

struct mliUserScenery {
        struct mliSceneryResources resources;
        struct mliFrame root;

        struct mliDynMap object_names;
        struct mliDynMap function_names;
        struct mliDynMap color_names;
        struct mliDynMap medium_names;
        struct mliDynMap surface_names;
};

int mliUserScenery_malloc(struct mliUserScenery *uscn);
void mliUserScenery_free(struct mliUserScenery *uscn);
struct mliUserScenery mliUserScenery_init(void);
#endif
