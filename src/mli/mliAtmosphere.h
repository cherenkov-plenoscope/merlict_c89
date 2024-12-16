/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLIATMOSPHERE_H_
#define MLIATMOSPHERE_H_

#include <stdint.h>
#include "../color/color.h"
#include "../vec/vec.h"

struct mliAtmosphere {
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

struct mliAtmosphere mliAtmosphere_init(void);
void mliAtmosphere_set_sun_direction(
        struct mliAtmosphere *atmosphere,
        const double sunLatitude,
        const double sunHourAngle);
void mliAtmosphere_increase_latitude(
        struct mliAtmosphere *atmosphere,
        const double increment);
void mliAtmosphere_decrease_latitude(
        struct mliAtmosphere *atmosphere,
        const double increment);
void mliAtmosphere_increase_hours(
        struct mliAtmosphere *atmosphere,
        const double increment);
void mliAtmosphere_decrease_hours(
        struct mliAtmosphere *atmosphere,
        const double increment);
void mliAtmosphere_increase_altitude(
        struct mliAtmosphere *atmosphere,
        const double factor);
void mliAtmosphere_decrease_altitude(
        struct mliAtmosphere *atmosphere,
        const double factor);

struct mli_Color mliAtmosphere_query(
        const struct mliAtmosphere *atmosphere,
        const struct mli_Vec orig,
        const struct mli_Vec dir);

struct mli_Color mliAtmosphere_hit_earth_body(
        const struct mliAtmosphere *atmosphere,
        const struct mli_Vec orig,
        const struct mli_Vec dir);

struct mli_Color mliAtmosphere_hit_outer_atmosphere(
        const struct mliAtmosphere *atmosphere,
        const struct mli_Vec orig,
        const struct mli_Vec dir,
        double tmin,
        double tmax);

struct mli_Color mliAtmosphere_compute_depth(
        const struct mliAtmosphere *atmosphere,
        const struct mli_Vec orig,
        const struct mli_Vec dir,
        double tmin,
        double tmax);

#endif
