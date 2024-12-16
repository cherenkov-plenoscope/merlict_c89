/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "mat.h"
#include <math.h>
#include <assert.h>
#include "../chk/chk.h"

void mli_Mat_set(struct mli_Mat *a, uint64_t col, uint64_t row, const double v)
{
        switch (col) {
        case 0:
                switch (row) {
                case 0:
                        a->r00 = v;
                        break;
                case 1:
                        a->r01 = v;
                        break;
                case 2:
                        a->r02 = v;
                        break;
                default:
                        assert(0);
                        break;
                }
                break;
        case 1:
                switch (row) {
                case 0:
                        a->r10 = v;
                        break;
                case 1:
                        a->r11 = v;
                        break;
                case 2:
                        a->r12 = v;
                        break;
                default:
                        assert(0);
                        break;
                }
                break;
        case 2:
                switch (row) {
                case 0:
                        a->r20 = v;
                        break;
                case 1:
                        a->r21 = v;
                        break;
                case 2:
                        a->r22 = v;
                        break;
                default:
                        assert(0);
                        break;
                }
                break;
        default:
                assert(0);
                break;
        }
}

double mli_Mat_get(const struct mli_Mat *a, uint64_t col, uint64_t row)
{
        double o = 0;
        switch (col) {
        case 0:
                switch (row) {
                case 0:
                        o = a->r00;
                        break;
                case 1:
                        o = a->r01;
                        break;
                case 2:
                        o = a->r02;
                        break;
                default:
                        assert(0);
                        break;
                }
                break;
        case 1:
                switch (row) {
                case 0:
                        o = a->r10;
                        break;
                case 1:
                        o = a->r11;
                        break;
                case 2:
                        o = a->r12;
                        break;
                default:
                        assert(0);
                        break;
                }
                break;
        case 2:
                switch (row) {
                case 0:
                        o = a->r20;
                        break;
                case 1:
                        o = a->r21;
                        break;
                case 2:
                        o = a->r22;
                        break;
                default:
                        assert(0);
                        break;
                }
                break;
        default:
                assert(0);
                break;
        }
        return o;
}

struct mli_Mat mli_Mat_unity(void)
{
        struct mli_Mat u;
        u.r00 = 1.0;
        u.r01 = 0.0;
        u.r02 = 0.0;
        u.r10 = 0.0;
        u.r11 = 1.0;
        u.r12 = 0.0;
        u.r20 = 0.0;
        u.r21 = 0.0;
        u.r22 = 1.0;
        return u;
}

int mli_Mat_equal_margin(
        const struct mli_Mat a,
        const struct mli_Mat b,
        const double margin)
{
        int i, j;
        for (i = 0; i < 3; i++) {
                for (j = 0; j < 3; j++) {
                        const double diff = fabs(
                                mli_Mat_get(&a, i, j) - mli_Mat_get(&b, i, j));
                        if (diff > margin) {
                                return 0;
                        }
                }
        }
        return 1;
}

struct mli_Mat mli_Mat_init_tait_bryan(
        const double rx,
        const double ry,
        const double rz)
{
        struct mli_Mat rot;
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

struct mli_Mat mli_Mat_init_axis_angle(
        const struct mli_Vec axis,
        const double angle)
{
        struct mli_Mat rot;
        const double norm = mli_Vec_norm(axis);
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

struct mli_Mat mli_Mat_init_columns(
        const struct mli_Vec c0,
        const struct mli_Vec c1,
        const struct mli_Vec c2)
{
        struct mli_Mat m;
        m.r00 = c0.x;
        m.r01 = c1.x;
        m.r02 = c2.x;
        m.r10 = c0.y;
        m.r11 = c1.y;
        m.r12 = c2.y;
        m.r20 = c0.z;
        m.r21 = c1.z;
        m.r22 = c2.z;
        return m;
}

struct mli_Mat mli_Mat_covariance(
        const struct mli_Vec *vecs,
        const uint64_t num_vecs,
        const struct mli_Vec vecs_mean)
{
        /*
        Estimates the is the 'sample covariance'.

        Parameters
        ----------
        vecs
                The sample of vectors to estimate the covariance of.
        num_vecs
                Number of vectors in sample.
        vecs_mean
                The mean of the vectors.
        */
        uint64_t i;

        double sum_xx = 0.0;
        double sum_xy = 0.0;
        double sum_xz = 0.0;

        double sum_yx = 0.0;
        double sum_yy = 0.0;
        double sum_yz = 0.0;

        double sum_zx = 0.0;
        double sum_zy = 0.0;
        double sum_zz = 0.0;

        double sum_x = 0.0;
        double sum_y = 0.0;
        double sum_z = 0.0;

        double inum = 1.0 / (((double)num_vecs) - 1.0);
        struct mli_Vec v;
        struct mli_Mat cov;

        for (i = 0; i < num_vecs; i++) {
                v = mli_Vec_substract(vecs[i], vecs_mean);
                sum_xx += v.x * v.x;
                sum_xy += v.x * v.y;
                sum_xz += v.x * v.z;

                /* sum_yx += v.y * v.x; */
                sum_yy += v.y * v.y;
                sum_yz += v.y * v.z;

                /* sum_zx += v.z * v.x; */
                /* sum_zy += v.z * v.y; */
                sum_zz += v.z * v.z;

                sum_x += v.x;
                sum_y += v.y;
                sum_z += v.z;
        }

        sum_yx = sum_xy;
        sum_zx = sum_xz;
        sum_zy = sum_yz;

        cov.r00 = (sum_xx * inum) - (sum_x * inum) * (sum_x * inum);
        cov.r01 = (sum_xy * inum) - (sum_x * inum) * (sum_y * inum);
        cov.r02 = (sum_xz * inum) - (sum_x * inum) * (sum_z * inum);
        cov.r10 = (sum_yx * inum) - (sum_y * inum) * (sum_x * inum);
        cov.r11 = (sum_yy * inum) - (sum_y * inum) * (sum_y * inum);
        cov.r12 = (sum_yz * inum) - (sum_y * inum) * (sum_z * inum);
        cov.r20 = (sum_zx * inum) - (sum_z * inum) * (sum_x * inum);
        cov.r21 = (sum_zy * inum) - (sum_z * inum) * (sum_y * inum);
        cov.r22 = (sum_zz * inum) - (sum_z * inum) * (sum_z * inum);
        return cov;
}

struct mli_Mat mli_Mat_transpose(const struct mli_Mat m)
{
        struct mli_Mat t;
        t.r00 = m.r00;
        t.r01 = m.r10;
        t.r02 = m.r20;

        t.r10 = m.r01;
        t.r11 = m.r11;
        t.r12 = m.r21;

        t.r20 = m.r02;
        t.r21 = m.r12;
        t.r22 = m.r22;
        return t;
}

struct mli_Mat mli_Mat_multiply(const struct mli_Mat x, const struct mli_Mat y)
{
        struct mli_Mat p;
        p.r00 = x.r00 * y.r00 + x.r01 * y.r10 + x.r02 * y.r20;
        p.r10 = x.r10 * y.r00 + x.r11 * y.r10 + x.r12 * y.r20;
        p.r20 = x.r20 * y.r00 + x.r21 * y.r10 + x.r22 * y.r20;
        p.r01 = x.r00 * y.r01 + x.r01 * y.r11 + x.r02 * y.r21;
        p.r11 = x.r10 * y.r01 + x.r11 * y.r11 + x.r12 * y.r21;
        p.r21 = x.r20 * y.r01 + x.r21 * y.r11 + x.r22 * y.r21;
        p.r02 = x.r00 * y.r02 + x.r01 * y.r12 + x.r02 * y.r22;
        p.r12 = x.r10 * y.r02 + x.r11 * y.r12 + x.r12 * y.r22;
        p.r22 = x.r20 * y.r02 + x.r21 * y.r12 + x.r22 * y.r22;
        return p;
}

struct mli_Mat mli_Mat_minor(const struct mli_Mat x, const int d)
{
        struct mli_Mat m = x;
        switch (d) {
        case 0:
                break;
        case 1:
                m.r00 = 1.0;

                m.r01 = 0.0;
                m.r02 = 0.0;

                m.r10 = 0.0;
                m.r20 = 0.0;
                break;
        case 2:
                m.r00 = 1.0;
                m.r11 = 1.0;

                m.r01 = 0.0;
                m.r02 = 0.0;

                m.r10 = 0.0;
                m.r20 = 0.0;

                m.r12 = 0.0;
                m.r21 = 0.0;
                break;
        default:
                assert(0);
                break;
        }
        return m;
}

struct mli_Mat mli_Mat_vector_outer_product(const struct mli_Vec v)
{
        struct mli_Mat x;
        x.r00 = -2.0 * v.x * v.x + 1;
        x.r01 = -2.0 * v.x * v.y;
        x.r02 = -2.0 * v.x * v.z;
        x.r10 = -2.0 * v.y * v.x;
        x.r11 = -2.0 * v.y * v.y + 1;
        x.r12 = -2.0 * v.y * v.z;
        x.r20 = -2.0 * v.z * v.x;
        x.r21 = -2.0 * v.z * v.y;
        x.r22 = -2.0 * v.z * v.z + 1;
        return x;
}

void mli_Mat_qr_decompose(
        const struct mli_Mat m,
        struct mli_Mat *q,
        struct mli_Mat *r)
{
        /* housholder */
        /* ========== */

        struct mli_Mat q0, q1;
        struct mli_Mat z = m;

        struct mli_Vec e;
        struct mli_Vec x;
        double xnorm;

        /* column 0 */
        /* -------- */
        z = mli_Mat_minor(z, 0);
        x = mli_Vec_init(z.r00, z.r10, z.r20);
        xnorm = mli_Vec_norm(x);
        if (m.r00 > 0) {
                xnorm = -xnorm;
        };

        e = mli_Vec_init(xnorm, 0.0, 0.0);
        e = mli_Vec_add(x, e);
        e = mli_Vec_normalized(e);

        q0 = mli_Mat_vector_outer_product(e);
        z = mli_Mat_multiply(q0, z);

        /* column 1 */
        /* -------- */
        z = mli_Mat_minor(z, 1);
        x = mli_Vec_init(z.r01, z.r11, z.r21);
        xnorm = mli_Vec_norm(x);
        if (m.r11 > 0) {
                xnorm = -xnorm;
        };

        e = mli_Vec_init(0.0, xnorm, 0.0);
        e = mli_Vec_add(x, e);
        e = mli_Vec_normalized(e);

        q1 = mli_Mat_vector_outer_product(e);
        z = mli_Mat_multiply(q1, z);

        /* finalize */
        /* ======== */
        (*q) = q0;
        (*r) = mli_Mat_multiply(q0, m);

        (*q) = mli_Mat_multiply(q1, *q);

        (*r) = mli_Mat_multiply(*q, m);
        (*q) = mli_Mat_transpose(*q);
}

int mli_Mat_has_shurform(const struct mli_Mat m, const double margin)
{
        if (fabs(m.r10) > margin)
                return 0;
        if (fabs(m.r20) > margin)
                return 0;
        if (fabs(m.r21) > margin)
                return 0;
        return 1;
}

void mli_Mat_find_eigenvalues(
        const struct mli_Mat a,
        double *e0,
        double *e1,
        double *e2,
        const double margin,
        const uint64_t max_num_iterations)
{
        struct mli_Mat ak = a;
        struct mli_Mat qq = mli_Mat_unity();
        uint64_t k = 0;

        while (k < max_num_iterations) {
                struct mli_Mat q, r;
                k += 1;
                mli_Mat_qr_decompose(ak, &q, &r);
                ak = mli_Mat_multiply(r, q);
                qq = mli_Mat_multiply(qq, q);

                if (mli_Mat_has_shurform(ak, margin)) {
                        break;
                }
        }
        (*e0) = ak.r00;
        (*e1) = ak.r11;
        (*e2) = ak.r22;
}

int mli_Mat_find_eigenvector_for_eigenvalue(
        struct mli_Mat A,
        const double eigen_value,
        struct mli_Vec *eigen_vector,
        const double tolerance)
{
        int pivots[4] = {0, 0, 0, 0};
        struct mli_Vec right_hand_side = {1.0, 1.0, 1.0};

        chk_msg(tolerance > 0.0, "Expected tolerance > 0.0");

        A.r00 -= eigen_value;
        A.r11 -= eigen_value;
        A.r22 -= eigen_value;

        chk_msg(mli_Mat_lup_decompose(&A, pivots, tolerance),
                "Can not decompose LU-matices");
        mli_Mat_lup_solve(&A, pivots, &right_hand_side, eigen_vector);
        (*eigen_vector) = mli_Vec_normalized(*eigen_vector);

        return 1;
chk_error:
        return 0;
}

int mli_Mat_lup_decompose(struct mli_Mat *A, int *P, const double tolerance)
{
        /*
        Parameters
        ----------
        A
                Matrix to decompose.
        tolerance
                small tolerance number to detect failure when the
                matrix is near degenerate
        Output
        ------
        A
                Matrix A is changed, it contains a copy of both matrices
                L-E and U as A=(L-E)+U such that P*A=L*U.
        P : int[N+1]
                The permutation matrix is not stored as a matrix,
                but in an integer vector P of size N+1 containing column
                indexes where the permutation matrix has "1".
                The last element P[N]=S+N,
                where S is the number of row exchanges needed for determinant
                computation, det(P)=(-1)^S

        Inspired by https://en.wikipedia.org/wiki/LU_decomposition
        */
        int i, j, k, imax;
        double maxA, absA;

        for (i = 0; i <= 3; i++) {
                /* Unit permutation matrix, P[N] initialized with N */
                P[i] = i;
        }

        for (i = 0; i < 3; i++) {
                maxA = 0.0;
                imax = i;

                for (k = i; k < 3; k++) {
                        absA = fabs(mli_Mat_get(A, k, i));
                        if (absA > maxA) {
                                maxA = absA;
                                imax = k;
                        }
                }

                if (maxA < tolerance) {
                        return 0; /* failure, matrix is degenerate */
                }

                if (imax != i) {
                        /* pivoting */
                        double tmp[3];
                        j = P[i];
                        P[i] = P[imax];
                        P[imax] = j;

                        /* pivoting rows of A */
                        tmp[0] = mli_Mat_get(A, i, 0);
                        tmp[1] = mli_Mat_get(A, i, 1);
                        tmp[2] = mli_Mat_get(A, i, 2);

                        mli_Mat_set(A, i, 0, mli_Mat_get(A, imax, 0));
                        mli_Mat_set(A, i, 1, mli_Mat_get(A, imax, 1));
                        mli_Mat_set(A, i, 2, mli_Mat_get(A, imax, 2));

                        mli_Mat_set(A, imax, 0, tmp[0]);
                        mli_Mat_set(A, imax, 1, tmp[1]);
                        mli_Mat_set(A, imax, 2, tmp[2]);

                        /* counting pivots starting from 3 (for determinant) */
                        P[3]++;
                }

                for (j = i + 1; j < 3; j++) {
                        double tmp =
                                mli_Mat_get(A, j, i) / mli_Mat_get(A, i, i);
                        mli_Mat_set(A, j, i, tmp);

                        for (k = i + 1; k < 3; k++) {
                                double tmp =
                                        (mli_Mat_get(A, j, k) -
                                         mli_Mat_get(A, j, i) *
                                                 mli_Mat_get(A, i, k));
                                mli_Mat_set(A, j, k, tmp);
                        }
                }
        }

        return 1; /* decomposition done */
}

void mli_Mat_lup_solve(
        const struct mli_Mat *A,
        const int *P,
        const struct mli_Vec *b,
        struct mli_Vec *x)
{
        /*
        Solve linear equation A*x = b for x.

        Parameters
        ----------
        A and P
                Filled in mli_Mat_lup_decompose.
        b
                Right hand side vector b in A*x = b.

        Output
        ------
        x
                The solution vector x of A*x = b
        */
        const uint64_t N = 3;
        uint64_t i, k, idown;
        for (i = 0; i < N; i++) {
                mli_Vec_set(x, i, mli_Vec_get(b, P[i]));
                for (k = 0; k < i; k++) {
                        const double tmp =
                                (mli_Vec_get(x, i) -
                                 mli_Mat_get(A, i, k) * mli_Vec_get(x, k));
                        mli_Vec_set(x, i, tmp);
                }
        }

        i = N - 1;
        for (idown = 0; idown < N; idown++) {
                double tmp;
                for (k = i + 1; k < N; k++) {
                        const double tmp =
                                (mli_Vec_get(x, i) -
                                 mli_Mat_get(A, i, k) * mli_Vec_get(x, k));
                        mli_Vec_set(x, i, tmp);
                }
                tmp = mli_Vec_get(x, i) / mli_Mat_get(A, i, i);
                mli_Vec_set(x, i, tmp);
                i--;
        }
}
