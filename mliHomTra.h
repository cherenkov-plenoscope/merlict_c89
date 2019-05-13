/* Copyright 2019 Sebastian Achim Mueller */
#ifndef MERLICT_MLIHOMTRA_H_
#define MERLICT_MLIHOMTRA_H_

#include <math.h>
#include <stdint.h>
#include "mliVec.h"
#include "mliRotMat.h"
#include "mliQuaternion.h"

typedef struct {
    mliVec trans;
    mliQuaternion rot;
} mliHomTraComp;

typedef struct {
    mliVec trans;
    mliRotMat rot;
} mliHomTra;

mliHomTra mliHomTra_from_compact(const mliHomTraComp trafo) {
    mliHomTra t;
    t.trans = trafo.trans;
    t.rot = mliQuaternion_to_matrix(trafo.rot);
    return t;}

mliVec mli_transform_orientation(
    const mliRotMat *rot,
    const mliVec in) {
    mliVec out;
    out.x = in.x*rot->r00 + in.y*rot->r01 + in.z*rot->r02;
    out.y = in.x*rot->r10 + in.y*rot->r11 + in.z*rot->r12;
    out.z = in.x*rot->r20 + in.y*rot->r21 + in.z*rot->r22;
    return out;}

mliVec mli_transform_orientation_inverse(
    const mliRotMat *rot,
    const mliVec in) {
    mliVec out;
    out.x = in.x*rot->r00 + in.y*rot->r10 + in.z*rot->r20;
    out.y = in.x*rot->r01 + in.y*rot->r11 + in.z*rot->r21;
    out.z = in.x*rot->r02 + in.y*rot->r12 + in.z*rot->r22;
    return out;}

mliVec mli_transform_position(
    const mliRotMat *rot,
    const mliVec trans,
    const mliVec in) {
    mliVec out;
    out.x = in.x*rot->r00 + in.y*rot->r01 + in.z*rot->r02 + trans.x;
    out.y = in.x*rot->r10 + in.y*rot->r11 + in.z*rot->r12 + trans.y;
    out.z = in.x*rot->r20 + in.y*rot->r21 + in.z*rot->r22 + trans.z;
    return out;}

mliVec mli_transform_position_inverse(
    const mliRotMat *rot,
    const mliVec trans,
    const mliVec in) {
    mliVec out;
    out.x = in.x*rot->r00 + in.y*rot->r10 + in.z*rot->r20 -
        (rot->r00*trans.x + rot->r10*trans.y + rot->r20*trans.z);
    out.y = in.x*rot->r01 + in.y*rot->r11 + in.z*rot->r21 -
        (rot->r01*trans.x + rot->r11*trans.y + rot->r21*trans.z);
    out.z = in.x*rot->r02 + in.y*rot->r12 + in.z*rot->r22 -
        (rot->r02*trans.x + rot->r12*trans.y + rot->r22*trans.z);
    return out;}

mliRay mli_transform_ray(
    const mliRotMat *rot,
    const mliVec trans,
    const mliRay in) {
    mliRay out;
    out.support = mli_transform_position(
        rot,
        trans,
        in.support);
    out.direction = mli_transform_orientation(
        rot,
        in.direction);
    return out;}

mliRay mli_transform_ray_inverse(
    const mliRotMat *rot,
    const mliVec trans,
    const mliRay in) {
    mliRay out;
    out.support = mli_transform_position_inverse(
        rot,
        trans,
        in.support);
    out.direction = mli_transform_orientation_inverse(
        rot,
        in.direction);
    return out;}

mliRay mliHomTra_ray(const mliHomTra *t, const mliRay in) {
    return mli_transform_ray(&t->rot, t->trans, in);}

mliRay mliHomTra_ray_inverse(const mliHomTra *t, const mliRay in) {
    return mli_transform_ray_inverse(&t->rot, t->trans, in);}

mliVec mliHomTra_pos(const mliHomTra *t, const mliVec in) {
    return mli_transform_position(&t->rot, t->trans, in);}

mliVec mliHomTra_pos_inverse(const mliHomTra *t, const mliVec in) {
    return mli_transform_position_inverse(&t->rot, t->trans, in);}

mliVec mliHomTra_dir(const mliHomTra *t, const mliVec in) {
    return mli_transform_orientation(&t->rot, in);}

mliVec mliHomTra_dir_inverse(const mliHomTra *t, const mliVec in) {
    return mli_transform_orientation_inverse(&t->rot, in);}

int mliHomTraComp_is_equal(const mliHomTraComp a, const mliHomTraComp b) {
    if (!mliVec_is_equal(a.trans, b.trans))
        return 0;
    if (!mliQuaternion_is_equal(a.rot, b.rot))
        return 0;
    return 1;}

#endif
