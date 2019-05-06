/* Copyright 2019 Sebastian Achim Mueller */
#ifndef MERLICT_MLISPHERICALCAP_H_
#define MERLICT_MLISPHERICALCAP_H_

#include <math.h>
#include "mliVec.h"
#include "mliRay.h"
#include "mliQuadraticEquation.h"
#include "mliHexagonalPrismZ.h"
#include "mliOuterPrismBound.h"
#include "mliIntersection.h"

int mli_spherical_cap_equation(
    const mliRay *ray,
    const double radius,
    double *plus_solution,
    double *minus_solution) {
    /*
        sphere intersection with ray
        z(x,y) = R - sqrt(R^2 -x^2 -y^2) |- R, *(-1)
        R - z  = sqrt(R^2 -x^2 -y^2)     |pow2
        (R-z)^2= R^2 - x^2 - y^2         |+x^2 +y^2
        R^2 -2Rz +z^2 +y^2 +x^2 = R^2    |-R^2
        x^2 + y^2 + z^2 -2Rz = 0         | x=sx+v*dx, y=sy+v*dy, z=sz+v*dz
        v^2*(d*d) + v*2(s*d -R*dz) +s*s -2*R*sz = 0
        v^2 + v 2(s*d - R*dz)/(d*d) + (s*s - 2*R*sz)/(d*d) = 0
    */
    const mliVec sup = ray->support;
    const mliVec dir = ray->direction;
    const double dir_times_dir = mliVec_dot(dir, dir);
    const double sup_times_sup = mliVec_dot(sup, sup);
    const double sup_times_dir = mliVec_dot(sup, dir);
    const double R_times_dir_z = radius*dir.z;
    const double R_times_sup_z = radius*sup.z;
    const double P = 2. * (sup_times_dir - R_times_dir_z) / dir_times_dir;
    const double Q = (sup_times_sup - 2.*R_times_sup_z) / dir_times_dir;
    return mli_quadratiq_equation(P, Q, plus_solution, minus_solution);}

mliVec mli_spherical_cap_surface_normal(
    const mliVec intersection_point,
    const double radius) {
    mliVec radial = mliVec_set(0., 0., radius);
    mliVec normal = mliVec_substract(radial, intersection_point);
    normal = mliVec_multiply(normal, 1./mliVec_norm(normal));
    return normal;}

int mli_ray_runs_from_outside_to_inside(
    const mliVec ray_direction_local,
    const mliVec surface_normal_local) {
    const double proj = mliVec_dot(surface_normal_local, ray_direction_local);
    if (proj < 0.)
        return 1;
    else
        return 0;}

int mli_spherical_cap_hex_bound_intersection(
    const mliRay *ray_local,
    const double curvature_radius,
    const double inner_hex_radius,
    mliIntersection *intersection) {
    double plus_solution, minus_solution;
    if (
        mli_spherical_cap_equation(
            ray_local,
            curvature_radius,
            &plus_solution,
            &minus_solution)
    ) {
        double causal_solution;
        mliBoundSurfaceChecklist cl;
        cl.plus_solution = plus_solution;
        cl.minus_solution = minus_solution;
        cl.plus_intersec = mliRay_at(ray_local, cl.plus_solution);
        cl.minus_intersec = mliRay_at(ray_local, cl.minus_solution);
        cl.plus_is_inside = mli_inside_hexagonal_prism_z(
            cl.plus_intersec, inner_hex_radius);
        cl.minus_is_inside = mli_inside_hexagonal_prism_z(
            cl.minus_intersec, inner_hex_radius);

        if (mli_outer_bound_surface_causal_intersection(
            cl,
            &causal_solution)
        ) {
            intersection->distance_of_ray = causal_solution;
            intersection->position = mliRay_at(ray_local, causal_solution);
            intersection->surface_normal_local =
                mli_spherical_cap_surface_normal(
                    intersection->position,
                    curvature_radius);
            intersection->from_outside_to_inside =
                mli_ray_runs_from_outside_to_inside(
                    ray_local->direction,
                    intersection->surface_normal_local);

            return 1;
        }
    }
    return 0;
}

typedef struct {
    float curvature_radius;
    float inner_hex_radius;
} mliSphericalCapHeagonal;

int mliSphericalCapHeagonal_is_equal(
    const mliSphericalCapHeagonal a,
    const mliSphericalCapHeagonal b) {
    if (a.curvature_radius != b.curvature_radius) return 0;
    if (a.inner_hex_radius != b.inner_hex_radius) return 0;
    return 1;}

#endif
