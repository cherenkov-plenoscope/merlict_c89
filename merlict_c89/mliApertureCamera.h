/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MERLICT_C89_MLIAPERTURECAMERA_H_
#define MERLICT_C89_MLIAPERTURECAMERA_H_

#include <math.h>
#include <assert.h>
#include <stdint.h>

#include "mliMT19937.h"
#include "mliVec.h"
#include "mliHomTra.h"

/*
principal-rays of the thin-lens
===============================
                                        | z
                                        |
        (A)                           --+-- object-distance
          \                             |
         | \\                           |
            \\                          |
         |   \\                         |
              \ \                       |
         |     \ \                      |
                \ \                     |
         |       \  \                   |
                 \   \                  |
         |        \   \                 |
                   \    \               |
         |          \    \              |
                     \    \             |
         |            \     \           |
                       \     \          |
         |             \      \         |
                        \       \       |
         |               \       \      |
                          \       \     |
         |                 \        \   |
                            \        \  |
         |                   \        \ |
                             \         \|
         |                    \       --+--  focal-length
                               \        |\
         |                      \       | \
                                 \      |  \
         |                        \     |   \
                                  \     |     \
         |                         \    |      \
                                    \   |       \
         |                           \  |         \
                                      \ |          \
         |                             \|           \      aperture-plane
   -|----O------------------------------O------------O----------------------|-
          \                             |\                                  |
             \                          | \          |                aperture-
                \                       |  \                           radius
                   \                    |   \        |
                      \                 |    \
                         \              |     \      |
                            \           |     \
                               \        |      \     |
                                  \     |       \
                                     \  |        \   |
                        focal-length  --+--       \
                                        | \       \  |
                                        |    \     \
                                        |       \   \|
    image-sensor-plane                  |          \\
                ------------------------+-----------(P)----------  x/y
                                        |\_ image-sensor-distance
                                        |

1)      Find point P on image-sensor-plane for (row, column).
        With P.z = -image-sensor-distance.
        Add random-scatter in pixel-bin.

2)      Find point A on the object-plane.
        With A.z = +object-distance

3)      Draw random point W on aperture-plane within aperture-radius.

4)      Trace ray(P - W) and assign to pixel (row, column).

*/

struct mliVec mliApCam_get_pixel_center_on_image_sensor_plane(
        const double image_sensor_width_x,
        const double image_sensor_width_y,
        const double image_sensor_distance,
        const uint64_t num_pixel_x,
        const uint64_t num_pixel_y,
        const uint64_t pixel_x,
        const uint64_t pixel_y);

struct mliVec mliApCam_get_random_pixel_support_on_image_sensor_plane(
        const double image_sensor_width_x,
        const double image_sensor_width_y,
        const double image_sensor_distance,
        const uint64_t num_pixel_x,
        const uint64_t num_pixel_y,
        const uint64_t pixel_x,
        const uint64_t pixel_y,
        struct mliMT19937 *prng);

struct mliVec mliApCam_get_object_point(
        const double focal_length,
        const struct mliVec pixel_support);

double mli_object_given_focal_and_image(
        const double focal_length,
        const double image_distance);

double mli_image_given_focal_and_object(
        const double focal_length,
        const double object_distance);

double mliApCam_focal_length_given_field_of_view_and_sensor_width(
        const double field_of_view,
        const double image_sensor_width);

struct mliVec mliApCam_ray_support_on_aperture(
        const double aperture_radius,
        struct mliMT19937 *prng);

struct mliRay mliApCam_get_ray_for_pixel(
        const double focal_length,
        const double aperture_radius,
        const double image_sensor_distance,
        const double image_sensor_width_x,
        const double image_sensor_width_y,
        const uint64_t num_pixel_x,
        const uint64_t num_pixel_y,
        const uint64_t pixel_x,
        const uint64_t pixel_y,
        struct mliMT19937 *prng);

struct mliApertureCamera {
        double focal_length;
        double aperture_radius;
        double image_sensor_distance;
        double image_sensor_width_x;
        double image_sensor_width_y;
};

void mliApertureCamera_render_image(
        struct mliMT19937 *prng,
        const struct mliApertureCamera camera,
        const struct mliHomTraComp camera2root_comp,
        const struct mliScenery *scenery,
        const struct mliOcTree *octree,
        struct mliImage *image);

#endif
