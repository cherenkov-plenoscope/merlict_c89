/* Copyright 2018-2020 Sebastian Achim Mueller */
/* based on:
 * 2009-2016 Scratchapixel. Distributed under the terms of the
 * GNU General Public License as published by the Free Software Foundation,
 * either version 3 of the License, or (at your option) any later version.
 */

#include "atmosphere.h"
#include <assert.h>
#include <float.h>
#include "../math/math.h"
#include "../homtra/homtra.h"

struct mli_Atmosphere mli_Atmosphere_init(void)
{
        struct mli_Atmosphere atm;
        atm.sunLatitude = 0.0;
        atm.sunHourAngle = 12.0;
        mli_Atmosphere_set_sun_direction(
                &atm, atm.sunLatitude, atm.sunHourAngle);

        atm.sunDistance = 1.5e11;
        atm.sunRadius = 7e8;

        atm.altitude = 2300.0;
        atm.earthRadius = 6360e3;
        atm.atmosphereRadius = atm.earthRadius + 60e3;

        /* The height for the density to drop by 1 over e */
        atm.Height_Rayleigh = 7994.0;
        atm.Height_Mie = 1200.0;

        mli_ColorSpectrum_set_beta_rayleigh(&atm.beta_Rayleigh_spectrum);
        mli_ColorSpectrum_set(&atm.beta_Mie_spectrum, 41e-6);
        mli_ColorSpectrum_set_radiance_of_black_body_W_per_m2_per_sr(
                &atm.sun_spectrum, 5000.0);

        atm.numSamples = 16;
        atm.numSamplesLight = 8;

        atm.power = 3000.0;

        return atm;
}

void mli_ColorSpectrum_set_beta_rayleigh(struct mli_ColorSpectrum *self)
{
        uint64_t i;
        struct mli_ColorSpectrumBinEdges edges =
                mli_ColorSpectrumBinEdges_init();
        const double beta = 1.36e-30;
        for (i = 0; i < MLI_COLORSPECTRUM_SIZE; i++) {
                const double wavelength =
                        0.5 * (edges.values[i] + edges.values[i + 1]);
                self->values[i] = beta / pow(wavelength, 4.0);
        }
}

void mli_Atmosphere_set_sun_direction(
        struct mli_Atmosphere *self,
        const double sunLatitude,
        const double sunHourAngle)
{
        self->sunHourAngle = sunHourAngle;
        self->sunLatitude = sunLatitude;

        {
                const double hours_rad =
                        MLI_MATH_PI +
                        2.0 * MLI_MATH_PI * self->sunHourAngle / 24.0;

                const struct mli_HomTraComp tc_latitude = mli_HomTraComp_set(
                        mli_Vec_init(0.0, 0.0, 0.0),
                        mli_Quaternion_set_tait_bryan(
                                self->sunLatitude, 0.0, 0.0));
                const struct mli_HomTraComp tc_hour = mli_HomTraComp_set(
                        mli_Vec_init(0.0, 0.0, 0.0),
                        mli_Quaternion_set_tait_bryan(0.0, hours_rad, 0.0));
                const struct mli_HomTraComp tc =
                        mli_HomTraComp_sequence(tc_latitude, tc_hour);
                const struct mli_HomTra t = mli_HomTraComp_from_compact(tc);
                const struct mli_Vec zenith = mli_Vec_init(0.0, 0.0, 1.0);
                self->sunDirection = mli_HomTraComp_dir(&t, zenith);
        }
}

struct mli_ColorSpectrum mli_Atmosphere_compute_depth(
        const struct mli_Atmosphere *self,
        const struct mli_Vec orig,
        const struct mli_Vec dir,
        double tmin,
        double tmax)
{
        uint64_t i, j;
        const double segmentLength = (tmax - tmin) / self->numSamples;
        double tCurrent = tmin;
        struct mli_ColorSpectrum sumR = mli_ColorSpectrum_init_zeros();
        struct mli_ColorSpectrum sumM = mli_ColorSpectrum_init_zeros();
        /* mie and rayleigh contribution */

        double opticalDepthR = 0.0;
        double opticalDepthM = 0.0;
        /*
         * mu in the paper which is the cosine of the angle between the sun
         * direction and the ray direction
         */
        const double mu = mli_Vec_dot(dir, self->sunDirection);
        const double phaseR = 3.f / (16.f * MLI_MATH_PI) * (1.0 + mu * mu);
        const double g = 0.76f;
        const double phaseM =
                3.f / (8.f * MLI_MATH_PI) *
                (((1.f - g * g) * (1.f + mu * mu)) /
                 ((2.f + g * g) * pow(1.f + g * g - 2.f * g * mu, 1.5f)));

        for (i = 0; i < self->numSamples; ++i) {
                const struct mli_Vec samplePosition = mli_Vec_add(
                        orig,
                        mli_Vec_multiply(
                                dir, (tCurrent + segmentLength * 0.5f)));
                const double height =
                        (mli_Vec_norm(samplePosition) - self->earthRadius);
                /* compute optical depth for light */
                const double hr =
                        segmentLength * exp(-height / self->Height_Rayleigh);
                const double hm =
                        segmentLength * exp(-height / self->Height_Mie);
                double t0Light = 0;
                double t1Light = 0;
                double segmentLengthLight = 0;
                double tCurrentLight = 0;
                double opticalDepthLightR = 0;
                double opticalDepthLightM = 0;

                opticalDepthR += hr;
                opticalDepthM += hm;
                /* light optical depth */
                mli_Ray_sphere_intersection(
                        samplePosition,
                        self->sunDirection,
                        self->atmosphereRadius,
                        &t0Light,
                        &t1Light);
                segmentLengthLight = t1Light / self->numSamplesLight;

                for (j = 0; j < self->numSamplesLight; ++j) {
                        const struct mli_Vec samplePositionLight = mli_Vec_add(
                                samplePosition,
                                mli_Vec_multiply(
                                        self->sunDirection,
                                        tCurrentLight +
                                                0.5 * segmentLengthLight));
                        const double heightLight =
                                mli_Vec_norm(samplePositionLight) -
                                self->earthRadius;

                        if (heightLight < 0)
                                break;

                        opticalDepthLightR +=
                                segmentLengthLight *
                                exp(-heightLight / self->Height_Rayleigh);
                        opticalDepthLightM +=
                                segmentLengthLight *
                                exp(-heightLight / self->Height_Mie);
                        tCurrentLight += segmentLengthLight;
                }

                if (j == self->numSamplesLight) {
                        const struct mli_ColorSpectrum tau =
                                mli_ColorSpectrum_add(
                                        mli_ColorSpectrum_multiply_scalar(
                                                self->beta_Rayleigh_spectrum,
                                                opticalDepthR +
                                                        opticalDepthLightR),
                                        mli_ColorSpectrum_multiply_scalar(
                                                self->beta_Mie_spectrum,
                                                1.1 * (opticalDepthM +
                                                       opticalDepthLightM)));
                        const struct mli_ColorSpectrum attenuation =
                                mli_ColorSpectrum_exp(tau, -1.0);

                        sumR = mli_ColorSpectrum_add(
                                sumR,
                                mli_ColorSpectrum_multiply_scalar(
                                        attenuation, hr));
                        sumM = mli_ColorSpectrum_add(
                                sumM,
                                mli_ColorSpectrum_multiply_scalar(
                                        attenuation, hm));
                }
                tCurrent += segmentLength;
        }

        return mli_ColorSpectrum_multiply_scalar(
                mli_ColorSpectrum_add(
                        mli_ColorSpectrum_multiply_scalar(
                                mli_ColorSpectrum_multiply(
                                        sumR, self->beta_Rayleigh_spectrum),
                                phaseR),
                        mli_ColorSpectrum_multiply_scalar(
                                mli_ColorSpectrum_multiply(
                                        sumM, self->beta_Mie_spectrum),
                                phaseM)),
                self->power);
}

struct mli_ColorSpectrum mli_Atmosphere_hit_outer_atmosphere(
        const struct mli_Atmosphere *self,
        const struct mli_Vec orig,
        const struct mli_Vec dir,
        double tmin,
        double tmax)
{
        double t_minus = -1.0;
        double t_plus = -1.0;
        int has_intersection = mli_Ray_sphere_intersection(
                orig, dir, self->atmosphereRadius, &t_minus, &t_plus);

        if (!has_intersection || t_plus < 0.0) {
                return mli_ColorSpectrum_init_zeros();
        }
        if (t_minus > tmin && t_minus > 0) {
                tmin = t_minus;
        }
        if (t_plus < tmax) {
                tmax = t_plus;
        }

        return mli_Atmosphere_compute_depth(self, orig, dir, tmin, tmax);
}

struct mli_ColorSpectrum mli_Atmosphere_hit_earth_body(
        const struct mli_Atmosphere *self,
        const struct mli_Vec orig,
        const struct mli_Vec dir)
{
        double t_minus = DBL_MAX;
        double t_plus = DBL_MAX;
        double t_max = DBL_MAX;
        int intersects_earth_body = mli_Ray_sphere_intersection(
                orig, dir, self->earthRadius, &t_minus, &t_plus);

        if (intersects_earth_body && t_minus > 0) {
                t_max = MLI_MATH_MAX2(0.0, t_minus);
        }

        return mli_Atmosphere_hit_outer_atmosphere(self, orig, dir, 0.0, t_max);
}

struct mli_ColorSpectrum mli_Atmosphere_query(
        const struct mli_Atmosphere *self,
        const struct mli_Vec orig,
        const struct mli_Vec dir)
{
        struct mli_Vec orig_up = orig;
        orig_up.z += (self->earthRadius + self->altitude);

        return mli_Atmosphere_hit_earth_body(self, orig_up, dir);
}

void mli_Atmosphere_increase_latitude(
        struct mli_Atmosphere *self,
        const double increment)
{
        if (self->sunLatitude + increment <= 0.5 * MLI_MATH_PI) {
                self->sunLatitude += increment;
                mli_Atmosphere_set_sun_direction(
                        self, self->sunLatitude, self->sunHourAngle);
        }
}

void mli_Atmosphere_decrease_latitude(
        struct mli_Atmosphere *self,
        const double increment)
{
        if (self->sunLatitude - increment >= -0.5 * MLI_MATH_PI) {
                self->sunLatitude -= increment;
                mli_Atmosphere_set_sun_direction(
                        self, self->sunLatitude, self->sunHourAngle);
        }
}

void mli_Atmosphere_increase_hours(
        struct mli_Atmosphere *self,
        const double increment)
{
        if (self->sunHourAngle + increment < 24.0) {
                self->sunHourAngle += increment;
                mli_Atmosphere_set_sun_direction(
                        self, self->sunLatitude, self->sunHourAngle);
        }
}

void mli_Atmosphere_decrease_hours(
        struct mli_Atmosphere *self,
        const double increment)
{
        if (self->sunHourAngle - increment >= 0.0) {
                self->sunHourAngle -= increment;
                mli_Atmosphere_set_sun_direction(
                        self, self->sunLatitude, self->sunHourAngle);
        }
}

void mli_Atmosphere_increase_altitude(
        struct mli_Atmosphere *self,
        const double factor)
{
        assert(factor > 1.0);
        self->altitude *= factor;
}

void mli_Atmosphere_decrease_altitude(
        struct mli_Atmosphere *self,
        const double factor)
{
        assert(factor < 1.0);
        assert(factor > 0.0);
        self->altitude *= factor;
}
