/* Copyright 2019 Sebastian Achim Mueller */
#ifndef MERLICT_MLICAMERA_H_
#define MERLICT_MLICAMERA_H_

#include <math.h>
#include <assert.h>
#include <stdint.h>
#include "mliVec.h"
#include "mliHomTra.h"
#include "mliTracer.h"

typedef struct {
    mliVec position;
    mliRotMat rotation;
    double field_of_view;
} mliCamera;

typedef struct {
    mliVec optical_axis;
    mliVec col_axis;
    mliVec row_axis;
} mliCameraSensor;

void mliCameraSensor_init(const mliCamera *camera, mliCameraSensor *sensor) {
    mliVec unit_x = {1., 0., 0.};
    mliVec unit_y = {0., 1., 0.};
    mliVec unit_z = {0., 0., 1.};
    sensor->optical_axis = mli_transform_orientation(&camera->rotation, &unit_z);
    sensor->col_axis = mli_transform_orientation(&camera->rotation, &unit_y);
    sensor->row_axis = mli_transform_orientation(&camera->rotation, &unit_x);}

void mliCamera_render_image(
    const mliCamera *camera,
    const mliScenery *scenery,
    mliImage *image) {
    uint32_t row, col;
    float distance_to_principal_point;
    mliVec principal_point;
    mliCameraSensor sensor;
    assert(camera->field_of_view > 0.);
    assert(camera->field_of_view < mli_deg2rad(180.));
    mliCameraSensor_init(camera, &sensor);
    distance_to_principal_point = (
        (.5*image->num_cols)/tan(.5*camera->field_of_view));
    principal_point = mliVec_multiply(
        &sensor.optical_axis,
        distance_to_principal_point);
    for (row = 0; row < image->num_rows; row++) {
        for (col = 0; col < image->num_cols; col++) {
            int row_idx_on_sensor = row - image->num_rows/2;
            int col_idx_on_sensor = col - image->num_cols/2;
            mliVec s_row = mliVec_multiply(&sensor.row_axis, row_idx_on_sensor);
            mliVec s_col = mliVec_multiply(&sensor.col_axis, col_idx_on_sensor);
            mliRay ray;
            mliColor color;
            mliVec sensor_intersection = principal_point;
            sensor_intersection = mliVec_add(sensor_intersection, s_row);
            sensor_intersection = mliVec_add(sensor_intersection, s_col);
            ray = mliRay_set(camera->position, sensor_intersection);
            color = mli_trace(scenery, &ray);
            mliImage_set(image, col, row, color);
        }
    }
}

#endif
