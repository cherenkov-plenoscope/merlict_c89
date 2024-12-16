/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLIMAT_H_
#define MLIMAT_H_

#include <stdint.h>
#include "../vec/vec.h"

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

void mliMat_set(struct mliMat *a, uint64_t col, uint64_t row, const double v);
double mliMat_get(const struct mliMat *a, uint64_t col, uint64_t row);
struct mliMat mliMat_unity(void);
int mliMat_equal_margin(
        const struct mliMat a,
        const struct mliMat b,
        const double margin);
struct mliMat mliMat_init_axis_angle(
        const struct mli_Vec axis,
        const double angle);
struct mliMat mliMat_init_tait_bryan(
        const double rx,
        const double ry,
        const double rz);
struct mliMat mliMat_init_columns(
        const struct mli_Vec c0,
        const struct mli_Vec c1,
        const struct mli_Vec c2);
struct mliMat mliMat_covariance(
        const struct mli_Vec *vecs,
        const uint64_t num_vecs,
        const struct mli_Vec vecs_mean);
struct mliMat mliMat_transpose(const struct mliMat m);
struct mliMat mliMat_multiply(const struct mliMat x, const struct mliMat y);
struct mliMat mliMat_minor(const struct mliMat x, const int d);
struct mliMat mliMat_vector_outer_product(const struct mli_Vec v);
void mliMat_qr_decompose(
        const struct mliMat m,
        struct mliMat *q,
        struct mliMat *r);
int mliMat_has_shurform(const struct mliMat m, const double margin);
void mliMat_find_eigenvalues(
        const struct mliMat a,
        double *e0,
        double *e1,
        double *e2,
        const double margin,
        const uint64_t max_num_iterations);
int mliMat_find_eigenvector_for_eigenvalue(
        struct mliMat a,
        const double eigen_value,
        struct mli_Vec *eigen_vector,
        const double tolerance);
int mliMat_lup_decompose(struct mliMat *A, int *pivots, const double tolerance);
void mliMat_lup_solve(
        const struct mliMat *A,
        const int *P,
        const struct mli_Vec *b,
        struct mli_Vec *x);
#endif
