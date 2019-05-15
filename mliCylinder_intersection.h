/* Copyright 2019 Sebastian Achim Mueller */
#ifndef MERLICT_MLICYLINDER_INTERSECTION_H_
#define MERLICT_MLICYLINDER_INTERSECTION_H_

#include <math.h>
#include "mliMath.h"
#include "mliIntersection.h"
#include "mliQuadraticEquation.h"

int mli_cylinder_equation(
    const double radius,
    const mliRay ray,
    double *plus_solution,
    double *minus_solution) {
    /*
    Cylinder is centered on the z-axis and is not limited in z.

    ray := s+l*d, s = (sx, sy, sz)^T, d = (dx, dy, dz)^T

    r = sqrt(x^2 + y^2)                                              (1)

    r = sqrt( (sx+l*dy)^2 + (sy+l*dy)^2 )                            (2)

    r = sqrt( l^2*(dx^2 + dy^2) + l*2*(sx*dx + sy*dy) +sx^2 +sy^2)   (3)

    0 = l^2 +
         l*2*(sxdx + sydy)/(dx^2 + dy^2) +
         (sx^2 +sy^2 -r^2)/(dx^2 + dy^2)
    */
    const double dx2 = ray.direction.x*ray.direction.x;
    const double dy2 = ray.direction.y*ray.direction.y;

    const double sxdx = ray.support.x*ray.direction.x;
    const double sydy = ray.support.y*ray.direction.y;

    const double sx2 = ray.support.x*ray.support.x;
    const double sy2 = ray.support.y*ray.support.y;

    const double r2 = radius*radius;
    const double dnom = dx2 + dy2;

    const double p = 2.*(sxdx + sydy) / dnom;
    const double q = (sx2 + sy2 -r2) / dnom;

    return mli_quadratiq_equation(p, q, plus_solution, minus_solution);
}

#endif
