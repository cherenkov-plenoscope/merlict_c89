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

#define MLI_VOID_FACE -1

typedef struct {
    uint32_t idx_face;
    mliVec position;
    mliVec surface_normal;
} mliIntersection;

int first_casual_intersection(
    const mliScenery *scenery,
    const mliRay *ray,
    int32_t face_coming_from,
    mliIntersection *intersection) {
    int hit = 0;
    int32_t idx_face;
    int32_t idx_closest_face = 0;
    double smallest_ray_parameter = 9e99;
    double ray_parameter;

    for (idx_face = 0; idx_face < scenery->mesh.num_faces; idx_face++) {
        if (face_coming_from == idx_face)
            continue;

        if (
            mliRay_intersects_triangle(
                &ray->support,
                &ray->direction,
                &scenery->mesh.vertices[scenery->mesh.faces[idx_face].a],
                &scenery->mesh.vertices[scenery->mesh.faces[idx_face].b],
                &scenery->mesh.vertices[scenery->mesh.faces[idx_face].c],
                &ray_parameter)
        ) {
            hit = hit + 1;
            if (ray_parameter < smallest_ray_parameter) {
                smallest_ray_parameter = ray_parameter;
                idx_closest_face = idx_face;
            }
        }
    }
    if (hit) {
        intersection->idx_face = idx_closest_face;
        intersection->position = mliRay_at(ray, smallest_ray_parameter);
        intersection->surface_normal = mli_triangle_surface_normal(
                &scenery->mesh.vertices[
                    scenery->mesh.faces[intersection->idx_face].a],
                &scenery->mesh.vertices[
                    scenery->mesh.faces[intersection->idx_face].b],
                &scenery->mesh.vertices[
                    scenery->mesh.faces[intersection->idx_face].c]);
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
        mliIntersection global_light_intersection;
        mliRay line_of_sight_to_source;
        mliVec dir_to_source = {1., 1., -3.};
        line_of_sight_to_source = mliRay_set(
            intersection.position,
            dir_to_source);
        color = scenery->colors[scenery->mesh_colors[intersection.idx_face]];

        if (first_casual_intersection(
                scenery,
                &line_of_sight_to_source,
                intersection.idx_face,
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
