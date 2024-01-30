/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "mliPinHoleCamera.h"
#include <math.h>
#include <assert.h>
#include "mliPixelWalk.h"
#include "mli_math.h"
#include "mliHomTra.h"


struct mliPinHoleCamera mliPinHoleCamera_init(
        const double field_of_view,
        const struct mliImage *image,
        const double row_over_column_pixel_ratio)
{
        struct mliPinHoleCamera sensor;

        assert(field_of_view > 0.);
        assert(field_of_view < mli_deg2rad(180.));

        sensor.optical_axis = mliVec_init(0.0, 0.0, 1.0);
        sensor.col_axis = mliVec_init(1.0, 0.0, 0.0);
        sensor.row_axis = mliVec_init(0.0, row_over_column_pixel_ratio, 0.0);

        sensor.distance_to_principal_point =
                ((0.5 * image->num_cols) / tan(0.5 * field_of_view));
        sensor.principal_point = mliVec_multiply(
                sensor.optical_axis, sensor.distance_to_principal_point);
        return sensor;
}

struct mliRay mliPinHoleCamera_ray_at_row_col(
        const struct mliPinHoleCamera *camera,
        const struct mliImage *image,
        const uint32_t row,
        const uint32_t col)
{
        struct mliVec pin_hole_position = mliVec_init(0.0, 0.0, 0.0);
        int row_idx_on_sensor = row - image->num_rows / 2;
        int col_idx_on_sensor = col - image->num_cols / 2;
        struct mliVec s_row =
                mliVec_multiply(camera->row_axis, row_idx_on_sensor);
        struct mliVec s_col =
                mliVec_multiply(camera->col_axis, col_idx_on_sensor);
        struct mliVec sensor_intersection = camera->principal_point;
        sensor_intersection = mliVec_add(sensor_intersection, s_row);
        sensor_intersection = mliVec_add(sensor_intersection, s_col);
        return mliRay_set(pin_hole_position, sensor_intersection);
}

void mliPinHoleCamera_render_image(
        struct mliPinHoleCamera camera,
        const struct mliHomTraComp camera2root_comp,
        const struct mliScenery *scenery,
        struct mliImage *image,
        const struct mliTracerConfig *tracer_config,
        struct mliPrng *prng)
{
        struct mliPixelWalk walk =
                mliPixelWalk_set(image->num_cols, image->num_rows, 16u);
        struct mliHomTra camera2root = mliHomTra_from_compact(camera2root_comp);
        uint32_t i;
        const uint32_t num_pixel = image->num_rows * image->num_cols;

        for (i = 0; i < num_pixel; i++) {
                struct mliPixel px = mliPixelWalk_get(&walk);
                struct mliRay ray_wrt_camera = mliPinHoleCamera_ray_at_row_col(
                        &camera, image, px.row, px.col);

                struct mliRay ray_wrt_root =
                        mliHomTra_ray(&camera2root, ray_wrt_camera);

                struct mliColor color =
                        mli_trace(scenery, ray_wrt_root, tracer_config, prng);
                mliImage_set(image, px.col, px.row, color);
                mliPixelWalk_walk(&walk);
        }
}

void mliPinHoleCamera_render_image_with_view(
        const struct mliView view,
        const struct mliScenery *scenery,
        struct mliImage *image,
        const double row_over_column_pixel_ratio,
        const struct mliTracerConfig *tracer_config,
        struct mliPrng *prng)
{
        struct mliPinHoleCamera camera = mliPinHoleCamera_init(
                view.field_of_view, image, row_over_column_pixel_ratio);
        struct mliHomTraComp camera2root_comp = mliView_to_HomTraComp(view);
        mliPinHoleCamera_render_image(
                camera, camera2root_comp, scenery, image, tracer_config, prng);
}
