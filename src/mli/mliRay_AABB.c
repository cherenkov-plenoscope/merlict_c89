/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "mliRay_AABB.h"
#include "../math/math.h"

void mliRay_aabb_intersections(
        const struct mliRay ray,
        const struct mliAABB aabb,
        double *t_near,
        double *t_far)
{
        struct mli_Vec frac;
        struct mli_Vec lower, upper;
        struct mli_Vec t1, t2;

        frac.x = 1. / ray.direction.x;
        frac.y = 1. / ray.direction.y;
        frac.z = 1. / ray.direction.z;

        lower.x = (aabb.lower.x - ray.support.x) * frac.x;
        lower.y = (aabb.lower.y - ray.support.y) * frac.y;
        lower.z = (aabb.lower.z - ray.support.z) * frac.z;

        upper.x = (aabb.upper.x - ray.support.x) * frac.x;
        upper.y = (aabb.upper.y - ray.support.y) * frac.y;
        upper.z = (aabb.upper.z - ray.support.z) * frac.z;

        t1.x = MLI_MATH_MIN2(lower.x, upper.x);
        t1.y = MLI_MATH_MIN2(lower.y, upper.y);
        t1.z = MLI_MATH_MIN2(lower.z, upper.z);

        t2.x = MLI_MATH_MAX2(lower.x, upper.x);
        t2.y = MLI_MATH_MAX2(lower.y, upper.y);
        t2.z = MLI_MATH_MAX2(lower.z, upper.z);

        (*t_near) = MLI_MATH_MAX3(t1.x, t1.y, t1.z);
        (*t_far) = MLI_MATH_MIN3(t2.x, t2.y, t2.z);
}

int mliRay_aabb_intersections_is_valid_given_near_and_far(
        const double t_near,
        const double t_far)
{
        if (t_far < 0) {
                return 0;
        }
        if (t_near > t_far) {
                return 0;
        }
        return 1;
}

int mliRay_has_overlap_aabb(const struct mliRay ray, const struct mliAABB aabb)
{
        double t_near, t_far;
        mliRay_aabb_intersections(ray, aabb, &t_near, &t_far);
        return mliRay_aabb_intersections_is_valid_given_near_and_far(
                t_near, t_far);
}
