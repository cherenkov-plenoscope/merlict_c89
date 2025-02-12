/* Copyright 2016 Sebastian A. Mueller */
#include "CorsikaPhotonBunch.h"
#include "../math/math.h"
#include "../physics/physics.h"

MLI_VECTOR_IMPLEMENTATION(
        mliDynCorsikaPhotonBunch,
        struct mli_corsika_PhotonBunch)

void mli_corsika_PhotonBunch_set_from_raw(
        struct mli_corsika_PhotonBunch *bunch,
        const float *raw)
{
        bunch->x_cm = raw[0];
        bunch->y_cm = raw[1];
        bunch->ux = raw[2];
        bunch->vy = raw[3];
        bunch->time_ns = raw[4];
        bunch->z_emission_cm = raw[5];
        bunch->weight_photons = raw[6];
        bunch->wavelength_nm = raw[7];
}

void mli_corsika_PhotonBunch_to_raw(
        const struct mli_corsika_PhotonBunch *bunch,
        float *raw)
{
        raw[0] = bunch->x_cm;
        raw[1] = bunch->y_cm;
        raw[2] = bunch->ux;
        raw[3] = bunch->vy;
        raw[4] = bunch->time_ns;
        raw[5] = bunch->z_emission_cm;
        raw[6] = bunch->weight_photons;
        raw[7] = bunch->wavelength_nm;
}

struct mli_Photon mli_corsika_PhotonBunch_to_merlict_photon(
        const struct mli_corsika_PhotonBunch bunch,
        const double production_distance_offset,
        const int64_t id)
{
        /*
        Returns an mli_Photon that will reach the observation-level in
        the same way as the corsika-photon-bunch. The weight of the
        corsika-photon-bunch is not taken into account here.

        Parameters
        ----------
        bunch :
                The corsika-photon-bunch
        production_distance_offset : double
                An arbitrary distance for the photon to travel until they
                reach the observation-level. If 0.0, the distance for a
                merlict photon is only defined by the relative arrival time
                on the observation-level.
                Ensure this offset is at least as big as your detector system
                so that photons do not start inside your detector.
        id : int64
                The photon's id.
        */

        const double vacuum_speed_of_light =
                MLI_PHYSICS_SPEED_OF_LIGHT_M_PER_S();
        const struct mli_Vec photon_direction_of_motion =
                mli_corsika_photon_direction_of_motion(bunch);

        const struct mli_Ray ray_running_upwards_to_production = mli_Ray_set(
                mli_corsika_photon_support_on_observation_level(bunch),
                mli_Vec_multiply(photon_direction_of_motion, -1.0));

        const double offset =
                (production_distance_offset +
                 vacuum_speed_of_light *
                         mli_corsika_photon_relative_arrival_time_on_observation_level(
                                 bunch));

        const struct mli_Vec photon_emission_position =
                mli_Ray_at(&ray_running_upwards_to_production, offset);

        struct mli_Photon photon;
        photon.ray.support = photon_emission_position;
        photon.ray.direction = photon_direction_of_motion;
        photon.wavelength = mli_corsika_photon_wavelength(bunch);
        photon.id = id;
        return photon;
}

struct mli_Vec mli_corsika_photon_direction_of_motion(
        const struct mli_corsika_PhotonBunch bunch)
{ /*
       KIT-CORSIKA coordinate-system

                         /\ z-axis
                         |
                         |\ p
                         | \ a
                         |  \ r
                         |   \ t
                         |    \ i
                         |     \ c
                         |      \ l
                         |       \ e
                         |        \
                         |  theta  \ m
                         |       ___\ o
                         |___----    \ m      ___
                         |            \ e       /| y-axis (west)
                         |             \ n    /
                         |              \ t /
                         |               \/u
                         |              / \ m
                         |            /    \
                         |          /       \
                         |        /__________\
                         |      /      ___---/
                         |    /   __---    /
                         |  /__--- phi \ /
         ________________|/--__________/______\ x-axis (north)
                        /|                    /
                      /  |
                    /    |
                  /


          Extensive Air Shower Simulation with CORSIKA, Figure 1, page 114
          (Version 7.6400 from December 27, 2017)

          Direction-cosines:

          cx = sin(theta) * cos(phi)
          cy = sin(theta) * sin(phi)

          The zenith-angle theta opens relative to the negative z-axis.

          It is the momentum of the Cherenkov-photon, which is pointing
          down towards the observation-plane.
  */
        const double z = sqrt(1.0 - bunch.ux * bunch.ux - bunch.vy * bunch.vy);
        return mli_Vec_set(bunch.ux, bunch.vy, -z);
}

struct mli_Vec mli_corsika_photon_support_on_observation_level(
        const struct mli_corsika_PhotonBunch bunch)
{
        return mli_Vec_set(
                (double)bunch.x_cm * 1e-2, (double)bunch.y_cm * 1e-2, 0.0);
}

double mli_corsika_photon_wavelength(const struct mli_corsika_PhotonBunch bunch)
{
        return fabs((double)bunch.wavelength_nm * 1e-9);
}

double mli_corsika_photon_emission_height(
        const struct mli_corsika_PhotonBunch bunch)
{
        return (double)bunch.z_emission_cm * 1e-2;
}

double mli_corsika_photon_relative_arrival_time_on_observation_level(
        const struct mli_corsika_PhotonBunch bunch)
{
        return (double)bunch.time_ns * 1e-9;
}
