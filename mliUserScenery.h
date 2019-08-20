/* Copyright 2019 Sebastian Achim Mueller */
#ifndef MERLICT_USERSCENERY_H_
#define MERLICT_USERSCENERY_H_

#include <math.h>
#include <stdint.h>
#include "mli_debug.h"
#include "mliColor.h"
#include "mliFunc.h"
#include "mliFrame.h"
#include "mliSurface.h"
#include "mliSurfaces.h"
#include "mliVector.h"

/*
    A scenery as a user wants to define it.
    It can grow dynamically.
    It can represent the geometric hiracrhy between abstract frames.
    Before ray tracing, this UserScenery will be translated into a Scenery
    which is optimized for ray tracing.
*/
typedef struct {
    mliVector functions;
    mliVector colors;
    mliVector surfaces;
    mliFrame root;
} mliUserScenery;

mliUserScenery mliUserScenery_init() {
    mliUserScenery uscn;
    uscn.functions = mliVector_init();
    uscn.colors = mliVector_init();
    uscn.surfaces = mliVector_init();
    uscn.root = mliFrame_init();
    return uscn;}

void mliUserScenery_free(mliUserScenery* uscn) {
    uint64_t c;
    for (c = 0; c < uscn->functions.size; c++) {
        mliFunc func = *(mliFunc*)mliVector_at(&uscn->functions, c);
        mliFunc_free(&func);}
    mliVector_free(&uscn->functions);
    mliVector_free(&uscn->colors);
    mliVector_free(&uscn->surfaces);
    mliFrame_free(&uscn->root);}

int mliUserScenery_malloc(mliUserScenery* uscn) {
    mli_check(
        mliVector_malloc(&uscn->functions, 0u, sizeof(mliFunc)),
        "Can not allocate functions.");
    mli_check(
        mliVector_malloc(&uscn->colors, 0u, sizeof(mliColor)),
        "Can not allocate colors.");
    mli_check(
        mliVector_malloc(&uscn->surfaces, 0u, sizeof(mliSurface)),
        "Can not allocate surfaces.");
    mli_check(
        mliFrame_malloc_as_type(&uscn->root, MLI_FRAME),
        "Can not allocate root-frame.")
    return 1;
error:
    return 0;}

#endif
