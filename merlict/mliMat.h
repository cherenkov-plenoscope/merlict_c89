/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLIMAT_H_
#define MLIMAT_H_

#include "mliVec.h"

struct mliMat {
        double r00;
        double r01;
        double r02;
        double r10;
        double r11;
        double r12;
        double r20;
        double r21;
        double r22;
};

int mliMat_equal_margin(
        const struct mliMat a,
        const struct mliMat b,
        const double margin);
void mliMat_print(const struct mliMat rot);
struct mliMat mliMat_init_axis(
        const struct mliVec rot_axis,
        const double rot_angle);
struct mliMat mliMat_init_tait_bryan(
        const double rx,
        const double ry,
        const double rz);
#endif
