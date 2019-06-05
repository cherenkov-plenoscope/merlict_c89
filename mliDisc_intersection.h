/* Copyright 2019 Sebastian Achim Mueller */
#ifndef MERLICT_MLIDISC_INTERSECTIOM_H_
#define MERLICT_MLIDISC_INTERSECTIOM_H_

#include "mliVec.h"
#include "mliRay.h"
#include "mliHomTra.h"
#include "mliIntersection.h"
#include "mliDisc.h"
#include "mli_xyplane.h"
#include "mli_cylindrical_prism.h"
#include "mli_from_outside_to_inside.h"


int mliDisc_intersection(
    const mliDisc disc,
    const mliHomTraComp local2root_comp,
    const mliRay ray,
    mliIntersection *intersection) {
    mliHomTra local2root = mliHomTra_from_compact(local2root_comp);
    mliRay ray_local = mliHomTra_ray_inverse(&local2root, ray);
    double solution;
    if (mli_xyplane_equation(
        ray_local,
        &solution)
    ) {
        mliVec position_local = mliRay_at(&ray_local, solution);
        if (mli_inside_cylindrical_prism(
            position_local,
            disc.radius)
        ) {
            if (solution < MLI_EPSILON) {
                return 0;
            } else {
                mliVec normal_local = mli_xyplane_surface_normal();
                intersection->position = mliHomTra_pos(
                    &local2root,
                    position_local);
                intersection->surface_normal = mliHomTra_dir(
                    &local2root,
                    normal_local);
                intersection->distance_of_ray = solution;
                intersection->from_outside_to_inside =
                    mli_ray_runs_from_outside_to_inside(
                        ray_local.direction,
                        normal_local);
                return 1;
            }
        }
    }
    return 0;
}

#endif