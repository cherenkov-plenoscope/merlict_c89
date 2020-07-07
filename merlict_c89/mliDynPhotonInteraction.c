/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "mliDynPhotonInteraction.h"

MLIDYNARRAY_IMPLEMENTATION(mli, PhotonInteraction, struct mliPhotonInteraction)

void mliDynPhotonInteraction_print(
        const struct mliDynPhotonInteraction *history,
        const struct mliScenery *scenery)
{
        uint64_t i;
        int64_t user_id;
        char type_string[1024];
        char out_in[] = "out->in";
        char in_out[] = "in->out";
        printf("History[%ld of %ld]\n",
               history->dyn.size,
               history->dyn.capacity);
        printf("==================\n");

        printf("idx, obj, ids, [   x,   y,   z], type                "
               ", refr.  , abs.   , dist,\n");
        printf("------------------------------------------------"
               "-------------------------\n");

        for (i = 0; i < history->dyn.size; i++) {

                mli_photoninteraction_type_to_string(
                        history->arr[i].type, type_string);
                if (history->arr[i].object_idx >= 0) {
                        user_id = scenery->user_ids[history->arr[i].object_idx];
                } else {
                        user_id = -1;
                }
                printf("% 3ld, % 3ld, % 3ld, "
                       "[% -.3e,% -.3e,% -.3e], %-20s, ",
                       i,
                       history->arr[i].object_idx,
                       user_id,
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
                        printf(" %1.1f, ", history->arr[i].distance_of_ray);
                }

                if (history->arr[i].object_idx >= 0) {
                        if (history->arr[i].from_outside_to_inside) {
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
