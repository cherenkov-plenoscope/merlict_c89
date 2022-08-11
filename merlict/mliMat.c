/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "mliMat.h"
#include <math.h>

struct mliMat mliMat_init_tait_bryan(
        const double rx,
        const double ry,
        const double rz)
{
        struct mliMat rot;
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

struct mliMat mliMat_init_axis_angle(
        const struct mliVec axis,
        const double angle)
{
        struct mliMat rot;
        const double norm = mliVec_norm(axis);
        const double rx = axis.x / norm;
        const double ry = axis.y / norm;
        const double rz = axis.z / norm;
        const double sinR = sin(-angle);
        const double cosR = cos(-angle);
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

int mliMat_equal_margin(
        const struct mliMat a,
        const struct mliMat b,
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
