/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLI_MAT_H_
#define MLI_MAT_H_

#include <stdint.h>
#include "../vec/vec.h"

struct mli_Mat {
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

void mli_Mat_set(struct mli_Mat *a, uint64_t col, uint64_t row, const double v);
double mli_Mat_get(const struct mli_Mat *a, uint64_t col, uint64_t row);
struct mli_Mat mli_Mat_unity(void);
int mli_Mat_equal_margin(
        const struct mli_Mat a,
        const struct mli_Mat b,
        const double margin);
struct mli_Mat mli_Mat_init_axis_angle(
        const struct mli_Vec axis,
        const double angle);
struct mli_Mat mli_Mat_init_tait_bryan(
        const double rx,
        const double ry,
        const double rz);
struct mli_Mat mli_Mat_init_columns(
        const struct mli_Vec c0,
        const struct mli_Vec c1,
        const struct mli_Vec c2);
struct mli_Mat mli_Mat_covariance(
        const struct mli_Vec *vecs,
        const uint64_t num_vecs,
        const struct mli_Vec vecs_mean);
struct mli_Mat mli_Mat_transpose(const struct mli_Mat m);
struct mli_Mat mli_Mat_multiply(const struct mli_Mat x, const struct mli_Mat y);
struct mli_Mat mli_Mat_minor(const struct mli_Mat x, const int d);
struct mli_Mat mli_Mat_vector_outer_product(const struct mli_Vec v);
void mli_Mat_qr_decompose(
        const struct mli_Mat m,
        struct mli_Mat *q,
        struct mli_Mat *r);
int mli_Mat_has_shurform(const struct mli_Mat m, const double margin);
void mli_Mat_find_eigenvalues(
        const struct mli_Mat a,
        double *e0,
        double *e1,
        double *e2,
        const double margin,
        const uint64_t max_num_iterations);
int mli_Mat_find_eigenvector_for_eigenvalue(
        struct mli_Mat a,
        const double eigen_value,
        struct mli_Vec *eigen_vector,
        const double tolerance);
int mli_Mat_lup_decompose(
        struct mli_Mat *A,
        int *pivots,
        const double tolerance);
void mli_Mat_lup_solve(
        const struct mli_Mat *A,
        const int *P,
        const struct mli_Vec *b,
        struct mli_Vec *x);
#endif
