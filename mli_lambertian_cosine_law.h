/* Copyright 2019 Sebastian Achim Mueller */
#ifndef MERLICT_MLI_LAMBERTIAN_COSINE_LAW_H_
#define MERLICT_MLI_LAMBERTIAN_COSINE_LAW_H_

#include <math.h>
#include <stdint.h>
#include "mliVec.h"

mliVec mli_draw_lambertian_direction_wrt_z(mliMT19937 *prng) {
    double azimuth;
    double sin_theta, cos_theta;
    azimuth = MLI_2PI*mliMT19937_uniform(prng);
    sin_theta = mliMT19937_uniform(prng);
    cos_theta = sqrt(1.0 - sin_theta*sin_theta);
    return mliVec_set(
        sin_theta*cos(azimuth),
        sin_theta*sin(azimuth),
        cos_theta);}

mliVec mli_draw_lambertian_direction_wrt_surface_normal(
    mliMT19937 *prng,
    const mliVec surface_normal) {
    const mliVec z = mliVec_set(0, 0, 1);
    const mliVec lambertian_wrt_z = mli_draw_lambertian_direction_wrt_z(prng);
    const double rho = mliVec_angle_between(z, surface_normal);
    if (rho > 0.0) {
        const mliRotMat rot = mliRotMat_init_axis(
            mliVec_cross(z, surface_normal),
            -1.0*rho);
        return mli_transform_orientation(&rot, lambertian_wrt_z);
    } else {
        return lambertian_wrt_z;}}

#endif