/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "mli_lambertian_cosine_law.h"

struct mliVec mli_draw_lambertian_direction_wrt_z(struct mliMT19937 *prng)
{
        double azimuth;
        double sin_theta, cos_theta;
        azimuth = MLI_2PI * mli_random_uniform(prng);
        sin_theta = mli_random_uniform(prng);
        cos_theta = sqrt(1.0 - sin_theta * sin_theta);
        return mliVec_set(
                sin_theta * cos(azimuth), sin_theta * sin(azimuth), cos_theta);
}

struct mliVec mli_draw_lambertian_direction_wrt_surface_normal(
        struct mliMT19937 *prng,
        const struct mliVec surface_normal)
{
        const struct mliVec z = mliVec_set(0, 0, 1);
        const struct mliVec lambertian_wrt_z =
                mli_draw_lambertian_direction_wrt_z(prng);
        const double rho = mliVec_angle_between(z, surface_normal);
        if (rho > 0.0) {
                const struct mliRotMat rot = mliRotMat_init_axis(
                        mliVec_cross(z, surface_normal), -1.0 * rho);
                return mli_transform_orientation(&rot, lambertian_wrt_z);
        } else {
                return lambertian_wrt_z;
        }
}
