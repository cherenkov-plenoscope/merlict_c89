/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MERLICT_C89_MLIPHOTONINTERACTION_H_
#define MERLICT_C89_MLIPHOTONINTERACTION_H_

#include <stdint.h>
#include "mliIntersectionSurfaceNormal.h"
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
        int32_t on_geometry_surface;
        struct mliGeometryId geometry_id;

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

int mli_time_of_flight(
        const struct mliSceneryResources *materials,
        const struct mliPhotonInteraction *phisec,
        const struct mliPhoton *photon,
        double *time_of_flight);
int mli_photoninteraction_type_to_string(const int32_t type, char *s);
#endif
