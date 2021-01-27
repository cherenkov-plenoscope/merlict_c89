/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MERLICT_C89_MLI_PIN_HOLE_CAMERA_H_
#define MERLICT_C89_MLI_PIN_HOLE_CAMERA_H_

#include <math.h>
#include <assert.h>
#include <stdint.h>

#include "mliTracer.h"
#include "mliView.h"

struct mliPinHoleCamera {
        struct mliVec optical_axis;
        struct mliVec col_axis;
        struct mliVec row_axis;
        struct mliVec principal_point;
        double distance_to_principal_point;
        double row_over_column_pixel_ratio;
};

struct mliPinHoleCamera mliPinHoleCamera_init(
        const double field_of_view,
        const struct mliImage *image,
        const double row_over_column_pixel_ratio);

void mli_pin_hole_camera_render_image(
        struct mliPinHoleCamera camera,
        const struct mliHomTraComp camera2root_comp,
        const struct mliCombine *combine,
        struct mliImage *image);

void mli_pin_hole_camera_render_image_with_view(
        const struct mliView view,
        const struct mliCombine *combine,
        struct mliImage *image,
        const double row_over_column_pixel_ratio);

struct mliRay mli_pin_hole_camera_ray_at_row_col(
        const struct mliPinHoleCamera *camera,
        const struct mliImage *image,
        const uint32_t row,
        const uint32_t col);

#endif
