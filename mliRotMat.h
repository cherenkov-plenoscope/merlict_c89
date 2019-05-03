/* Copyright 2019 Sebastian Achim Mueller */
#ifndef MERLICT_MLIROTMAT_H_
#define MERLICT_MLIROTMAT_H_

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

#endif
