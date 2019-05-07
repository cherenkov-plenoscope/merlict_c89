/* Copyright 2019 Sebastian Achim Mueller */
#ifndef MERLICT_MLISPHERICALCAP_H_
#define MERLICT_MLISPHERICALCAP_H_

#include <math.h>
#include "mliVec.h"
#include "mliRay.h"
#include "mliHexagonalPrismZ.h"
#include "mliOuterPrismBound.h"
#include "mliIntersection.h"
#include "mli_spherical_cap.h"


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
            intersection->surface_normal =
                mli_spherical_cap_surface_normal(
                    intersection->position,
                    curvature_radius);
            intersection->from_outside_to_inside =
                mli_ray_runs_from_outside_to_inside(
                    ray_local->direction,
                    intersection->surface_normal);

            return 1;
        }
    }
    return 0;
}

#define MLI_INNER_TO_OUTER_HEX 1.1547005383792517

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

int mliSphericalCapHex_intersection(
    const mliSphericalCapHeagonal cap,
    const mliHomTra T,
    const mliRay ray,
    mliIntersection *intersection) {
    return 0;}

float mliSphericalCapHex_bounding_radius(
    const mliSphericalCapHeagonal cap) {
    return hypot(
        cap.inner_hex_radius*MLI_INNER_TO_OUTER_HEX,
        z_sphere(
            cap.inner_hex_radius*MLI_INNER_TO_OUTER_HEX,
            cap.curvature_radius));}

#endif
