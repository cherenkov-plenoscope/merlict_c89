/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "mli_pin_hole_camera.h"

struct mliPinHoleCameraImageSensor mliPinHoleCameraImageSensor_init(
        const struct mliView view,
        const struct mliImage *image,
        const double row_over_column_pixel_ratio)
{
        struct mliPinHoleCameraImageSensor sensor;
        struct mliRotMat rot;
        struct mliVec unit_x = {1., 0., 0.};
        struct mliVec unit_y = {0., 1., 0.};
        struct mliVec unit_z = {0., 0., 1.};
        rot = mliRotMat_init_tait_bryan(
                view.rotation.x, view.rotation.y, view.rotation.z);
        sensor.optical_axis = mli_transform_orientation(&rot, unit_z);
        sensor.col_axis = mli_transform_orientation(&rot, unit_x);
        sensor.row_axis = mli_transform_orientation(&rot, unit_y);
        sensor.row_axis =
                mliVec_multiply(sensor.row_axis, row_over_column_pixel_ratio);
        sensor.distance_to_principal_point =
                ((.5 * image->num_cols) / tan(.5 * view.field_of_view));
        sensor.principal_point = mliVec_multiply(
                sensor.optical_axis, sensor.distance_to_principal_point);
        return sensor;
}

struct mliRay mli_pin_hole_camera_ray_at_row_col(
        const struct mliView view,
        const struct mliPinHoleCameraImageSensor *sensor,
        const struct mliImage *image,
        const uint32_t row,
        const uint32_t col)
{
        int row_idx_on_sensor = row - image->num_rows / 2;
        int col_idx_on_sensor = col - image->num_cols / 2;
        struct mliVec s_row =
                mliVec_multiply(sensor->row_axis, row_idx_on_sensor);
        struct mliVec s_col =
                mliVec_multiply(sensor->col_axis, col_idx_on_sensor);
        struct mliVec sensor_intersection = sensor->principal_point;
        sensor_intersection = mliVec_add(sensor_intersection, s_row);
        sensor_intersection = mliVec_add(sensor_intersection, s_col);
        return mliRay_set(view.position, sensor_intersection);
}

void mli_pin_hole_camera_render_image(
        const struct mliView view,
        const struct mliScenery *scenery,
        const struct mliOcTree *octree,
        struct mliImage *image,
        const double row_over_column_pixel_ratio)
{
        uint32_t row, col;
        struct mliPinHoleCameraImageSensor sensor =
                mliPinHoleCameraImageSensor_init(
                        view, image, row_over_column_pixel_ratio);
        assert(view.field_of_view > 0.);
        assert(view.field_of_view < mli_deg2rad(180.));
        for (row = 0; row < image->num_rows; row++) {
                for (col = 0; col < image->num_cols; col++) {
                        struct mliRay ray = mli_pin_hole_camera_ray_at_row_col(
                                view, &sensor, image, row, col);
                        struct mliColor color = mli_trace(scenery, octree, ray);
                        mliImage_set(image, col, row, color);
                }
        }
}
