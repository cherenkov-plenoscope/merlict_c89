/* Copyright 2019 Sebastian Achim Mueller */
#ifndef MERLICT_MLIBICIRCLEPLANE_INTERSECTION_H_
#define MERLICT_MLIBICIRCLEPLANE_INTERSECTION_H_

#include "mliVec.h"
#include "mliRay.h"
#include "mliHomTra.h"
#include "mliIntersection.h"
#include "mliBiCirclePlane.h"
#include "mli_xyplane.h"
#include "mli_from_outside_to_inside.h"


int mliBiCirclePlane_intersection(
    const mliBiCirclePlane plane,
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
        if (mli_is_inside_dual_circle_prism(
                position_local.x,
                position_local.y,
                plane.x_height,
                plane.y_width)
        ) {
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
    return 0;
}

#endif