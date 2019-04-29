/* Copyright 2019 Sebastian Achim Mueller */
#ifndef MERLICT_MLIHOMTRA_H_
#define MERLICT_MLIHOMTRA_H_

#include <math.h>
#include <stdint.h>
#include "mliVec.h"

typedef struct {
    double r00;
    double r01;
    double r02;
    double r10;
    double r11;
    double r12;
    double r20;
    double r21;
    double r22;
} mliRotMat;

mliRotMat mliRotMat_init_tait_bryan(
    const double rx,
    const double ry,
    const double rz) {
    mliRotMat rot;
    const double cosRx = cos(rx);
    const double cosRy = cos(ry);
    const double cosRz = cos(rz);
    const double sinRx = sin(rx);
    const double sinRy = sin(ry);
    const double sinRz = sin(rz);
    rot.r00 = cosRy*cosRz;
    rot.r01 = cosRx*sinRz + sinRx*sinRy*cosRz;
    rot.r02 = sinRx*sinRz - cosRx*sinRy*cosRz;
    rot.r10 =-cosRy*sinRz;
    rot.r11 = cosRx*cosRz - sinRx*sinRy*sinRz;
    rot.r12 = sinRx*cosRz + cosRx*sinRy*sinRz;
    rot.r20 = sinRy;
    rot.r21 =-sinRx*cosRy;
    rot.r22 = cosRx*cosRy;
    return rot;}

mliRotMat mliRotMat_init_axis(
    const mliVec* rot_axis,
    const double rot_angle) {
    mliRotMat rot;
    const double norm = mliVec_norm(rot_axis);
    const double rx = rot_axis->x/norm;
    const double ry = rot_axis->y/norm;
    const double rz = rot_axis->z/norm;
    const double sinR = sin(-rot_angle);
    const double cosR = cos(-rot_angle);
    rot.r00 = cosR +  rx*rx*(1.-cosR);
    rot.r01 = rx*ry*(1.-cosR)-rz*sinR;
    rot.r02 = rx*rz*(1.-cosR)+ry*sinR;
    rot.r10 = ry*rx*(1.-cosR)+rz*sinR;
    rot.r11 = cosR +  ry*ry*(1.-cosR);
    rot.r12 = ry*rz*(1.-cosR)-rx*sinR;
    rot.r20 = rz*rx*(1.-cosR)-ry*sinR;
    rot.r21 = rz*ry*(1.-cosR)+rx*sinR;
    rot.r22 = cosR +  rz*rz*(1.-cosR);
    return rot;}


mliVec mli_transform_orientation(
    const mliRotMat *rot,
    const mliVec *in) {
    mliVec out;
    out.x = in->x*rot->r00 + in->y*rot->r01 + in->z*rot->r02;
    out.y = in->x*rot->r10 + in->y*rot->r11 + in->z*rot->r12;
    out.z = in->x*rot->r20 + in->y*rot->r21 + in->z*rot->r22;
    return out;}

mliVec mli_transform_orientation_inverse(
    const mliRotMat *rot,
    const mliVec *in) {
    mliVec out;
    out.x = in->x*rot->r00 + in->y*rot->r10 + in->z*rot->r20;
    out.y = in->x*rot->r01 + in->y*rot->r11 + in->z*rot->r21;
    out.z = in->x*rot->r02 + in->y*rot->r12 + in->z*rot->r22;
    return out;}

mliVec mli_transform_position(
    const mliRotMat *rot,
    const mliVec *trans,
    const mliVec *in) {
    mliVec out;
    out.x = in->x*rot->r00 + in->y*rot->r01 + in->z*rot->r02 + trans->x;
    out.y = in->x*rot->r10 + in->y*rot->r11 + in->z*rot->r12 + trans->y;
    out.z = in->x*rot->r20 + in->y*rot->r21 + in->z*rot->r22 + trans->z;
    return out;}

mliVec mli_transform_position_inverse(
    const mliRotMat *rot,
    const mliVec *trans,
    const mliVec *in) {
    mliVec out;
    out.x = in->x*rot->r00 + in->y*rot->r10 + in->z*rot->r20 -
        (rot->r00*trans->x + rot->r10*trans->y + rot->r20*trans->z);
    out.y = in->x*rot->r01 + in->y*rot->r11 + in->z*rot->r21 -
        (rot->r01*trans->x + rot->r11*trans->y + rot->r21*trans->z);
    out.z = in->x*rot->r02 + in->y*rot->r12 + in->z*rot->r22 -
        (rot->r02*trans->x + rot->r12*trans->y + rot->r22*trans->z);
    return out;}

#endif
