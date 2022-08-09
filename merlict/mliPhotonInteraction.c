/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "mliPhotonInteraction.h"

int mli_photoninteraction_type_to_string(
        const int32_t type,
        char *s,
        const uint64_t s_capacity)
{
        switch (type) {
        case MLI_PHOTON_CREATION:
                chk(snprintf(s, s_capacity - 1, "creation"));
                break;
        case MLI_PHOTON_ABSORBTION:
                chk(snprintf(s, s_capacity - 1, "absorbtion"));
                break;
        case MLI_PHOTON_ABSORBTION_MEDIUM:
                chk(snprintf(s, s_capacity - 1, "absorbtion in medium"));
                break;
        case MLI_PHOTON_FRESNEL_REFLECTION:
                chk(snprintf(s, s_capacity - 1, "Fresnel reflection"));
                break;
        case MLI_PHOTON_REFRACTION:
                chk(snprintf(s, s_capacity - 1, "refraction"));
                break;
        case MLI_PHOTON_SPECULAR_REFLECTION:
                chk(snprintf(s, s_capacity - 1, "specular reflection"));
                break;
        case MLI_PHOTON_DIFFUSE_REFLECTION:
                chk(snprintf(s, s_capacity - 1, "diffuse reflection"));
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
        const double wavelength,
        double *time_of_flight)
{
        double refractive_index;
        chk_msg(mliFunc_evaluate(
                        &materials->media[phisec->medium_coming_from]
                                 .refraction,
                        wavelength,
                        &refractive_index),
                "Failed to eval. refraction for wavelength.");

        (*time_of_flight) = (refractive_index * phisec->distance_of_ray) /
                            MLI_VACUUM_SPPED_OF_LIGHT;
        return 1;
error:
        return 0;
}
