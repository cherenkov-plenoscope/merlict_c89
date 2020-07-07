/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "mliRay_OBB.h"

int mliRay_has_overlap_obb(
        const struct mliRay ray,
        const struct mliOBB obb,
        double *ray_parameter)
{
        const double frac_x = 1. / ray.direction.x;
        const double frac_y = 1. / ray.direction.y;
        const double frac_z = 1. / ray.direction.z;

        const double t1 = (obb.lower.x - ray.support.x) * frac_x;
        const double t2 = (obb.upper.x - ray.support.x) * frac_x;
        const double t3 = (obb.lower.y - ray.support.y) * frac_y;
        const double t4 = (obb.upper.y - ray.support.y) * frac_y;
        const double t5 = (obb.lower.z - ray.support.z) * frac_z;
        const double t6 = (obb.upper.z - ray.support.z) * frac_z;

        const double tmin = MLI_MAX2(
                MLI_MAX2(MLI_MIN2(t1, t2), MLI_MIN2(t3, t4)), MLI_MIN2(t5, t6));
        const double tmax = MLI_MIN2(
                MLI_MIN2(MLI_MAX2(t1, t2), MLI_MAX2(t3, t4)), MLI_MAX2(t5, t6));
        if (tmax < 0) {
                (*ray_parameter) = tmax;
                return 0;
        }

        /* if tmin > tmax, ray doesn't intersect AABB */
        if (tmin > tmax) {
                (*ray_parameter) = tmax;
                return 0;
        }

        (*ray_parameter) = tmin;
        return 1;
}
