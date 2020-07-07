/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MERLICT_C89_MLIROTMAT_H_
#define MERLICT_C89_MLIROTMAT_H_

#include <math.h>
#include <stdint.h>

#include "mliVec.h"

struct mliRotMat {
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

int mliRotMat_equal_margin(
        const struct mliRotMat a,
        const struct mliRotMat b,
        const double margin);
void mliRotMat_print(const struct mliRotMat rot);
struct mliRotMat mliRotMat_init_axis(
        const struct mliVec rot_axis,
        const double rot_angle);
struct mliRotMat mliRotMat_init_tait_bryan(
        const double rx,
        const double ry,
        const double rz);
#endif
