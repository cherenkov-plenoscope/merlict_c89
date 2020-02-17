/* Copyright 2019 Sebastian Achim Mueller */
#ifndef MERLICT_MLIQUATERNION_H_
#define MERLICT_MLIQUATERNION_H_

#include <math.h>
#include <float.h>
#include <stdint.h>
#include "mliVec.h"

struct mliQuaternion{
    double w;
    double x;
    double y;
    double z;
};

struct mliQuaternion mliQuaternion_set(
    const double w,
    const double x,
    const double y,
    const double z) {
    struct mliQuaternion o;
    o.w = w;
    o.x = x;
    o.y = y;
    o.z = z;
    return o;}

int mliQuaternion_is_equal(
    const struct mliQuaternion a,
    const struct mliQuaternion b) {
    if (fabs(a.w - b.w) > DBL_EPSILON) return 0;
    if (fabs(a.x - b.x) > DBL_EPSILON) return 0;
    if (fabs(a.y - b.y) > DBL_EPSILON) return 0;
    if (fabs(a.z - b.z) > DBL_EPSILON) return 0;
    return 1;}

int mliQuaternion_is_equal_margin(
    const struct mliQuaternion a,
    const struct mliQuaternion b,
    const double margin) {
    if (fabs(a.w - b.w) >= margin) {return 0;}
    if (fabs(a.x - b.x) >= margin) {return 0;}
    if (fabs(a.y - b.y) >= margin) {return 0;}
    if (fabs(a.z - b.z) >= margin) {return 0;}
    return 1;}

struct mliQuaternion mliQuaternion_complex_conjugate(
    const struct mliQuaternion q) {
    struct mliQuaternion c;
    c.w = q.w;
    c.x = -q.x;
    c.y = -q.y;
    c.z = -q.z;
    return c;}

struct mliQuaternion mliQuaternion_product(
    const struct mliQuaternion p,
    const struct mliQuaternion q) {
    struct mliQuaternion pq;
    const mliVec P = mliVec_set(p.x, p.y, p.z);
    const mliVec Q = mliVec_set(q.x, q.y, q.z);
    const mliVec P_cross_Q = mliVec_cross(P, Q);
    pq.w = p.w*q.w - mliVec_dot(P, Q);
    pq.x = p.w*Q.x + q.w*P.x + P_cross_Q.x;
    pq.y = p.w*Q.y + q.w*P.y + P_cross_Q.y;
    pq.z = p.w*Q.z + q.w*P.z + P_cross_Q.z;
    return pq;}

double mliQuaternion_product_complex_conjugate(const struct mliQuaternion p) {
    return p.w*p.w + p.x*p.x + p.y*p.y + p.z*p.z;}

double mliQuaternion_norm(const struct mliQuaternion q) {
    return sqrt(mliQuaternion_product_complex_conjugate(q));}

struct mliQuaternion mliQuaternion_set_rotaxis_and_angle(
    const mliVec rot_axis,
    const double angle) {
    struct mliQuaternion quat;
    const double angle_half = .5*angle;
    const double sin_angle_half = sin(angle_half);
    quat.w = cos(angle_half);
    quat.x = rot_axis.x * sin_angle_half;
    quat.y = rot_axis.y * sin_angle_half;
    quat.z = rot_axis.z * sin_angle_half;
    return quat;}

mliRotMat mliQuaternion_to_matrix(const struct mliQuaternion quat) {
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
    /* const double 2yw */
    /* const double 2yx */
    const double _2yz = 2. * quat.y * quat.z;
    o.r00 = w2 + x2 - y2 - z2;
    o.r01 = _2xy - _2wz;
    o.r02 = _2xz + _2wy;
    o.r10 = _2xy + _2wz;
    o.r11 = w2 - x2 + y2 - z2;
    o.r12 = _2yz - _2wx;
    o.r20 = _2xz - _2wy;
    o.r21 = _2yz + _2wx;
    o.r22 = w2 - x2 - y2 + z2;
    return o;}

struct mliQuaternion mliQuaternion_set_tait_bryan(
    const double rx,
    const double ry,
    const double rz) {
    const struct mliQuaternion qz = mliQuaternion_set_rotaxis_and_angle(
        mliVec_set(0,0,1),
        -rz);
    const struct mliQuaternion qy = mliQuaternion_set_rotaxis_and_angle(
        mliVec_set(0,1,0),
        -ry);
    const struct mliQuaternion qx = mliQuaternion_set_rotaxis_and_angle(
        mliVec_set(1,0,0),
        -rx);
    const struct mliQuaternion qz_qy = mliQuaternion_product(qz, qy);
    return mliQuaternion_product(qz_qy, qx);}

void mliQuaternion_print(const struct mliQuaternion q) {
    printf("(w:%f, [%f, %f, %f])", q.w, q.x, q.y, q.z);}

#endif
