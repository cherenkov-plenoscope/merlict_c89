/* Copyright 2019 Sebastian Achim Mueller */
#ifndef MERLICT_MLISPHERICALCAPHEX_INTERSECTION_H_
#define MERLICT_MLISPHERICALCAPHEX_INTERSECTION_H_

#include <math.h>
#include "mli_math.h"
#include "mliVec.h"
#include "mliRay.h"
#include "mliHomTra.h"
#include "mliHexagonalPrismZ.h"
#include "mliOuterPrismBound.h"
#include "mliIntersection.h"
#include "mli_spherical_cap.h"
#include "mli_from_outside_to_inside.h"

int mliSphericalCapHex_intersection(
    const struct mliSphericalCapHex cap,
    const struct mliHomTraComp local2root_comp,
    const struct mliRay ray,
    struct mliIntersection *intersection) {
    struct mliHomTra local2root = mliHomTra_from_compact(local2root_comp);
    struct mliRay ray_local = mliHomTra_ray_inverse(&local2root, ray);

    double plus_solution, minus_solution;
    if (
        mli_spherical_cap_equation(
            ray_local,
            cap.curvature_radius,
            &plus_solution,
            &minus_solution)
    ) {
        double causal_solution;
        struct mliBoundSurfaceChecklist cl;
        struct mliVec plus_intersec, minus_intersec;
        cl.plus_solution = plus_solution;
        cl.minus_solution = minus_solution;
        plus_intersec = mliRay_at(&ray_local, cl.plus_solution);
        minus_intersec = mliRay_at(&ray_local, cl.minus_solution);
        cl.plus_is_inside = mli_inside_hexagonal_prism_z(
            plus_intersec,
            cap.inner_hex_radius);
        cl.minus_is_inside = mli_inside_hexagonal_prism_z(
            minus_intersec,
            cap.inner_hex_radius);

        if (mli_outer_bound_surface_causal_intersection(
                cl,
                &causal_solution)
        ) {
            if (causal_solution < MLI_EPSILON) {
                return 0;
            } else {
                struct mliVec position_local = mliRay_at(
                    &ray_local,
                    causal_solution);
                if (position_local.z >= cap.curvature_radius) {
                    return 0;
                } else {
                    struct mliVec normal_local =
                        mli_spherical_cap_surface_normal(
                            position_local,
                            cap.curvature_radius);

                    intersection->distance_of_ray = causal_solution;
                    intersection->position =  mliHomTra_pos(
                        &local2root,
                        position_local);
                    intersection->surface_normal = mliHomTra_dir(
                        &local2root,
                        normal_local);
                    intersection->from_outside_to_inside =
                        mli_ray_runs_from_outside_to_inside(
                            ray_local.direction,
                            normal_local);

                    return 1;
                }
            }
        }
    }
    return 0;
}

#endif
