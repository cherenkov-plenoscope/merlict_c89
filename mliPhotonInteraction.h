/* Copyright 2019 Sebastian Achim Mueller */
#ifndef MERLICT_MLIPHOTONINTERACTION_H_
#define MERLICT_MLIPHOTONINTERACTION_H_

#include <math.h>
#include <stdint.h>
#include "mliIntersection.h"
#include "mliVec.h"



#define MLI_PHOTON_CREATION               101u
#define MLI_PHOTON_ABSORBTION             102u

#define MLI_PHOTON_FRESNEL_REFLECTION     103u
#define MLI_PHOTON_REFRACTION             104u

#define MLI_PHOTON_SPECULAR_REFLECTION    105u
#define MLI_PHOTON_DIFFUSE_REFLECTION     106u

typedef struct {
    mliVec position;
    uint32_t refraction_coming_from;
    uint32_t refraction_going_to;
    uint32_t absorbtion_coming_from;
    uint32_t absorbtion_going_to;
    double distance_of_ray;
    int32_t _object_idx;
    int8_t _from_outside_to_inside;
    int8_t type;
} mliPhotonInteraction;

int mli_photoninteraction_type_to_string(const int8_t type, char* s) {
    switch (type) {
        case MLI_PHOTON_CREATION: sprintf(s, "creation"); break;
        case MLI_PHOTON_ABSORBTION: sprintf(s, "absorbtion"); break;
        case MLI_PHOTON_FRESNEL_REFLECTION: sprintf(s, "Fresnel reflection"); break;
        case MLI_PHOTON_REFRACTION: sprintf(s, "refraction"); break;
        case MLI_PHOTON_SPECULAR_REFLECTION: sprintf(s, "specular reflection"); break;
        case MLI_PHOTON_DIFFUSE_REFLECTION: sprintf(s, "diffuse reflection"); break;
        default: mli_sentinel("PhotonInteraction.type is unknown."); break;}
    return 1;
error:
    return 0;}

#endif
