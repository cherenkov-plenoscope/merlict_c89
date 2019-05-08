/* Copyright 2019 Sebastian Achim Mueller */
#ifndef MERLICT_MLISPHERE_INTERSECTIOM_H_
#define MERLICT_MLISPHERE_INTERSECTIOM_H_

#include <math.h>
#include "mliOBB.h"
#include "mliVec.h"
#include "mliRay.h"
#include "mliHomTra.h"
#include "mliIntersection.h"
#include "mliQuadraticEquation.h"

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

int mliSphere_facing_sphere_from_outside_given_p_m(
    const double v_Plus,
    const double v_Minus) {
    return v_Plus >= 0. && v_Minus >= 0.;}

int mliSphere_facing_away_from_outside_given_p_m(
    const double v_Plus,
    const double v_Minus) {
    return v_Plus < 0. && v_Minus < 0.;}

void mliSphere_set_intersection(
    mliIntersection *intersection,
    mliRay ray,
    double ray_parameter) {
    /* transformation */
    intersection->position = mliRay_at(&ray, ray_parameter);
    intersection->surface_normal = intersection->position;
}

int mliSphere_intersection(
    const float radius,
    const mliHomTraComp trafo,
    const mliRay ray,
    mliIntersection *intersection) {
    /*    mliRay local_ray;
    local_ray.support = mliHomTra_transform_position(trafo, )*/

    double plus_solution, minus_solution;
    if (mli_sphere_intersection_equation(
        radius,
        ray,
        &plus_solution,
        &minus_solution)
    ) {
        if (mliSphere_facing_sphere_from_outside_given_p_m(
            plus_solution,
            minus_solution)
        ) {
            mliSphere_set_intersection(intersection, ray, minus_solution);
        } else if (mliSphere_facing_away_from_outside_given_p_m(
            plus_solution,
            minus_solution)
        ) {
            mliSphere_set_intersection(intersection, ray, plus_solution);
        }
        return 1;
    }
    return 0;
}

#endif
