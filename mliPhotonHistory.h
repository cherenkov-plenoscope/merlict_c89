/* Copyright 2019 Sebastian Achim Mueller */
#ifndef MERLICT_MLIPHOTONHISTORY_H_
#define MERLICT_MLIPHOTONHISTORY_H_

#include <math.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include "mli_debug.h"
#include "mliPhotonInteraction.h"

MLIDYNARRAY_TEMPLATE(mli, PhotonInteraction, struct mliPhotonInteraction)

/*
struct mliPhotonHistory {
    uint32_t num_reserved;
    uint32_t num;
    struct mliPhotonInteraction *actions;
};

struct mliPhotonHistory mliPhotonHistory_init(const uint32_t num_reserved) {
    struct mliPhotonHistory history;
    history.num_reserved = num_reserved;
    history.num = 0u;
    history.actions = NULL;
    return history;}

void mliPhotonHistory_free(struct mliPhotonHistory *history) {
    free(history->actions);
    (*history) = mliPhotonHistory_init(0);}

int mliPhotonHistory_malloc(struct mliPhotonHistory *history) {
    mli_malloc(
        history->actions, struct mliPhotonInteraction, history->num_reserved);
    return 1;
error:
    mliPhotonHistory_free(history);
    return 0;}

int mliPhotonHistory_push_back(
    struct mliPhotonHistory *history,
    const struct mliPhotonInteraction action) {
    mli_check(history->num < history->num_reserved, "Too many interactions.");
    history->actions[history->num] = action;
    history->num += 1;
    return 1;
error:
    return 0;}
*/

void mliDynPhotonInteraction_print(
    const struct mliDynPhotonInteraction *history)
{
    int64_t i;
    char type_string[1024];
    char out_in[] = "out->in";
    char in_out[] = "in->out";
    printf("History[%ld of %ld]\n", history->dyn.size, history->dyn.capacity);
    printf("==================\n");

    printf("idx, obj, [   x,   y,   z], type                , refr.  , abs.   , dist,\n");
    printf("-------------------------------------------------------------------------\n");

    for (i = 0; i < history->dyn.size; i++) {

        mli_photoninteraction_type_to_string(
            history->arr[i].type,
            type_string);

        printf("% 3ld, % 3d, [% -1.1f,% -1.1f,% -1.1f], %-20s, ",
            i,
            history->arr[i]._object_idx,
            history->arr[i].position.x,
            history->arr[i].position.y,
            history->arr[i].position.z,
            type_string);

        printf("{% 2d,% 2d}, ",
            history->arr[i].refraction_coming_from,
            history->arr[i].refraction_going_to);

        printf("{% 2d,% 2d}, ",
            history->arr[i].absorbtion_coming_from,
            history->arr[i].absorbtion_going_to);

        if (history->arr[i].type != MLI_PHOTON_CREATION) {
            printf(" %1.1f, ", history->arr[i].distance_of_ray);}


        if (history->arr[i]._object_idx >= 0) {
            if (history->arr[i]._from_outside_to_inside) {
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
