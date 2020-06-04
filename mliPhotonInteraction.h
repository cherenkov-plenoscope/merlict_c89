/* Copyright 2019 Sebastian Achim Mueller */
#ifndef MERLICT_MLIPHOTONINTERACTION_H_
#define MERLICT_MLIPHOTONINTERACTION_H_

#include <math.h>
#include <stdint.h>
#include "mliIntersection.h"
#include "mliVec.h"
#include "mliPhoton.h"

#define MLI_VACUUM_SPPED_OF_LIGHT 299792458.0

#define MLI_PHOTON_CREATION 101u
#define MLI_PHOTON_ABSORBTION 102u
#define MLI_PHOTON_ABSORBTION_MEDIUM 103u

#define MLI_PHOTON_FRESNEL_REFLECTION 104u
#define MLI_PHOTON_REFRACTION 105u

#define MLI_PHOTON_SPECULAR_REFLECTION 106u
#define MLI_PHOTON_DIFFUSE_REFLECTION 107u

struct mliPhotonInteraction {
        int64_t object_idx;

        struct mliVec position;
        struct mliVec position_local;
        double distance_of_ray;

        uint32_t refraction_coming_from;
        uint32_t refraction_going_to;
        uint32_t absorbtion_coming_from;
        uint32_t absorbtion_going_to;

        int32_t from_outside_to_inside;
        int32_t type;
};

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
                mli_sentinel("PhotonInteraction.type is unknown.");
                break;
        }
        return 1;
error:
        return 0;
}

int mli_time_of_flight(
        const struct mliScenery *scenery,
        const struct mliPhotonInteraction *phisec,
        const struct mliPhoton *photon,
        double *time_of_flight)
{
        double refractive_index;
        mli_check(
                mliFunc_evaluate(
                        &scenery->resources
                                 .functions[phisec->refraction_coming_from],
                        photon->wavelength,
                        &refractive_index),
                "Failed to eval. refraction for wavelength.");

        (*time_of_flight) = (refractive_index * phisec->distance_of_ray) /
                            MLI_VACUUM_SPPED_OF_LIGHT;
        return 1;
error:
        return 0;
}

#endif
