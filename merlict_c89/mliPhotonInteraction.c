/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "mliPhotonInteraction.h"

int mli_photoninteraction_type_to_string(const int32_t type, char *s)
{
        switch (type) {
        case MLI_PHOTON_CREATION:
                sprintf(s, "creation");
                break;
        case MLI_PHOTON_ABSORBTION:
                sprintf(s, "absorbtion");
                break;
        case MLI_PHOTON_ABSORBTION_MEDIUM:
                sprintf(s, "absorbtion in medium");
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
        return 1;
error:
        return 0;
}

int mli_time_of_flight(
        const struct mliMaterials *materials,
        const struct mliPhotonInteraction *phisec,
        const struct mliPhoton *photon,
        double *time_of_flight)
{
        double refractive_index;
        chk_msg(mliFunc_evaluate(
                        &materials->media[phisec->medium_coming_from]
                                 .refraction,
                        photon->wavelength,
                        &refractive_index),
                "Failed to eval. refraction for wavelength.");

        (*time_of_flight) = (refractive_index * phisec->distance_of_ray) /
                            MLI_VACUUM_SPPED_OF_LIGHT;
        return 1;
error:
        return 0;
}
