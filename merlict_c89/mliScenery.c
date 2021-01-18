/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "mliScenery.h"

struct mliScenery mliScenery_init(void)
{
        struct mliScenery scn;
        scn.resources = mliSceneryResources_init();

        scn.num_robjects = 0u;
        scn.robjects = NULL;
        scn.robject_user_ids = NULL;

        scn.robject_boundary_layers = NULL;
        scn.robject2root = NULL;
        return scn;
}

void mliScenery_free(struct mliScenery *scenery)
{
        mliSceneryResources_free(&scenery->resources);

        free(scenery->robjects);
        free(scenery->robject_user_ids);
        free(scenery->robject_boundary_layers);
        free(scenery->robject2root);

        (*scenery) = mliScenery_init();
}

int mliScenery_malloc(struct mliScenery *scn, const uint32_t num_robjects)
{
        mliScenery_free(scn);

        scn->num_robjects = num_robjects;

        mli_malloc(scn->robjects, uint32_t, scn->num_robjects);
        mli_malloc(scn->robject_user_ids, uint32_t, scn->num_robjects);
        mli_malloc(
                scn->robject_boundary_layers,
                struct mliBoundaryLayer,
                scn->num_robjects);
        mli_malloc(scn->robject2root, struct mliHomTraComp, scn->num_robjects);

        return 1;
error:
        mliScenery_free(scn);
        return 0;
}
