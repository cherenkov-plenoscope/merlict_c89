/* Copyright 2019 Sebastian Achim Mueller */
#ifndef MERLICT_MLIRAY_H_
#define MERLICT_MLIRAY_H_

#include <stdio.h>

struct mliRay{
        struct mliVec support;
        struct mliVec direction;
} mliRay;

struct mliRay mliRay_set(
        const struct mliVec support,
        const struct mliVec direction)
{
        struct mliRay ray;
        ray.support = support;
        ray.direction = mliVec_multiply(direction, 1./mliVec_norm(direction));
        return ray;
}

struct mliVec mliRay_at(
        const struct mliRay *ray,
        const double t)
{
        struct mliVec out;
        out.x = ray->support.x + t *ray->direction.x;
        out.y = ray->support.y + t *ray->direction.y;
        out.z = ray->support.z + t *ray->direction.z;
        return out;
}

void mliRay_print(const struct mliRay ray)
{
        printf("[%f, %f, %f] [%f, %f, %f]",
                ray.support.x,
                ray.support.y,
                ray.support.z,
                ray.direction.x,
                ray.direction.y,
                ray.direction.z);
}

#endif
