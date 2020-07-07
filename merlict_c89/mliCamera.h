/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MERLICT_C89_MLICAMERA_H_
#define MERLICT_C89_MLICAMERA_H_

#include <math.h>
#include <assert.h>
#include <stdint.h>

#include "mliVec.h"
#include "mliHomTra.h"
#include "mliTracer.h"
#include "mliOcTree.h"

struct mliTaitBryanAngles {
        double x;
        double y;
        double z;
};

struct mliCamera {
        struct mliVec position;
        struct mliTaitBryanAngles rotation;
        double field_of_view;
};

struct mliCameraSensor {
        struct mliVec optical_axis;
        struct mliVec col_axis;
        struct mliVec row_axis;
        struct mliVec principal_point;
        double distance_to_principal_point;
};

void mliCamera_render_image(
        const struct mliCamera *camera,
        const struct mliScenery *scenery,
        const struct mliOcTree *octree,
        struct mliImage *image,
        const double row_over_column_pixel_ratio);
struct mliRay mliCamera_ray_at_row_col(
        const struct mliCamera *camera,
        const struct mliCameraSensor *sensor,
        const struct mliImage *image,
        const uint32_t row,
        const uint32_t col);
void mliCameraSensor_init(
        struct mliCameraSensor *sensor,
        const struct mliCamera *camera,
        const struct mliImage *image,
        const double row_over_column_pixel_ratio);
struct mliCamera mliCamera_look_up_when_possible(
        const struct mliCamera camin,
        const double rate);
struct mliCamera mliCamera_decrease_fov(
        const struct mliCamera camin,
        const double rate);
struct mliCamera mliCamera_increase_fov(
        const struct mliCamera camin,
        const double rate);
struct mliCamera mliCamera_look_down_when_possible(
        const struct mliCamera camin,
        const double rate);
struct mliCamera mliCamera_look_right(
        const struct mliCamera camin,
        const double rate);
struct mliCamera mliCamera_move_up(
        const struct mliCamera camin,
        const double rate);
struct mliCamera mliCamera_move_right(
        const struct mliCamera camin,
        const double rate);
struct mliCamera mliCamera_move_forward(
        const struct mliCamera camin,
        const double rate);
struct mliVec mliCamera_direction_up(const struct mliCamera cam);
struct mliVec mliCamera_direction_right(const struct mliCamera cam);
struct mliVec mliCamera_optical_axis(const struct mliCamera cam);
#endif
