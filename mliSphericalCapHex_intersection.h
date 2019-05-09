/* Copyright 2019 Sebastian Achim Mueller */
#ifndef MERLICT_MLISPHERICALCAPHEX_INTERSECTION_H_
#define MERLICT_MLISPHERICALCAPHEX_INTERSECTION_H_

#include <math.h>
#include "mliVec.h"
#include "mliRay.h"
#include "mliHomTra.h"
#include "mliHexagonalPrismZ.h"
#include "mliOuterPrismBound.h"
#include "mliIntersection.h"
#include "mli_spherical_cap.h"
#include "mli_from_outside_to_inside.h"

int mliSphericalCapHex_intersection(
    const mliSphericalCapHex cap,
    const mliHomTraComp root2local_comp,
    const mliRay ray,
    mliIntersection *intersection) {
    mliHomTra root2local = mliHomTra_from_compact(root2local_comp);
    mliRay ray_local = mliHomTra_ray(&root2local, ray);

    double plus_solution, minus_solution;
    if (
        mli_spherical_cap_equation(
            ray_local,
            cap.curvature_radius,
            &plus_solution,
            &minus_solution)
    ) {
        double causal_solution;
        mliBoundSurfaceChecklist cl;
        cl.plus_solution = plus_solution;
        cl.minus_solution = minus_solution;
        cl.plus_intersec = mliRay_at(&ray_local, cl.plus_solution);
        cl.minus_intersec = mliRay_at(&ray_local, cl.minus_solution);
        cl.plus_is_inside = mli_inside_hexagonal_prism_z(
            cl.plus_intersec,
            cap.inner_hex_radius);
        cl.minus_is_inside = mli_inside_hexagonal_prism_z(
            cl.minus_intersec,
            cap.inner_hex_radius);

        if (mli_outer_bound_surface_causal_intersection(
                cl,
                &causal_solution)
        ) {
            mliVec position_local = mliRay_at(&ray_local, causal_solution);
            mliVec normal_local = mli_spherical_cap_surface_normal(
                position_local,
                cap.curvature_radius);

            intersection->distance_of_ray = causal_solution;
            intersection->position =  mliHomTra_pos_inverse(
                &root2local,
                position_local);
            intersection->surface_normal = mliHomTra_dir_inverse(
                &root2local,
                normal_local);
            intersection->from_outside_to_inside =
                mli_ray_runs_from_outside_to_inside(
                    ray_local.direction,
                    normal_local);

            return 1;
        }
    }
    return 0;
}

#endif
