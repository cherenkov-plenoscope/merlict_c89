/* Copyright 2018-2020 Sebastian Achim Mueller */
/* based on:
 * 2009-2016 Scratchapixel. Distributed under the terms of the
 * GNU General Public License as published by the Free Software Foundation,
 * either version 3 of the License, or (at your option) any later version.
 */

#include "mliAtmosphere.h"

struct mliAtmosphere mliAtmosphere_init(void) {
        struct mliAtmosphere atm;
        atm.sunLatitude = 0.0;
        atm.sunHourAngle = 12.0;
        mliAtmosphere_set_sun_direction(
                &atm,
                atm.sunLatitude,
                atm.sunHourAngle);

        atm.sunDistance = 1.5e11;
        atm.sunRadius = 7e8;

        atm.altitude = 2300.0;
        atm.earthRadius = 6360e3;
        atm.atmosphereRadius = atm.earthRadius + 60e3;

        atm.Height_Rayleigh = 7994.0;
        atm.Height_Mie = 1200.0;

        atm.beta_Rayleigh = mliVec_set(3.8e-6, 13.5e-6, 33.1e-6);
        atm.beta_Mie = mliVec_multiply(mliVec_set(1.0, 1.0, 1.0), 81e-6);

        atm.numSamples = 16;
        atm.numSamplesLight = 8;

        atm.power = 3000.0;

        return atm;
}

void mliAtmosphere_set_sun_direction(
        struct mliAtmosphere *atmosphere,
        const double sunLatitude,
        const double sunHourAngle)
{
        atmosphere->sunHourAngle = sunHourAngle;
        atmosphere->sunLatitude = sunLatitude;

        {
                const double hours_rad = MLI_PI +
                        2.0 * MLI_PI * atmosphere->sunHourAngle / 24.0;

                const struct mliHomTraComp tc_latitude = mliHomTraComp_set(
                        mliVec_set(0.0, 0.0, 0.0),
                        mliQuaternion_set_tait_bryan(
                                atmosphere->sunLatitude, 0.0, 0.0)
                );
                const struct mliHomTraComp tc_hour = mliHomTraComp_set(
                        mliVec_set(0.0, 0.0, 0.0),
                        mliQuaternion_set_tait_bryan(0.0, hours_rad, 0.0)
                );
                const struct mliHomTraComp tc = mliHomTraComp_sequence(
                        tc_latitude, tc_hour);
                const struct mliHomTra t = mliHomTra_from_compact(tc);
                const struct mliVec zenith = mliVec_set(0.0, 0.0, 1.0);
                atmosphere->sunDirection = mliHomTra_dir(&t, zenith);
        }
}

struct mliColor _mliAtmosphere_compute_depth(
        const struct mliAtmosphere *atmosphere,
        const struct mliVec orig,
        const struct mliVec dir,
        double tmin,
        double tmax)
{
        uint64_t i, j;
        const double segmentLength = (tmax - tmin) / atmosphere->numSamples;
        double tCurrent = tmin;
        struct mliVec sumR = mliVec_set(0.0, 0.0, 0.0);
        struct mliVec sumM = mliVec_set(0.0, 0.0, 0.0);
        /* mie and rayleigh contribution */

        double opticalDepthR = 0.0;
        double opticalDepthM = 0.0;
        /*
         * mu in the paper which is the cosine of the angle between the sun
         * direction and the ray direction
         */
        const double mu = mliVec_dot(dir, atmosphere->sunDirection);
        const double phaseR = 3.f / (16.f * MLI_PI) * (1.0 + mu * mu);
        const double g = 0.76f;
        const double phaseM = 3.f / (8.f * MLI_PI) * (
                ((1.f - g * g) * (1.f + mu * mu)) /
                ((2.f + g * g) * pow(1.f + g * g - 2.f * g * mu, 1.5f))
        );
        struct mliVec col;

        for (i = 0; i < atmosphere->numSamples; ++i) {
                const struct mliVec samplePosition = mliVec_add(
                        orig,
                        mliVec_multiply(dir, (tCurrent + segmentLength * 0.5f))
                );
                const double height = (
                        mliVec_norm(samplePosition) - atmosphere->earthRadius);
                /* compute optical depth for light */
                const double hr = segmentLength * exp(
                        -height / atmosphere->Height_Rayleigh);
                const double hm = segmentLength * exp(
                        -height / atmosphere->Height_Mie);
                double t0Light = 0;
                double t1Light = 0;
                double segmentLengthLight = 0;
                double tCurrentLight = 0;
                double opticalDepthLightR = 0;
                double opticalDepthLightM = 0;

                opticalDepthR += hr;
                opticalDepthM += hm;
                /* light optical depth */
                mliRay_sphere_intersection(
                        samplePosition,
                        atmosphere->sunDirection,
                        atmosphere->atmosphereRadius,
                        &t0Light,
                        &t1Light);
                segmentLengthLight = t1Light / atmosphere->numSamplesLight;

                for (j = 0; j < atmosphere->numSamplesLight; ++j) {
                        const struct mliVec samplePositionLight = mliVec_add(
                                samplePosition,
                                mliVec_multiply(
                                        atmosphere->sunDirection,
                                        (tCurrentLight + segmentLengthLight * 0.5f)
                                )
                        );
                        const double heightLight = mliVec_norm(samplePositionLight)
                                - atmosphere->earthRadius;
                        if (heightLight < 0) break;
                        opticalDepthLightR += segmentLengthLight * exp(
                                -heightLight / atmosphere->Height_Rayleigh);
                        opticalDepthLightM += segmentLengthLight * exp(
                                -heightLight / atmosphere->Height_Mie);
                        tCurrentLight += segmentLengthLight;
                }

                if (j == atmosphere->numSamplesLight) {
                        const struct mliVec tau = mliVec_add(
                                mliVec_multiply(
                                        atmosphere->beta_Rayleigh,
                                        opticalDepthR + opticalDepthLightR
                                ),
                                mliVec_multiply(
                                        atmosphere->beta_Mie,
                                        1.1*(opticalDepthM + opticalDepthLightM)
                                )
                        );
                        const struct mliVec attenuation = mliVec_set(
                                exp(-tau.x),
                                exp(-tau.y),
                                exp(-tau.z)
                        );
                        sumR = mliVec_add(
                                sumR, mliVec_multiply(attenuation, hr));
                        sumM = mliVec_add(
                                sumM, mliVec_multiply(attenuation, hm));
                }
                tCurrent += segmentLength;
        }

        col = mliVec_multiply(
                mliVec_add(
                        mliVec_multiply(
                                mliVec_set(
                                        sumR.x * atmosphere->beta_Rayleigh.x,
                                        sumR.y * atmosphere->beta_Rayleigh.y,
                                        sumR.z * atmosphere->beta_Rayleigh.z
                                ),
                                phaseR),
                        mliVec_multiply(
                                mliVec_set(
                                        sumM.x * atmosphere->beta_Mie.x,
                                        sumM.y * atmosphere->beta_Mie.y,
                                        sumM.z * atmosphere->beta_Mie.z
                                ),
                                phaseM)
                ),
                atmosphere->power
        );

        return mliColor_set(col.x, col.y, col.z);
}

struct mliColor _mliAtmosphere_hit_outer_atmosphere(
        const struct mliAtmosphere *atmosphere,
        const struct mliVec orig,
        const struct mliVec dir,
        double tmin,
        double tmax)
{
        double t_minus = -1.0;
        double t_plus = -1.0;
        int has_intersection = mliRay_sphere_intersection(
                orig,
                dir,
                atmosphere->atmosphereRadius,
                &t_minus,
                &t_plus);

        if (!has_intersection || t_plus < 0.0) {
                return mliColor_set(0.0, 0.0, 0.0);
        }
        if (t_minus > tmin && t_minus > 0) {
                tmin = t_minus;
        }
        if (t_plus < tmax) {
                tmax = t_plus;
        }

        return _mliAtmosphere_compute_depth(
                atmosphere, orig, dir, tmin, tmax);
}

struct mliColor _mliAtmosphere_hit_earth_body(
        const struct mliAtmosphere *atmosphere,
        const struct mliVec orig,
        const struct mliVec dir)
{
        double t_minus = DBL_MAX;
        double t_plus = DBL_MAX;
        double t_max = DBL_MAX;
        int intersects_earth_body = mliRay_sphere_intersection(
                orig,
                dir,
                atmosphere->earthRadius,
                &t_minus,
                &t_plus);

        if (intersects_earth_body && t_minus > 0) {
                t_max = MLI_MAX2(0.0, t_minus);
        }

        return _mliAtmosphere_hit_outer_atmosphere(
                atmosphere, orig, dir, 0.0, t_max);
}

struct mliColor mliAtmosphere_query(
        const struct mliAtmosphere *atmosphere,
        const struct mliVec orig,
        const struct mliVec dir)
{
        struct mliVec orig_up = orig;
        orig_up.z += (atmosphere->earthRadius + atmosphere->altitude);

        return _mliAtmosphere_hit_earth_body(atmosphere, orig_up, dir);
}

void mliAtmosphere_increase_latitude(
        struct mliAtmosphere *atmosphere,
        const double increment)
{
        if (atmosphere->sunLatitude + increment <= 0.5 * MLI_PI) {
                atmosphere->sunLatitude += increment;
                mliAtmosphere_set_sun_direction(
                        atmosphere,
                        atmosphere->sunLatitude,
                        atmosphere->sunHourAngle);
        }
}

void mliAtmosphere_decrease_latitude(
        struct mliAtmosphere *atmosphere,
        const double increment)
{
        if (atmosphere->sunLatitude - increment >= -0.5 * MLI_PI) {
                atmosphere->sunLatitude -= increment;
                mliAtmosphere_set_sun_direction(
                        atmosphere,
                        atmosphere->sunLatitude,
                        atmosphere->sunHourAngle);
        }
}

void mliAtmosphere_increase_hours(
        struct mliAtmosphere *atmosphere,
        const double increment)
{
        if (atmosphere->sunHourAngle + increment < 24.0) {
                atmosphere->sunHourAngle += increment;
                mliAtmosphere_set_sun_direction(
                        atmosphere,
                        atmosphere->sunLatitude,
                        atmosphere->sunHourAngle);
        }
}

void mliAtmosphere_decrease_hours(
        struct mliAtmosphere *atmosphere,
        const double increment)
{
        if (atmosphere->sunHourAngle - increment >= 0.0) {
                atmosphere->sunHourAngle -= increment;
                mliAtmosphere_set_sun_direction(
                        atmosphere,
                        atmosphere->sunLatitude,
                        atmosphere->sunHourAngle);
        }
}
