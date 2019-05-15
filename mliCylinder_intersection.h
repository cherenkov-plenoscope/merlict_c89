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

mliVec mliCylinder_surface_normal(const double x, const double y) {
    mliVec surface_normal = mliVec_set(x, y, 0.);
    return mliVec_normalized(surface_normal);}

void mliCylinder_set_intersection(
    const mliHomTra *local2root,
    const mliRay *ray_local,
    const double ray_solution,
    mliIntersection *intersection) {
    mliVec position_local = mliRay_at(ray_local, ray_solution);
    mliVec normal_local = mliCylinder_surface_normal(
        position_local.x,
        position_local.y);
    intersection->position = mliHomTra_pos(
        local2root,
        position_local);
    intersection->surface_normal = mliHomTra_dir(
        local2root,
        normal_local);
    intersection->distance_of_ray = ray_solution;
    intersection->from_outside_to_inside = mli_ray_runs_from_outside_to_inside(
        ray_local->direction,
        normal_local);}

int mliCylinder_intersection(
    const mliCylinder cylinder,
    const mliHomTraComp local2root_comp,
    const mliRay ray,
    mliIntersection *intersection) {
    mliHomTra local2root = mliHomTra_from_compact(local2root_comp);
    mliRay ray_local = mliHomTra_ray_inverse(&local2root, ray);
    double plus_solution, minus_solution;
    if (mli_cylinder_equation(
        cylinder.radius,
        ray_local,
        &plus_solution,
        &minus_solution)
    ) {
        int m_gt_e = minus_solution > MLI_EPSILON;
        int p_gt_e = plus_solution > MLI_EPSILON;
        int fm_le_e = fabs(minus_solution) <= MLI_EPSILON;
        int fp_le_e = fabs(plus_solution) <= MLI_EPSILON;

        if (m_gt_e && p_gt_e && !fm_le_e && !fp_le_e) {
            /*
                         Z      ray
                         ^      /
                     |   |   | /
                     |   |   |/
                     |       O p
                     |      /|
                     |     / |
                     |    /  |
                     |   /   |
                     |  /    |
                     | /     |
                     |/      |
                     O m     |
                    /|       |
                   / |       |
                  X  |       |
               support
            */
            mliCylinder_set_intersection(
                &local2root,
                &ray_local,
                minus_solution,
                intersection);
            return 1;
        }

        if (!m_gt_e && p_gt_e && !fm_le_e && !fp_le_e) {
            /*
                         Z      ray
                         ^      /
                     |   |   | /
                     |   |   |/
                     |       O p
                     |      /|
                     |     / |
                     |    /  |
                     |   X support
                     |  /    |
                     | /     |
                     |/      |
                     O m     |
                    /|       |
                   / |       |
                  /  |       |

            */
            mliCylinder_set_intersection(
                &local2root,
                &ray_local,
                plus_solution,
                intersection);
            return 1;
        }

        if (!m_gt_e && p_gt_e && fm_le_e && !fp_le_e) {
            /*
                         Z      ray
                         ^      /
                     |   |   | /
                     |   |   |/
                     |       O p
                     |      /|
                     |     / |
                     |    /  |
                     |   /   |
                     |  /    |
                     | /     |
                     |/      |
                     X support = m
                    /|       |
                   / |       |
                  /  |       |

            */
            mliCylinder_set_intersection(
                &local2root,
                &ray_local,
                minus_solution,
                intersection);
            return 1;
        }
        /*
                     Z      ray
                     ^      /
                 |   |   | /
                 |   |   |/
                 |       X support = p
                 |      /|
                 |     / |
                 |    /  |
                 |   /   |
                 |  /    |
                 | /     |
                 |/      |
                 O m     |
                /|       |
               / |       |
              /  |       |
            NO causal intersection
        */
        /*
                     Z      ray
                     ^      /
                 |   |   | X support
                 |   |   |/
                 |       O p
                 |      /|
                 |     / |
                 |    /  |
                 |   /   |
                 |  /    |
                 | /     |
                 |/      |
                 O m     |
                /|       |
               / |       |
              /  |       |
            NO causal intersection
        */
    }
    return 0;
}

#endif
