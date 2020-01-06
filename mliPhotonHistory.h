/* Copyright 2019 Sebastian Achim Mueller */
#ifndef MERLICT_MLIPHOTONHISTORY_H_
#define MERLICT_MLIPHOTONHISTORY_H_

#include <math.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include "mli_debug.h"
#include "mliPhotonInteraction.h"

typedef struct {
    uint32_t num_reserved;
    uint32_t num;
    mliPhotonInteraction *actions;
} mliPhotonHistory;

mliPhotonHistory mliPhotonHistory_init(const uint32_t num_reserved) {
    mliPhotonHistory history;
    history.num_reserved = num_reserved;
    history.num = 0u;
    history.actions = NULL;
    return history;}

void mliPhotonHistory_free(mliPhotonHistory *history) {
    free(history->actions);
    (*history) = mliPhotonHistory_init(0);}

int mliPhotonHistory_malloc(mliPhotonHistory *history) {
    mli_malloc(history->actions, mliPhotonInteraction, history->num_reserved);
    return 1;
error:
    mliPhotonHistory_free(history);
    return 0;}

int mliPhotonHistory_push_back(
    mliPhotonHistory *history,
    const mliPhotonInteraction action) {
    mli_check(history->num < history->num_reserved, "Too many interactions.");
    history->actions[history->num] = action;
    history->num += 1;
    return 1;
error:
    return 0;}

void mliPhotonHistory_print(const mliPhotonHistory *history) {
    int64_t i;
    char type_string[1024];
    char out_in[] = "out->in";
    char in_out[] = "in->out";
    printf("History[%d of %d]\n", history->num, history->num_reserved);
    printf("==================\n");

    printf("idx, obj, [   x,   y,   z], type                , refr.  , abs.   , dist,\n");
    printf("-------------------------------------------------------------------------\n");

    for (i = 0; i < history->num; i++) {

        mli_photoninteraction_type_to_string(
            history->actions[i].type,
            type_string);

        printf("% 3ld, % 3d, [% -1.1f,% -1.1f,% -1.1f], %-20s, ",
            i,
            history->actions[i]._object_idx,
            history->actions[i].position.x,
            history->actions[i].position.y,
            history->actions[i].position.z,
            type_string);

        printf("{% 2d,% 2d}, ",
            history->actions[i].refraction_coming_from,
            history->actions[i].refraction_going_to);

        printf("{% 2d,% 2d}, ",
            history->actions[i].absorbtion_coming_from,
            history->actions[i].absorbtion_going_to);

        if (history->actions[i].type != MLI_PHOTON_CREATION) {
            printf(" %1.1f, ", history->actions[i].distance_of_ray);}


        if (history->actions[i]._object_idx >= 0) {
            if (history->actions[i]._from_outside_to_inside) {
                printf("%s", out_in);
            } else {
                printf("%s", in_out);
            }

        } else {
            printf(" ");
        }
        printf("\n");
    }
}

#endif
