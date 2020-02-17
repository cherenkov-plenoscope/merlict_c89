/* Copyright 2019 Sebastian Achim Mueller */
#ifndef MERLICT_MLICAMERA_H_
#define MERLICT_MLICAMERA_H_

#include <math.h>
#include <assert.h>
#include <stdint.h>
#include "mliVec.h"
#include "mliHomTra.h"
#include "mliTracer.h"
#include "mliOcTree.h"

typedef struct {
    double x;
    double y;
    double z;
} mliTaitBryanAngles;

typedef struct {
    struct mliVec position;
    mliTaitBryanAngles rotation;
    double field_of_view;
} mliCamera;

typedef struct {
    struct mliVec optical_axis;
    struct mliVec col_axis;
    struct mliVec row_axis;
    struct mliVec principal_point;
    double distance_to_principal_point;
} mliCameraSensor;

struct mliVec mliCamera_optical_axis(const mliCamera cam) {
    mliRotMat rot = mliRotMat_init_tait_bryan(
        cam.rotation.x,
        cam.rotation.y,
        cam.rotation.z);
    return mli_transform_orientation(&rot, mliVec_set(0., 0., 1.));}

struct mliVec mliCamera_direction_right(const mliCamera cam) {
    mliRotMat rot = mliRotMat_init_tait_bryan(
        cam.rotation.x,
        cam.rotation.y,
        cam.rotation.z);
    return mli_transform_orientation(&rot, mliVec_set(0., 1., 0.));}

struct mliVec mliCamera_direction_up(const mliCamera cam) {
    mliRotMat rot = mliRotMat_init_tait_bryan(
        cam.rotation.x,
        cam.rotation.y,
        cam.rotation.z);
    return mli_transform_orientation(&rot, mliVec_set(1., 0., 0.));}

mliCamera mliCamera_move_forward(const mliCamera camin, const double rate) {
    mliCamera camout = camin;
    struct mliVec optical_axis = mliCamera_optical_axis(camin);
    camout.position = mliVec_add(
        camout.position,
        mliVec_multiply(optical_axis, rate));
    return camout;}

mliCamera mliCamera_move_right(const mliCamera camin, const double rate) {
    mliCamera camout = camin;
    struct mliVec direction_right = mliCamera_direction_right(camout);
    camout.position = mliVec_add(
        camout.position,
        mliVec_multiply(direction_right, rate));
    return camout;}

mliCamera mliCamera_move_up(const mliCamera camin, const double rate) {
    mliCamera camout = camin;
    camout.position.z += rate;
    return camout;}

mliCamera mliCamera_look_right(const mliCamera camin, const double rate) {
    mliCamera camout = camin;
    const double diff = camin.field_of_view*rate;
    camout.rotation.z = fmod(camout.rotation.z + diff, (2.*MLI_PI));
    return camout;}

mliCamera mliCamera_look_down_when_possible(
    const mliCamera camin,
    const double rate) {
    const double diff = camin.field_of_view*rate;
    mliCamera camout = camin;
    const int fals_forward_over = camin.rotation.y < -MLI_PI + diff;
    if (fals_forward_over) {
        camout.rotation.y = - MLI_PI;
    } else {
        camout.rotation.y = camout.rotation.y - diff;
    }
    return camout;
}

mliCamera mliCamera_increase_fov(
    const mliCamera camin,
    const double rate) {
    mliCamera camout = camin;
    if (camout.field_of_view * rate > mli_deg2rad(170)) {
        camout.field_of_view = mli_deg2rad(170);
    } else {
        camout.field_of_view *= rate;
    }
    return camout;}

mliCamera mliCamera_decrease_fov(
    const mliCamera camin,
    const double rate) {
    mliCamera camout = camin;
    if (camout.field_of_view / rate < mli_deg2rad(.1)) {
        camout.field_of_view = mli_deg2rad(.1);
    } else {
        camout.field_of_view /= rate;
    }
    return camout;}

mliCamera mliCamera_look_up_when_possible(
    const mliCamera camin,
    const double rate) {
    const double diff = camin.field_of_view*rate;
    mliCamera camout = camin;
    const int fals_backwards_over = camin.rotation.y > diff;
    if (fals_backwards_over) {
        camout.rotation.y = 0.;
    } else {
        camout.rotation.y = camout.rotation.y + diff;
    }
    return camout;
}

void mliCameraSensor_init(
    mliCameraSensor *sensor,
    const mliCamera *camera,
    const mliImage *image) {
    mliRotMat rot;
    struct mliVec unit_x = {1., 0., 0.};
    struct mliVec unit_y = {0., 1., 0.};
    struct mliVec unit_z = {0., 0., 1.};
    rot = mliRotMat_init_tait_bryan(
        camera->rotation.x,
        camera->rotation.y,
        camera->rotation.z);
    sensor->optical_axis = mli_transform_orientation(&rot, unit_z);
    sensor->col_axis = mli_transform_orientation(&rot, unit_y);
    sensor->row_axis = mli_transform_orientation(&rot, unit_x);
    sensor->distance_to_principal_point = (
        (.5*image->num_cols)/tan(.5*camera->field_of_view));
    sensor->principal_point = mliVec_multiply(
        sensor->optical_axis,
        sensor->distance_to_principal_point);}

struct mliRay mliCamera_ray_at_row_col(
    const mliCamera *camera,
    const mliCameraSensor *sensor,
    const mliImage *image,
    const uint32_t row,
    const uint32_t col) {
    int row_idx_on_sensor = row - image->num_rows/2;
    int col_idx_on_sensor = col - image->num_cols/2;
    struct mliVec s_row = mliVec_multiply(sensor->row_axis, row_idx_on_sensor);
    struct mliVec s_col = mliVec_multiply(sensor->col_axis, col_idx_on_sensor);
    struct mliVec sensor_intersection = sensor->principal_point;
    sensor_intersection = mliVec_add(sensor_intersection, s_row);
    sensor_intersection = mliVec_add(sensor_intersection, s_col);
    return mliRay_set(camera->position, sensor_intersection);}

void mliCamera_render_image(
    const mliCamera *camera,
    const mliScenery *scenery,
    const mliOcTree *octree,
    mliImage *image) {
    uint32_t row, col;
    mliCameraSensor sensor;
    assert(camera->field_of_view > 0.);
    assert(camera->field_of_view < mli_deg2rad(180.));
    mliCameraSensor_init(&sensor, camera, image);
    for (row = 0; row < image->num_rows; row++) {
        for (col = 0; col < image->num_cols; col++) {
            struct mliRay ray = mliCamera_ray_at_row_col(
                camera,
                &sensor,
                image,
                row,
                col);
            mliColor color = mli_trace(scenery, octree, ray);
            mliImage_set(image, col, row, color);
        }
    }
}

#endif
