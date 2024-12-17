/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "photon_interaction_vector.h"
#include "../chk/chk.h"
#include <stdlib.h>

MLI_VECTOR_IMPLEMENTATION(mli_PhotonInteractionVector, struct mli_PhotonInteraction)

int mli_PhotonInteractionVector_time_of_flight(
        const struct mli_PhotonInteractionVector *history,
        const struct mliScenery *scenery,
        const double wavelength,
        double *total_time_of_flight)
{
        uint64_t i;
        (*total_time_of_flight) = 0.0;
        for (i = 0; i < history->size; i++) {
                double time_of_flight = 0.0;
                chk_msg(mli_photon_time_of_flight(
                                &scenery->materials,
                                &history->array[i],
                                wavelength,
                                &time_of_flight),
                        "Can't estimate time-of-flight.");
                (*total_time_of_flight) += time_of_flight;
        }
        return 1;
chk_error:
        return 0;
}

void mli_PhotonInteractionVector_print(
        const struct mli_PhotonInteractionVector *history,
        const struct mliScenery *scenery)
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
                struct mli_PhotonInteraction phisec = history->array[i];
                printf(" % 3d  ", (int32_t)i);

                if (phisec.on_geometry_surface == 1) {
                        printf("(% 5d;% 5d,% 5d,% 5d)  ",
                               scenery->geometry
                                       .robject_ids[phisec.geometry_id.robj],
                               phisec.geometry_id.robj,
                               scenery->geometry
                                       .robjects[phisec.geometry_id.robj],
                               phisec.geometry_id.face);
                } else {
                        printf("            n/a            ");
                }

                printf("[% -.1e,% -.1e,% -.1e]  ",
                       phisec.position.x,
                       phisec.position.y,
                       phisec.position.z);

                mli_photon_interaction_type_to_string(phisec.type, type_string);

                printf("%-24s ", type_string);

                printf("{%-12s,%-12s}  ",
                       scenery->materials
                               .medium_names[phisec.medium_coming_from]
                               .array,
                       scenery->materials.medium_names[phisec.medium_going_to]
                               .array);

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
                        printf("n/a");
                }
                printf("\n");
        }
}
