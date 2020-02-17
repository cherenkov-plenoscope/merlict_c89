/* Copyright 2019 Sebastian Achim Mueller */
#ifndef MERLICT_MLICYLINDER_INTERSECTION_H_
#define MERLICT_MLICYLINDER_INTERSECTION_H_

#include <math.h>
#include "mli_math.h"
#include "mliIntersection.h"
#include "mliQuadraticEquation.h"

int mli_cylinder_equation(
    const double radius,
    const struct mliRay ray,
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

struct mliVec mliCylinder_surface_normal(const double x, const double y) {
    struct mliVec surface_normal = mliVec_set(x, y, 0.);
    return mliVec_normalized(surface_normal);}

void mliCylinder_set_intersection(
    const mliHomTra *local2root,
    const struct mliRay *ray_local,
    const double ray_solution,
    mliIntersection *intersection) {
    struct mliVec position_local = mliRay_at(ray_local, ray_solution);
    struct mliVec normal_local = mliCylinder_surface_normal(
        position_local.x,
        position_local.y);
    intersection->position = mliHomTra_pos(
        local2root,
        position_local);
    intersection->surface_normal = mliHomTra_dir(
        local2root,
        normal_local);
    intersection->distance_of_ray = ray_solution;
    intersection->from_outside_to_inside =
        mli_ray_runs_from_outside_to_inside(
            ray_local->direction,
            normal_local);
}

int mliCylinder_intersection(
    const mliCylinder cylinder,
    const mliHomTraComp local2root_comp,
    const struct mliRay ray,
    mliIntersection *intersection) {
    /*                                                                        */
    /* m > epsilon                                                            */
    /* p > epsilon                  (always true for causal intersections)    */
    /* fabs(m) <= epsilon                                                     */
    /* fabs(p) <= epsilon           (always false for causal intersections)   */
    /* fabs(m.z) <= length/2                                                  */
    /* fabs(p.z) <= length/2                                                  */
    /*                                                                        */
    /*  bit-mask = [
            m > epsilon,
            fabs(m) <= epsilon,
            fabs(m.z) <= length/2,
            fabs(p.z) <= length/2
        ]
    */

    /* support infront                                                        */
    /*                                                             /          */
    /*                                                            /           */
    /*                                                           /            */
    /*                                                          O p           */
    /*                                                         /              */
    /*                                       /                /               */
    /*                                      /                /                */
    /*      |       |             |       |/            |   /   |             */
    /*      |       |             |       O p           |  /    |             */
    /*      |       |             |      /|             | /     |             */
    /*      |       |  /          |     / |             |/      |             */
    /*      |       | /           |    /  |             O m     |             */
    /*      |       |/            |   /   |            /|       |             */
    /*      |       O p           |  /    |         s X |       |             */
    /*      |      /|             | /     |          /  |       |             */
    /*      |     / |             |/      |             |       |             */
    /*      |    /  |             O m     |             |       |             */
    /*      |   /   |            /|       |             |       |             */
    /*         /              s X                                             */
    /*        /                /                                              */
    /*       /                                                                */
    /*      O m                                                               */
    /*     /                                                                  */
    /*    X s                                                                 */
    /*   /                                                                    */
    /*      [1,0,0,1]              [1,0,1,1]             [1,0,1,0]            */
    /*      9                      13                    5                    */


    /* support equals minus-intersection                                      */
    /*                                                             /          */
    /*                                                            /           */
    /*                                                           /            */
    /*                                                          O p           */
    /*                                                         /              */
    /*                                       /                /               */
    /*                                      /                /                */
    /*      |       |             |       |/            |   /   |             */
    /*      |       |             |       O p           |  /    |             */
    /*      |       |             |      /|             | /     |             */
    /*      |       |  /          |     / |             |/      |             */
    /*      |       | /           |    /  |             X m=s   |             */
    /*      |       |/            |   /   |            /|       |             */
    /*      |       O p           |  /    |           / |       |             */
    /*      |      /|             | /     |          /  |       |             */
    /*      |     / |             |/      |             |       |             */
    /*      |    /  |             X m=s   |             |       |             */
    /*      |   /   |            /|       |             |       |             */
    /*         /                /                                             */
    /*        /                /                                              */
    /*       /                                                                */
    /*      X m=s                                                             */
    /*     /                                                                  */
    /*    /                                                                   */
    /*   /                                                                    */
    /*                                                                        */
    /*      [0,1,0,1]              [0,1,1,1]            [0,1,1,0]             */
    /*      10                     14                   6                     */

    /* support inside                                                         */
    /*                                                             /          */
    /*                                                            /           */
    /*                                                           /            */
    /*                                                          O p           */
    /*                                                         /              */
    /*                                       /                /               */
    /*                                      /                /                */
    /*      |       |             |       |/            |   X s |             */
    /*      |       |             |       O p           |  /    |             */
    /*      |       |             |      /|             | /     |             */
    /*      |       |  /          |     / |             |/      |             */
    /*      |       | /           |    /  |             O m     |             */
    /*      |       |/            |   X s |            /|       |             */
    /*      |       O p           |  /    |           / |       |             */
    /*      |      /|             | /     |          /  |       |             */
    /*      |     / |             |/      |             |       |             */
    /*      |    /  |             O m     |             |       |             */
    /*      |   X s |            /|       |             |       |             */
    /*         /                /                                             */
    /*        /                /                                              */
    /*       /                                                                */
    /*      O m                                                               */
    /*     /                                                                  */
    /*    /                                                                   */
    /*   /                                                                    */
    /*                                                                        */
    /*      [0,0,0,1]              [0,0,1,1]                                  */
    /*      8                      12                                         */

    /* support equals plus-intersection                                       */
    /*                                                             /          */
    /*                                                            /           */
    /*                                                           /            */
    /*                                                          X p=s         */
    /*                                                         /              */
    /*                                       /                /               */
    /*                                      /                /                */
    /*      |       |             |       |/            |   /   |             */
    /*      |       |             |       X p=s         |  /    |             */
    /*      |       |             |      /|             | /     |             */
    /*      |       |  /          |     / |             |/      |             */
    /*      |       | /           |    /  |             O m     |             */
    /*      |       |/            |   /   |            /|       |             */
    /*      |       X p=s         |  /    |           / |       |             */
    /*      |      /|             | /     |          /  |       |             */
    /*      |     / |             |/      |             |       |             */
    /*      |    /  |             O m     |             |       |             */
    /*      |   /   |            /|       |             |       |             */
    /*         /                /                                             */
    /*        /                /                                              */
    /*       /                                                                */
    /*      O m                                                               */
    /*     /                                                                  */
    /*    /                                                                   */
    /*   /                                                                    */
    /*                                                                        */

    /* support behind                                                         */
    /*                                                             /          */
    /*                                                            X s         */
    /*                                                           /            */
    /*                                                          O p           */
    /*                                                         /              */
    /*                                       /                /               */
    /*                                      X s              /                */
    /*      |       |             |       |/            |   /   |             */
    /*      |       |             |       O p           |  /    |             */
    /*      |       |             |      /|             | /     |             */
    /*      |       |  /          |     / |             |/      |             */
    /*      |       | X s         |    /  |             O m     |             */
    /*      |       |/            |   /   |            /|       |             */
    /*      |       O p           |  /    |           / |       |             */
    /*      |      /|             | /     |          /  |       |             */
    /*      |     / |             |/      |             |       |             */
    /*      |    /  |             O m     |             |       |             */
    /*      |   /   |            /|       |             |       |             */
    /*         /                /                                             */
    /*        /                /                                              */
    /*       /                                                                */
    /*      O m                                                               */
    /*     /                                                                  */
    /*    /                                                                   */
    /*   /                                                                    */
    /*                                                                        */

    double plus_solution, minus_solution;
    mliHomTra local2root = mliHomTra_from_compact(local2root_comp);
    struct mliRay ray_local = mliHomTra_ray_inverse(&local2root, ray);
    if (mli_cylinder_equation(
        cylinder.radius,
        ray_local,
        &plus_solution,
        &minus_solution)
    ) {
        if (plus_solution < MLI_EPSILON) {
            return 0;
        } else {
            const int m_gt_e = minus_solution > MLI_EPSILON;
            const int fm_le_e = fabs(minus_solution) <= MLI_EPSILON;
            const int fmz_le_hl =
                fabs(mliRay_at(&ray_local, minus_solution).z) <=
                cylinder.length*.5;
            const int fpz_le_hl =
                fabs(mliRay_at(&ray_local, plus_solution).z) <=
                cylinder.length*.5;
            int scenario = m_gt_e*1 + fm_le_e*2 + fmz_le_hl*4 + fpz_le_hl*8;
            double causal_solution;
            int causal = 1;
            switch (scenario) {
                case 9:
                    causal_solution = plus_solution;
                    break;
                case 13:
                    causal_solution = minus_solution;
                    break;
                case 5:
                    causal_solution = minus_solution;
                    break;

                case 10:
                    causal_solution = plus_solution;
                    break;
                case 14:
                    causal_solution = plus_solution;
                    break;
                case 6:
                    causal_solution = minus_solution;
                    break;

                case 8:
                    causal_solution = plus_solution;
                    break;
                case 12:
                    causal_solution = plus_solution;
                    break;

                default:
                    causal = 0;
                    break;
            }

            if (causal) {
                mliCylinder_set_intersection(
                    &local2root,
                    &ray_local,
                    causal_solution,
                    intersection);
                return 1;
            }
        }
    }
    return 0;
}

#endif
