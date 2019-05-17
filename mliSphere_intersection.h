/* Copyright 2019 Sebastian Achim Mueller */
#ifndef MERLICT_MLISPHERE_INTERSECTIOM_H_
#define MERLICT_MLISPHERE_INTERSECTIOM_H_

#include <math.h>
#include "mliMath.h"
#include "mliOBB.h"
#include "mliVec.h"
#include "mliRay.h"
#include "mliHomTra.h"
#include "mliIntersection.h"
#include "mliQuadraticEquation.h"
#include "mli_from_outside_to_inside.h"

int mli_sphere_intersection_equation(
    const double radius,
    const mliRay ray,
    double *plus_solution,
    double *minus_solution) {
    /*
     *  r = sqrt( x^2 + y^2 + z^2 )
     *  g:  b + v*d
     *  put g into sphere eq
     *  r = sqrt((bx+v*dx)^2 + (by+v*dy)^2 + (bz+v*dz)^2 )
     *  r = sqrt( v^2 (dd) + v 2(bd) + (bb) )
     *  r^2 = v^2 (dd) + v 2(bd) + (bb)
     *  0 = v^2 + v 2(bd/dd) + (bb/dd -r^2)
     *  solve quadrativ eqaution in v
     */
    const double sup_times_dir = mliVec_dot(ray.support, ray.direction);
    const double dir_times_dir = mliVec_dot(ray.direction, ray.direction);
    const double sup_times_sup = mliVec_dot(ray.support, ray.support);
    const double radius_square = radius*radius;
    return mli_quadratiq_equation(
        2*(sup_times_dir/dir_times_dir),
        sup_times_sup/dir_times_dir - radius_square,
        plus_solution,
        minus_solution);}

void mliSphere_set_intersection(
    const mliHomTra *local2root,
    const mliRay *ray_local,
    const double ray_solution,
    mliIntersection *intersection) {
    mliVec position_local = mliRay_at(ray_local, ray_solution);
    mliVec normal_local = mliVec_normalized(position_local);
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

int mliSphere_intersection(
    const double radius,
    const mliHomTraComp local2root_comp,
    const mliRay ray,
    mliIntersection *intersection) {
    mliHomTra local2root = mliHomTra_from_compact(local2root_comp);
    mliRay ray_local = mliHomTra_ray_inverse(&local2root, ray);
    double plus_solution, minus_solution;
    if (mli_sphere_intersection_equation(
        radius,
        ray_local,
        &plus_solution,
        &minus_solution)
    ) {
        int m_gt_e = minus_solution > MLI_EPSILON;
        int p_gt_e = plus_solution > MLI_EPSILON;
        int fm_le_e = fabs(minus_solution) <= MLI_EPSILON;
        int fp_le_e = fabs(plus_solution) <= MLI_EPSILON;

        if (m_gt_e && p_gt_e && !fm_le_e && !fp_le_e) {
            mliSphere_set_intersection(
                &local2root,
                &ray_local,
                minus_solution,
                intersection);
            return 1;
        }

        if (!m_gt_e && p_gt_e && !fm_le_e && !fp_le_e) {
            mliSphere_set_intersection(
                &local2root,
                &ray_local,
                plus_solution,
                intersection);
            return 1;
        }

        if (!m_gt_e && p_gt_e && fm_le_e && !fp_le_e) {
            mliSphere_set_intersection(
                &local2root,
                &ray_local,
                minus_solution,
                intersection);
            return 1;
        }
    }
    return 0;
}

#endif
