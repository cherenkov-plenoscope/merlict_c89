/* Copyright 2019 Sebastian Achim Mueller */
#ifndef MERLICT_MLIRAY_H_
#define MERLICT_MLIRAY_H_

typedef struct {
    mliVec support;
    mliVec direction;
} mliRay;

mliRay mliRay_set(
    const mliVec support,
    const mliVec direction) {
    mliRay ray;
    ray.support = support;
    ray.direction = mliVec_multiply(
        &direction, 1./mliVec_norm(&direction));
    return ray;}

mliVec mliRay_at(
    const mliRay *ray,
    const float t) {
    mliVec out;
    out.x = ray->support.x + t *ray->direction.x;
    out.y = ray->support.y + t *ray->direction.y;
    out.z = ray->support.z + t *ray->direction.z;
    return out;}

#endif
