/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MERLICT_C89_MLIATMOSPHERE_H_
#define MERLICT_C89_MLIATMOSPHERE_H_

struct mliAtmosphere {
        double sunLatitude;
        double sunHourAngle;

        struct mliVec sunDirection;
        double sunDistance;
        double sunRadius;

        double earthRadius;
        double atmosphereRadius;

        double Height_Rayleigh;
        double Height_Mie;

        struct mliVec beta_Rayleigh;
        struct mliVec beta_Mie;

        uint64_t numSamples;
        uint64_t numSamplesLight;

        double power;
        double altitude;
};

struct mliAtmosphere mliAtmosphere_init(void);

void mliAtmosphere_set_sun_direction(struct mliAtmosphere *atmosphere);

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


struct mliColor mliAtmosphere_computeIncidentLight(
        const struct mliAtmosphere *atmosphere,
        const struct mliVec orig,
        const struct mliVec dir,
        double tmin,
        double tmax);

struct mliColor mliAtmosphere_query(
        const struct mliAtmosphere *atmosphere,
        const struct mliVec orig,
        const struct mliVec dir);

#endif
