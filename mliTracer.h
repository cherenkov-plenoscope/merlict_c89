/* Copyright 2019 Sebastian Achim Mueller */
#ifndef MERLICT_MLITRACER_H_
#define MERLICT_MLITRACER_H_

#include <math.h>
#include <stdint.h>
#include "mliVec.h"
#include "mliRay.h"
#include "mliHomTra.h"
#include "mliColor.h"
#include "mliFunc.h"
#include "mliTriangleIntersection.h"
#include "mliScenery.h"
#include "mliIntersection.h"


int first_casual_intersection(
    const mliScenery *scenery,
    const mliRay *ray,
    const int32_t face_coming_from,
    mliIntersection *intersection) {
    int hit = 0;
    int32_t idx_tri;
    int32_t idx_closest_face = 0;
    double smallest_ray_parameter = 9e99;
    double ray_parameter;

    for (idx_tri = 0; idx_tri < scenery->num_triangles; idx_tri++) {
        if (face_coming_from == idx_tri)
            continue;
        if (
            mliRay_intersects_triangle(
                ray->support,
                ray->direction,
                scenery->vertices[scenery->triangles[idx_tri].a],
                scenery->vertices[scenery->triangles[idx_tri].b],
                scenery->vertices[scenery->triangles[idx_tri].c],
                &ray_parameter)
        ) {
            hit = hit + 1;
            if (ray_parameter < smallest_ray_parameter) {
                smallest_ray_parameter = ray_parameter;
                idx_closest_face = idx_tri;
            }
        }
    }
    if (hit) {
        intersection->object_idx = idx_closest_face;
        intersection->position = mliRay_at(ray, smallest_ray_parameter);
        intersection->surface_normal_local = mli_triangle_surface_normal(
            scenery->vertices[scenery->triangles[intersection->object_idx].a],
            scenery->vertices[scenery->triangles[intersection->object_idx].b],
            scenery->vertices[scenery->triangles[intersection->object_idx].c]);
    }
    return hit;
}

mliColor mli_trace(
    const mliScenery *scenery,
    const mliRay *ray) {
    mliColor color = {128., 128., 128.};
    mliIntersection intersection;
    if (
        first_casual_intersection(
            scenery,
            ray,
            MLI_VOID_FACE,
            &intersection)
    ) {
        uint32_t surface_idx;
        uint32_t color_idx;
        mliIntersection global_light_intersection;
        mliRay line_of_sight_to_source;
        mliVec dir_to_source = {1., 1., -3.};
        line_of_sight_to_source = mliRay_set(
            intersection.position,
            dir_to_source);
        surface_idx =
            scenery->triangles_surfaces[intersection.object_idx].outer;
        color_idx = scenery->surfaces[surface_idx].color;
        color = scenery->colors[color_idx];

        if (first_casual_intersection(
                scenery,
                &line_of_sight_to_source,
                intersection.object_idx,
                &global_light_intersection)
        ) {
            color.r = color.r*.5;
            color.g = color.g*.5;
            color.b = color.b*.5;
        }
    }

    return color;
}

#endif
