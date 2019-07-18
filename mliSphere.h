/* Copyright 2019 Sebastian Achim Mueller */
#ifndef MERLICT_SPHERE_H_
#define MERLICT_SPHERE_H_

typedef struct {
    double radius;
} mliSphere;

int mliSphere_is_equal(
    const mliSphere a,
    const mliSphere b) {
    return a.radius == b.radius;}

#endif
