/* Copyright 2019 Sebastian Achim Mueller */
#ifndef MERLICT_MLIPHOTONHISTORY_H_
#define MERLICT_MLIPHOTONHISTORY_H_

#include <math.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include "mli_debug.h"
#include "mliIntersection.h"

typedef struct {
    uint32_t num_reserved;
    uint32_t num;
    mliIntersection *sections;
    int32_t *actions;
} mliPhotonHistory;

mliPhotonHistory mliPhotonHistory_init(const uint32_t num_reserved) {
    mliPhotonHistory history;
    history.num_reserved = num_reserved;
    history.num = 0u;
    history.sections = NULL;
    history.actions = NULL;
    return history;}

void mliPhotonHistory_free(mliPhotonHistory *history) {
    free(history->sections);
    free(history->actions);
    (*history) = mliPhotonHistory_init(0);}

int mliPhotonHistory_malloc(mliPhotonHistory *history) {
    mli_malloc(history->sections, mliIntersection, history->num_reserved);
    mli_malloc(history->actions, int32_t, history->num_reserved);
    return 1;
error:
    mliPhotonHistory_free(history);
    return 0;}

#endif
