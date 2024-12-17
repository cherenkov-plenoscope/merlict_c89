/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLI_ATMOSPHERE_H_
#define MLI_ATMOSPHERE_H_

#include <stdint.h>
#include "../color/color.h"
#include "../vec/vec.h"

struct mli_Atmosphere {
        double sunLatitude;
        double sunHourAngle;

        struct mli_Vec sunDirection;
        double sunDistance;
        double sunRadius;

        double earthRadius;
        double atmosphereRadius;

        double Height_Rayleigh;
        double Height_Mie;

        struct mli_Color beta_Rayleigh;
        struct mli_Color beta_Mie;

        uint64_t numSamples;
        uint64_t numSamplesLight;

        double power;
        double altitude;
};

struct mli_Atmosphere mli_Atmosphere_init(void);
void mli_Atmosphere_set_sun_direction(
        struct mli_Atmosphere *self,
        const double sunLatitude,
        const double sunHourAngle);
void mli_Atmosphere_increase_latitude(
        struct mli_Atmosphere *self,
        const double increment);
void mli_Atmosphere_decrease_latitude(
        struct mli_Atmosphere *self,
        const double increment);
void mli_Atmosphere_increase_hours(
        struct mli_Atmosphere *self,
        const double increment);
void mli_Atmosphere_decrease_hours(
        struct mli_Atmosphere *self,
        const double increment);
void mli_Atmosphere_increase_altitude(
        struct mli_Atmosphere *self,
        const double factor);
void mli_Atmosphere_decrease_altitude(
        struct mli_Atmosphere *self,
        const double factor);

struct mli_Color mli_Atmosphere_query(
        const struct mli_Atmosphere *self,
        const struct mli_Vec orig,
        const struct mli_Vec dir);

struct mli_Color mli_Atmosphere_hit_earth_body(
        const struct mli_Atmosphere *self,
        const struct mli_Vec orig,
        const struct mli_Vec dir);

struct mli_Color mli_Atmosphere_hit_outer_atmosphere(
        const struct mli_Atmosphere *self,
        const struct mli_Vec orig,
        const struct mli_Vec dir,
        double tmin,
        double tmax);

struct mli_Color mli_Atmosphere_compute_depth(
        const struct mli_Atmosphere *self,
        const struct mli_Vec orig,
        const struct mli_Vec dir,
        double tmin,
        double tmax);

#endif
