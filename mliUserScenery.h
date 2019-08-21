/* Copyright 2019 Sebastian Achim Mueller */
#ifndef MERLICT_USERSCENERY_H_
#define MERLICT_USERSCENERY_H_

#include <math.h>
#include <stdint.h>
#include "mli_debug.h"
#include "mliScenery.h"
#include "mliFrame.h"

/*
    A scenery as a user wants to define it.
    It can grow dynamically.
    It can represent the geometric hiracrhy between abstract frames.
    Before ray tracing, this UserScenery will be translated into a Scenery
    which is optimized for ray tracing.
*/
typedef struct {
    mliScenery surface_resources; /* stores only the surfaces */
    mliFrame root;
} mliUserScenery;

mliUserScenery mliUserScenery_init() {
    mliUserScenery uscn;
    uscn.surface_resources = mliScenery_init();
    uscn.root = mliFrame_init();
    return uscn;}

void mliUserScenery_free(mliUserScenery* uscn) {
    mliScenery_free(&uscn->surface_resources);
    mliFrame_free(&uscn->root);}

int mliUserScenery_malloc(mliUserScenery* uscn) {
    mli_check(
        mliScenery_malloc(&uscn->surface_resources),
        "Can not allocate surface_resources in UserScenery.");
    mli_check(
        mliFrame_malloc(&uscn->root, MLI_FRAME),
        "Can not allocate root-frame in UserScenery.")
    return 1;
error:
    return 0;}

#endif
