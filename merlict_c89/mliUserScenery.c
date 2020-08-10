/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "mliUserScenery.h"

/*
 *  A scenery as a user wants to define it.
 *  It can grow dynamically.
 *  It can represent the geometric hiracrhy between abstract frames.
 *  Before ray tracing, this UserScenery will be translated into a Scenery
 *  which is optimized for ray tracing.
 */

struct mliUserScenery mliUserScenery_init(void)
{
        struct mliUserScenery uscn;
        uscn.resources = mliSceneryResources_init();
        uscn.root = mliFrame_init();

        uscn.function_names = mliMap2_init();
        uscn.color_names = mliMap2_init();
        uscn.medium_names = mliMap2_init();
        uscn.surface_names = mliMap2_init();
        uscn.default_medium = 0;
        return uscn;
}

void mliUserScenery_free(struct mliUserScenery *uscn)
{
        mliSceneryResources_free(&uscn->resources);
        mliFrame_free(&uscn->root);

        mliMap2_free(&uscn->function_names);
        mliMap2_free(&uscn->color_names);
        mliMap2_free(&uscn->medium_names);
        mliMap2_free(&uscn->surface_names);
}

int mliUserScenery_malloc(struct mliUserScenery *uscn)
{
        mliUserScenery_free(uscn);
        mli_check(
                mliFrame_malloc(&uscn->root, MLI_FRAME),
                "Can not allocate root-frame in UserScenery.") return 1;
error:
        return 0;
}