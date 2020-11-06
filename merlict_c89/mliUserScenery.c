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

        uscn.object_names = mliDynMap_init();
        uscn.function_names = mliDynMap_init();
        uscn.color_names = mliDynMap_init();
        uscn.medium_names = mliDynMap_init();
        uscn.surface_names = mliDynMap_init();
        uscn.default_medium = 0;
        return uscn;
}

void mliUserScenery_free(struct mliUserScenery *uscn)
{
        mliSceneryResources_free(&uscn->resources);
        mliFrame_free(&uscn->root);

        mliDynMap_free(&uscn->object_names);
        mliDynMap_free(&uscn->function_names);
        mliDynMap_free(&uscn->color_names);
        mliDynMap_free(&uscn->medium_names);
        mliDynMap_free(&uscn->surface_names);
        (*uscn) = mliUserScenery_init();
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
