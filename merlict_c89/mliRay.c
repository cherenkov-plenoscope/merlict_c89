/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "mliRay.h"

struct mliRay mliRay_set(
        const struct mliVec support,
        const struct mliVec direction)
{
        struct mliRay ray;
        ray.support = support;
        ray.direction = mliVec_multiply(direction, 1. / mliVec_norm(direction));
        return ray;
}

struct mliVec mliRay_at(const struct mliRay *ray, const double t)
{
        struct mliVec out;
        out.x = ray->support.x + t * ray->direction.x;
        out.y = ray->support.y + t * ray->direction.y;
        out.z = ray->support.z + t * ray->direction.z;
        return out;
}
