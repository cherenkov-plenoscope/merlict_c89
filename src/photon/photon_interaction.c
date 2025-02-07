/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "photon_interaction.h"
#include "../chk/chk.h"
#include "../physics/physics.h"

chk_rc mli_photon_interaction_type_to_string(const int32_t type, char *s)
{
        switch (type) {
        case MLI_PHOTON_CREATION:
                sprintf(s, "creation");
                break;
        case MLI_PHOTON_ABSORPTION:
                sprintf(s, "absorption");
                break;
        case MLI_PHOTON_ABSORPTION_MEDIUM:
                sprintf(s, "absorption in medium");
                break;
        case MLI_PHOTON_FRESNEL_REFLECTION:
                sprintf(s, "Fresnel reflection");
                break;
        case MLI_PHOTON_REFRACTION:
                sprintf(s, "refraction");
                break;
        case MLI_PHOTON_SPECULAR_REFLECTION:
                sprintf(s, "specular reflection");
                break;
        case MLI_PHOTON_DIFFUSE_REFLECTION:
                sprintf(s, "diffuse reflection");
                break;
        default:
                chk_bad("PhotonInteraction.type is unknown.");
                break;
        }
        return CHK_SUCCESS;
chk_error:
        return CHK_FAIL;
}

chk_rc mli_photon_time_of_flight(
        const struct mli_Materials *materials,
        const struct mli_PhotonInteraction *phisec,
        const double wavelength,
        double *time_of_flight)
{
        double refractive_index;
        const uint64_t spc_idx =
                materials->media.array[phisec->medium_coming_from]
                        .refraction_spectrum;
        const struct mli_Func *spc =
                &materials->spectra.array[spc_idx].spectrum;
        chk_msg(mli_Func_evaluate(spc, wavelength, &refractive_index),
                "Failed to eval. refraction for wavelength.");

        (*time_of_flight) = (refractive_index * phisec->distance_of_ray) /
                            MLI_PHYSICS_SPEED_OF_LIGHT_M_PER_S;
        return CHK_SUCCESS;
chk_error:
        return CHK_FAIL;
}
