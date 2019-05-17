/* Copyright 2019 Sebastian Achim Mueller */
#ifndef MERLICT_MLIQUATERNION_H_
#define MERLICT_MLIQUATERNION_H_

#include <math.h>
#include <stdint.h>
#include "mliVec.h"

typedef struct {
    double w;
    double x;
    double y;
    double z;
} mliQuaternion;

int mliQuaternion_is_equal(const mliQuaternion a, const mliQuaternion b) {
    if (a.w != b.w) return 0;
    if (a.x != b.x) return 0;
    if (a.y != b.y) return 0;
    if (a.z != b.z) return 0;
    return 1;}

mliQuaternion mliQuaternion_set_rotaxis_and_angle(
    const mliVec rot_axis,
    const double angle) {
    mliQuaternion quat;
    const double angle_half = .5*angle;
    const double sin_angle_half = sin(angle_half);
    quat.w = cos(angle_half);
    quat.x = rot_axis.x * sin_angle_half;
    quat.y = rot_axis.y * sin_angle_half;
    quat.z = rot_axis.z * sin_angle_half;
    return quat;}

mliRotMat mliQuaternion_to_matrix(const mliQuaternion quat) {
    mliRotMat o;
    const double w2 = quat.w * quat.w;
    const double x2 = quat.x * quat.x;
    const double y2 = quat.y * quat.y;
    const double z2 = quat.z * quat.z;
    const double _2wx = 2. * quat.w * quat.x;
    const double _2wy = 2. * quat.w * quat.y;
    const double _2wz = 2. * quat.w * quat.z;
    /* const double 2xw */
    const double _2xy = 2. * quat.x * quat.y;
    const double _2xz = 2. * quat.x * quat.z;
    /* const double 2ca */
    /* const double 2cb */
    const double _2yz = 2. * quat.y * quat.z;
    o.r00 = w2 + x2 - y2 - z2;
    o.r01 = _2xy - _2wz;
    o.r02 = _2xz + _2wy;
    o.r10 = _2xy + _2wz;
    o.r11 = w2 - x2 + y2 - z2;
    o.r12 = _2yz - _2wx;
    o.r20 = _2xz - _2wy;
    o.r21 = _2yz - _2wx;
    o.r22 = w2 - x2 - y2 + z2;
    return o;}

#endif
