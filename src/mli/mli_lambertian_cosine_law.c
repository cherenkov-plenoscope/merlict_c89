/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "mli_lambertian_cosine_law.h"
#include <math.h>
#include "../math/math.h"
#include "mliMat.h"
#include "mliHomTra.h"

struct mliVec mli_draw_lambertian_direction_wrt_z(struct mtl_Prng *prng)
{
        double azimuth;
        double sin_theta, cos_theta;
        azimuth = MLI_MATH_2PI * mtl_Prng_uniform(prng);
        sin_theta = mtl_Prng_uniform(prng);
        cos_theta = sqrt(1.0 - sin_theta * sin_theta);
        return mliVec_init(
                sin_theta * cos(azimuth), sin_theta * sin(azimuth), cos_theta);
}

struct mliVec mli_draw_lambertian_direction_wrt_surface_normal(
        struct mtl_Prng *prng,
        const struct mliVec surface_normal)
{
        const struct mliVec z = mliVec_init(0, 0, 1);
        const struct mliVec lambertian_wrt_z =
                mli_draw_lambertian_direction_wrt_z(prng);
        const double rho = mliVec_angle_between(z, surface_normal);
        if (rho > 0.0) {
                const struct mliMat rot = mliMat_init_axis_angle(
                        mliVec_cross(z, surface_normal), -1.0 * rho);
                return mli_transform_orientation(&rot, lambertian_wrt_z);
        } else {
                return lambertian_wrt_z;
        }
}
