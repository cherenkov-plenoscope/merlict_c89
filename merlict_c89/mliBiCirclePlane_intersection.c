/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "mliBiCirclePlane_intersection.h"

int mliBiCirclePlane_intersection(
        const struct mliBiCirclePlane plane,
        const struct mliHomTraComp local2root_comp,
        const struct mliRay ray,
        struct mliIntersection *intersection)
{
        struct mliHomTra local2root = mliHomTra_from_compact(local2root_comp);
        struct mliRay ray_local = mliHomTra_ray_inverse(&local2root, ray);
        double solution = MLI_NAN;
        if (mli_xyplane_equation(ray_local, &solution)) {
                struct mliVec position_local = mliRay_at(&ray_local, solution);
                if (mli_is_inside_dual_circle_prism(
                            position_local.x,
                            position_local.y,
                            plane.x_height,
                            plane.y_width)) {
                        if (solution < MLI_EPSILON) {
                                return 0;
                        } else {
                                struct mliVec normal_local =
                                        mli_xyplane_surface_normal();
                                intersection->position = mliHomTra_pos(
                                        &local2root, position_local);
                                intersection->surface_normal = mliHomTra_dir(
                                        &local2root, normal_local);
                                intersection->position_local = position_local;
                                intersection->surface_normal_local =
                                        normal_local;
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