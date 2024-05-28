/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "mliRay_AABB.h"
#include "mli_math.h"

#define MIN2 MLI_MIN2
#define MAX2 MLI_MAX2

int mliRay_has_overlap_aabb(
        const struct mliRay ray,
        const struct mliAABB aabb,
        double *ray_parameter)
{
        const double frac_x = 1. / ray.direction.x;
        const double frac_y = 1. / ray.direction.y;
        const double frac_z = 1. / ray.direction.z;

        const double xl = (aabb.lower.x - ray.support.x) * frac_x;
        const double xu = (aabb.upper.x - ray.support.x) * frac_x;
        const double yl = (aabb.lower.y - ray.support.y) * frac_y;
        const double yu = (aabb.upper.y - ray.support.y) * frac_y;
        const double zl = (aabb.lower.z - ray.support.z) * frac_z;
        const double zu = (aabb.upper.z - ray.support.z) * frac_z;

        const double tmin = MLI_MAX3(MIN2(xl, xu), MIN2(yl, yu), MIN2(zl, zu));
        const double tmax = MLI_MIN3(MAX2(xl, xu), MAX2(yl, yu), MAX2(zl, zu));

        /*  if tmax < 0, ray (line) is intersecting AABB
         *  but the whole AABB is behind us
         */
        if (tmax < 0) {
                (*ray_parameter) = tmax;
                return 0;
        }

        /* if tmin > tmax, ray doesn't intersect AABB */
        if (tmin > tmax) {
                (*ray_parameter) = tmax;
                return 1;
        }

        (*ray_parameter) = tmin;
        return 1;
}
