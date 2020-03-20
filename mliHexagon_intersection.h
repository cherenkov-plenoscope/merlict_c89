/* Copyright 2019 Sebastian Achim Mueller */
#ifndef MERLICT_MLIHEXAGON_INTERSECTIOM_H_
#define MERLICT_MLIHEXAGON_INTERSECTIOM_H_

#include "mliVec.h"
#include "mliRay.h"
#include "mliHomTra.h"
#include "mliIntersection.h"
#include "mliHexagon.h"
#include "mli_xyplane.h"
#include "mli_from_outside_to_inside.h"

int mliHexagon_intersection(
        const struct mliHexagon hex,
        const struct mliHomTraComp local2root_comp,
        const struct mliRay ray,
        struct mliIntersection *intersection)
{
        struct mliHomTra local2root = mliHomTra_from_compact(local2root_comp);
        struct mliRay ray_local = mliHomTra_ray_inverse(&local2root, ray);
        double solution;
        if (mli_xyplane_equation(ray_local, &solution)) {
                struct mliVec position_local = mliRay_at(&ray_local, solution);
                if (mli_inside_hexagonal_prism_z(
                            position_local, hex.inner_radius)) {
                        if (solution < MLI_EPSILON) {
                                return 0;
                        } else {
                                struct mliVec normal_local =
                                        mli_xyplane_surface_normal();
                                intersection->position = mliHomTra_pos(
                                        &local2root, position_local);
                                intersection->surface_normal = mliHomTra_dir(
                                        &local2root, normal_local);
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
