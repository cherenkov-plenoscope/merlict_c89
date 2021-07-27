/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "mliDynPhotonInteraction.h"

MLIDYNARRAY_IMPLEMENTATION(mli, PhotonInteraction, struct mliPhotonInteraction)

void mliDynPhotonInteraction_print(
        const struct mliDynPhotonInteraction *history,
        const struct mliGeometry *scenery)
{
        uint64_t i;
        char type_string[1024];
        char out_in[] = "out->in";
        char in_out[] = "in->out";
        printf("History(%usize, %ucapacity)\n",
               (uint32_t)history->size,
               (uint32_t)history->capacity);
        printf("==================\n");

        printf("   #  "
               "(   id; robj,  obj, face)  "
               "[     x/m,     y/m,     z/m]  "
               "type         "
               "refr.    "
               "abs.    "
               "dist/m"
               "\n");
        printf("-----------------------------------------------------"
               "-----------------------------------------------------\n");

        for (i = 0; i < history->size; i++) {
                struct mliPhotonInteraction phisec = history->array[i];
                printf(" % 3d  ", (int32_t)i);

                if (phisec.on_geometry_surface == 1) {
                        printf("(% 5d;% 5d,% 5d,% 5d)  ",
                               scenery->robject_ids[phisec.geometry_id.robj],
                               phisec.geometry_id.robj,
                               scenery->robjects[phisec.geometry_id.robj],
                               phisec.geometry_id.face);
                } else {
                        printf("            n/a            ");
                }

                printf("[% -.1e,% -.1e,% -.1e]  ",
                       phisec.position.x,
                       phisec.position.y,
                       phisec.position.z);

                mli_photoninteraction_type_to_string(phisec.type, type_string);

                printf("%-12s ", type_string);

                printf("{% 2d,% 2d}  ",
                       phisec.refraction_coming_from,
                       phisec.refraction_going_to);

                printf("{% 2d,% 2d}  ",
                       phisec.absorbtion_coming_from,
                       phisec.absorbtion_going_to);

                if (phisec.type == MLI_PHOTON_CREATION) {
                        printf(" n/a  ");
                } else {
                        printf(" %1.1e  ", phisec.distance_of_ray);
                }

                if (phisec.on_geometry_surface == 1) {
                        if (phisec.from_outside_to_inside) {
                                printf("%s", out_in);
                        } else {
                                printf("%s", in_out);
                        }
                } else {
                        printf("n/a  ");
                }
                printf("\n");
        }
}
