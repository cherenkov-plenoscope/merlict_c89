/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLI_PHOTON_INTERACTION_H_
#define MLI_PHOTON_INTERACTION_H_

#include <stdint.h>
#include "photon.h"
#include "../intersection/intersection_surface_normal.h"
#include "../vec/vec.h"
#include "../materials/materials.h"

enum mli_photon_interaction_types {
        MLI_PHOTON_CREATION = 101u,
        MLI_PHOTON_ABSORPTION = 102u,
        MLI_PHOTON_ABSORPTION_MEDIUM = 103u,
        MLI_PHOTON_FRESNEL_REFLECTION = 104u,
        MLI_PHOTON_REFRACTION = 105u,
        MLI_PHOTON_SPECULAR_REFLECTION = 106u,
        MLI_PHOTON_DIFFUSE_REFLECTION = 107u
};

struct mli_PhotonInteraction {
        int32_t on_geometry_surface;
        struct mli_GeometryId geometry_id;

        struct mli_Vec position;
        struct mli_Vec position_local;
        double distance_of_ray;

        uint64_t medium_coming_from;
        uint64_t medium_going_to;

        int32_t from_outside_to_inside;
        int32_t type;
};

chk_rc mli_photon_time_of_flight(
        const struct mli_Materials *materials,
        const struct mli_PhotonInteraction *phisec,
        const double wavelength,
        double *time_of_flight);
chk_rc mli_photon_interaction_type_to_string(const int32_t type, char *s);
#endif
