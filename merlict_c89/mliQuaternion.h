/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MERLICT_C89_MLIQUATERNION_H_
#define MERLICT_C89_MLIQUATERNION_H_

#include <math.h>
#include <float.h>
#include <stdint.h>

#include "mliVec.h"

struct mliQuaternion {
        double w;
        double x;
        double y;
        double z;
};

void mliQuaternion_print(const struct mliQuaternion q);
struct mliQuaternion mliQuaternion_set_tait_bryan(
        const double rx,
        const double ry,
        const double rz);
struct mliRotMat mliQuaternion_to_matrix(const struct mliQuaternion quat);
struct mliQuaternion mliQuaternion_set_rotaxis_and_angle(
        const struct mliVec rot_axis,
        const double angle);
double mliQuaternion_norm(const struct mliQuaternion q);
double mliQuaternion_product_complex_conjugate(const struct mliQuaternion p);
struct mliQuaternion mliQuaternion_product(
        const struct mliQuaternion p,
        const struct mliQuaternion q);
struct mliQuaternion mliQuaternion_complex_conjugate(
        const struct mliQuaternion q);
int mliQuaternion_is_equal_margin(
        const struct mliQuaternion a,
        const struct mliQuaternion b,
        const double margin);
int mliQuaternion_is_equal(
        const struct mliQuaternion a,
        const struct mliQuaternion b);
struct mliQuaternion mliQuaternion_set(
        const double w,
        const double x,
        const double y,
        const double z);
#endif
