/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLI_QUATERNION_H_
#define MLI_QUATERNION_H_

#include "../vec/vec.h"
#include "../mat/mat.h"

struct mli_Quaternion {
        double w;
        double x;
        double y;
        double z;
};

void mli_Quaternion_print(const struct mli_Quaternion q);
struct mli_Quaternion mli_Quaternion_set_tait_bryan(
        const double rx,
        const double ry,
        const double rz);
struct mli_Mat mli_Quaternion_to_matrix(const struct mli_Quaternion quat);
struct mli_Quaternion mli_Quaternion_set_rotaxis_and_angle(
        const struct mli_Vec rot_axis,
        const double angle);
double mli_Quaternion_norm(const struct mli_Quaternion q);
double mli_Quaternion_product_complex_conjugate(const struct mli_Quaternion p);
struct mli_Quaternion mli_Quaternion_product(
        const struct mli_Quaternion p,
        const struct mli_Quaternion q);
struct mli_Quaternion mli_Quaternion_complex_conjugate(
        const struct mli_Quaternion q);
int mli_Quaternion_equal_margin(
        const struct mli_Quaternion a,
        const struct mli_Quaternion b,
        const double margin);
int mli_Quaternion_equal(
        const struct mli_Quaternion a,
        const struct mli_Quaternion b);
struct mli_Quaternion mli_Quaternion_set(
        const double w,
        const double x,
        const double y,
        const double z);
struct mli_Quaternion mli_Quaternion_set_unit_xyz(
        const double x,
        const double y,
        const double z);
#endif
