/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "mliPinHoleCamera.h"
#include <math.h>
#include <assert.h>
#include "../image/image_PixelWalk.h"
#include "../math/math.h"
#include "../homtra/homtra.h"

struct mliPinHoleCamera mliPinHoleCamera_init(
        const double field_of_view,
        const struct mli_Image *image,
        const double row_over_column_pixel_ratio)
{
        struct mliPinHoleCamera sensor;

        assert(field_of_view > 0.);
        assert(field_of_view < mli_math_deg2rad(180.));

        sensor.optical_axis = mli_Vec_init(0.0, 0.0, 1.0);
        sensor.col_axis = mli_Vec_init(1.0, 0.0, 0.0);
        sensor.row_axis = mli_Vec_init(0.0, row_over_column_pixel_ratio, 0.0);

        sensor.distance_to_principal_point =
                ((0.5 * mli_Image_num_cols(image)) / tan(0.5 * field_of_view));
        sensor.principal_point = mli_Vec_multiply(
                sensor.optical_axis, sensor.distance_to_principal_point);
        return sensor;
}

struct mli_Ray mliPinHoleCamera_ray_at_row_col(
        const struct mliPinHoleCamera *camera,
        const struct mli_Image *image,
        const uint32_t row,
        const uint32_t col)
{
        struct mli_Vec pin_hole_position = mli_Vec_init(0.0, 0.0, 0.0);
        int row_idx_on_sensor = row - mli_Image_num_rows(image) / 2;
        int col_idx_on_sensor = col - mli_Image_num_cols(image) / 2;
        struct mli_Vec s_row =
                mli_Vec_multiply(camera->row_axis, row_idx_on_sensor);
        struct mli_Vec s_col =
                mli_Vec_multiply(camera->col_axis, col_idx_on_sensor);
        struct mli_Vec sensor_intersection = camera->principal_point;
        sensor_intersection = mli_Vec_add(sensor_intersection, s_row);
        sensor_intersection = mli_Vec_add(sensor_intersection, s_col);
        return mli_Ray_set(pin_hole_position, sensor_intersection);
}

void mliPinHoleCamera_render_image(
        struct mliPinHoleCamera camera,
        const struct mli_HomTraComp camera2root_comp,
        const struct mliTracer *tracer,
        struct mli_Image *image,
        struct mli_Prng *prng)
{
        struct mli_image_PixelWalk walk = mli_image_PixelWalk_init();
        struct mli_HomTra camera2root =
                mli_HomTraComp_from_compact(camera2root_comp);
        uint32_t i;
        const uint32_t num_pixel = mli_Image_num_pixel(image);

        for (i = 0; i < num_pixel; i++) {
                struct mli_image_Pixel px =
                        mli_image_PixelWalk_get_Pixel(&walk, &image->geometry);
                struct mli_Ray ray_wrt_camera = mliPinHoleCamera_ray_at_row_col(
                        &camera, image, px.row, px.col);

                struct mli_Ray ray_wrt_root =
                        mli_HomTraComp_ray(&camera2root, ray_wrt_camera);

                struct mli_Color color =
                        mliTracer_trace_ray(tracer, ray_wrt_root, prng);
                mli_Image_set_by_col_row(image, px.col, px.row, color);
                mli_image_PixelWalk_walk(&walk, &image->geometry);
        }
}

void mliPinHoleCamera_render_image_with_view(
        const struct mli_View view,
        const struct mliTracer *tracer,
        struct mli_Image *image,
        const double row_over_column_pixel_ratio,
        struct mli_Prng *prng)
{
        struct mliPinHoleCamera camera = mliPinHoleCamera_init(
                view.field_of_view, image, row_over_column_pixel_ratio);
        struct mli_HomTraComp camera2root_comp = mli_View_to_HomTraComp(view);
        mliPinHoleCamera_render_image(
                camera, camera2root_comp, tracer, image, prng);
}
