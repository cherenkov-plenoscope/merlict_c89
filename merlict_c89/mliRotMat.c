/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "mliRotMat.h"

/* Copyright 2019 Sebastian Achim Mueller */

struct mliRotMat mliRotMat_init_tait_bryan(
        const double rx,
        const double ry,
        const double rz)
{
        struct mliRotMat rot;
        const double cosRx = cos(rx);
        const double cosRy = cos(ry);
        const double cosRz = cos(rz);
        const double sinRx = sin(rx);
        const double sinRy = sin(ry);
        const double sinRz = sin(rz);
        rot.r00 = cosRy * cosRz;
        rot.r01 = cosRx * sinRz + sinRx * sinRy * cosRz;
        rot.r02 = sinRx * sinRz - cosRx * sinRy * cosRz;
        rot.r10 = -cosRy * sinRz;
        rot.r11 = cosRx * cosRz - sinRx * sinRy * sinRz;
        rot.r12 = sinRx * cosRz + cosRx * sinRy * sinRz;
        rot.r20 = sinRy;
        rot.r21 = -sinRx * cosRy;
        rot.r22 = cosRx * cosRy;
        return rot;
}

struct mliRotMat mliRotMat_init_axis(
        const struct mliVec rot_axis,
        const double rot_angle)
{
        struct mliRotMat rot;
        const double norm = mliVec_norm(rot_axis);
        const double rx = rot_axis.x / norm;
        const double ry = rot_axis.y / norm;
        const double rz = rot_axis.z / norm;
        const double sinR = sin(-rot_angle);
        const double cosR = cos(-rot_angle);
        rot.r00 = cosR + rx * rx * (1. - cosR);
        rot.r01 = rx * ry * (1. - cosR) - rz * sinR;
        rot.r02 = rx * rz * (1. - cosR) + ry * sinR;
        rot.r10 = ry * rx * (1. - cosR) + rz * sinR;
        rot.r11 = cosR + ry * ry * (1. - cosR);
        rot.r12 = ry * rz * (1. - cosR) - rx * sinR;
        rot.r20 = rz * rx * (1. - cosR) - ry * sinR;
        rot.r21 = rz * ry * (1. - cosR) + rx * sinR;
        rot.r22 = cosR + rz * rz * (1. - cosR);
        return rot;
}

void mliRotMat_print(const struct mliRotMat rot)
{
        printf("[%.2f, %.2f, %.2f]\n", rot.r00, rot.r01, rot.r02);
        printf("[%.2f, %.2f, %.2f]\n", rot.r10, rot.r11, rot.r12);
        printf("[%.2f, %.2f, %.2f]", rot.r20, rot.r21, rot.r22);
}

int mliRotMat_equal_margin(
        const struct mliRotMat a,
        const struct mliRotMat b,
        const double margin)
{
        if (fabs(a.r00 - b.r00) > margin)
                return 0;
        if (fabs(a.r01 - b.r01) > margin)
                return 0;
        if (fabs(a.r02 - b.r02) > margin)
                return 0;
        if (fabs(a.r10 - b.r10) > margin)
                return 0;
        if (fabs(a.r11 - b.r11) > margin)
                return 0;
        if (fabs(a.r12 - b.r12) > margin)
                return 0;
        if (fabs(a.r20 - b.r20) > margin)
                return 0;
        if (fabs(a.r21 - b.r21) > margin)
                return 0;
        if (fabs(a.r22 - b.r22) > margin)
                return 0;
        return 1;
}
