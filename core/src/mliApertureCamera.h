/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLIAPTCAM_H_
#define MLIAPTCAM_H_

#include <stdint.h>
#include "mli_random.h"
#include "mliVec.h"
#include "mliHomTra.h"
#include "mliTracer.h"

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

struct mliVec mliApertureCamera_pixel_center_on_image_sensor_plane(
        const double image_sensor_width_x,
        const double image_sensor_width_y,
        const double image_sensor_distance,
        const uint64_t num_pixel_x,
        const uint64_t num_pixel_y,
        const uint64_t pixel_x,
        const uint64_t pixel_y);

struct mliVec mliApertureCamera_pixel_support_on_image_sensor_plane(
        const double image_sensor_width_x,
        const double image_sensor_width_y,
        const double image_sensor_distance,
        const uint64_t num_pixel_x,
        const uint64_t num_pixel_y,
        const uint64_t pixel_x,
        const uint64_t pixel_y,
        struct mliPrng *prng);

struct mliVec mliApertureCamera_get_object_point(
        const double focal_length,
        const struct mliVec pixel_support);

double mli_thin_lens_get_object_given_focal_and_image(
        const double focal_length,
        const double image_distance);

double mli_thin_lens_get_image_given_focal_and_object(
        const double focal_length,
        const double object_distance);

double mliApertureCamera_focal_length_given_field_of_view_and_sensor_width(
        const double field_of_view,
        const double image_sensor_width);

struct mliVec mliApertureCamera_ray_support_on_aperture(
        const double aperture_radius,
        struct mliPrng *prng);

struct mliRay mliApertureCamera_get_ray_for_pixel(
        const double focal_length,
        const double aperture_radius,
        const double image_sensor_distance,
        const double image_sensor_width_x,
        const double image_sensor_width_y,
        const uint64_t num_pixel_x,
        const uint64_t num_pixel_y,
        const uint64_t pixel_x,
        const uint64_t pixel_y,
        struct mliPrng *prng);

struct mliApertureCamera {
        double focal_length;
        double aperture_radius;
        double image_sensor_distance;
        double image_sensor_width_x;
        double image_sensor_width_y;
};

struct mliApertureCamera mliApertureCamera_init(void);

int mliApertureCamera_render_image(
        const struct mliApertureCamera camera,
        const struct mliHomTraComp camera2root_comp,
        const struct mliScenery *scenery,
        struct mliImage *image,
        const struct mliTracerConfig *tracer_config,
        struct mliPrng *prng);

void mliApertureCamera_aquire_pixels(
        const struct mliApertureCamera camera,
        const struct mliImage *image,
        const struct mliHomTraComp camera2root_comp,
        const struct mliScenery *scenery,
        const struct mliPixels *pixels_to_do,
        struct mliImage *colors,
        const struct mliTracerConfig *tracer_config,
        struct mliPrng *prng);

void mliApertureCamera_assign_pixel_colors_to_sum_and_exposure_image(
        const struct mliPixels *pixels,
        const struct mliImage *colors,
        struct mliImage *sum_image,
        struct mliImage *exposure_image);

#endif
