/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MERLICT_C89_MLI_PIN_HOLE_CAMERA_H_
#define MERLICT_C89_MLI_PIN_HOLE_CAMERA_H_

#include <math.h>
#include <assert.h>
#include <stdint.h>

#include "mliVec.h"
#include "mliHomTra.h"
#include "mliTracer.h"
#include "mliOcTree.h"
#include "mliView.h"

struct mliPinHoleCameraImageSensor {
        struct mliVec optical_axis;
        struct mliVec col_axis;
        struct mliVec row_axis;
        struct mliVec principal_point;
        double distance_to_principal_point;
};

void mli_pin_hole_camera_render_image(
        const struct mliView view,
        const struct mliScenery *scenery,
        const struct mliOcTree *octree,
        struct mliImage *image,
        const double row_over_column_pixel_ratio);
struct mliRay mli_pin_hole_camera_ray_at_row_col(
        const struct mliView view,
        const struct mliPinHoleCameraImageSensor *sensor,
        const struct mliImage *image,
        const uint32_t row,
        const uint32_t col);
struct mliPinHoleCameraImageSensor mliPinHoleCameraImageSensor_init(
        const struct mliView view,
        const struct mliImage *image,
        const double row_over_column_pixel_ratio);
#endif
