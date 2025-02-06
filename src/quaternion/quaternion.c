/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "quaternion.h"
#include <math.h>
#include <float.h>

struct mli_Quaternion mli_Quaternion_set(
        const double w,
        const double x,
        const double y,
        const double z)
{
        struct mli_Quaternion o;
        o.w = w;
        o.x = x;
        o.y = y;
        o.z = z;
        return o;
}

struct mli_Quaternion mli_Quaternion_set_unit_xyz(
        const double x,
        const double y,
        const double z)
{
        /*
         *       Recover 4th element: q.w.
         *       Expect unit-quaternion:
         *       1.0 != q.w**2 + q.x**2 + q.y**2 + q.z**2
         *       thus:
         *       q.w**2 = 1.0 - q.x**2 - q.y**2 - q.z**2
         *       q.w = sqrt(1.0 - q.x**2 - q.y**2 - q.z**2)
         */
        const double w = sqrt(1.0 - x * x - y * y - z * z);
        return mli_Quaternion_set(w, x, y, z);
}

mli_bool mli_Quaternion_equal(
        const struct mli_Quaternion a,
        const struct mli_Quaternion b)
{
        if (fabs(a.w - b.w) > DBL_EPSILON)
                return MLI_FALSE;
        if (fabs(a.x - b.x) > DBL_EPSILON)
                return MLI_FALSE;
        if (fabs(a.y - b.y) > DBL_EPSILON)
                return MLI_FALSE;
        if (fabs(a.z - b.z) > DBL_EPSILON)
                return MLI_FALSE;
        return MLI_TRUE;
}

mli_bool mli_Quaternion_equal_margin(
        const struct mli_Quaternion a,
        const struct mli_Quaternion b,
        const double margin)
{
        if (fabs(a.w - b.w) >= margin) {
                return MLI_FALSE;
        }
        if (fabs(a.x - b.x) >= margin) {
                return MLI_FALSE;
        }
        if (fabs(a.y - b.y) >= margin) {
                return MLI_FALSE;
        }
        if (fabs(a.z - b.z) >= margin) {
                return MLI_FALSE;
        }
        return MLI_TRUE;
}

struct mli_Quaternion mli_Quaternion_complex_conjugate(
        const struct mli_Quaternion q)
{
        struct mli_Quaternion c;
        c.w = q.w;
        c.x = -q.x;
        c.y = -q.y;
        c.z = -q.z;
        return c;
}

struct mli_Quaternion mli_Quaternion_product(
        const struct mli_Quaternion p,
        const struct mli_Quaternion q)
{
        struct mli_Quaternion pq;
        const struct mli_Vec P = mli_Vec_init(p.x, p.y, p.z);
        const struct mli_Vec Q = mli_Vec_init(q.x, q.y, q.z);
        const struct mli_Vec P_cross_Q = mli_Vec_cross(P, Q);
        pq.w = p.w * q.w - mli_Vec_dot(P, Q);
        pq.x = p.w * Q.x + q.w * P.x + P_cross_Q.x;
        pq.y = p.w * Q.y + q.w * P.y + P_cross_Q.y;
        pq.z = p.w * Q.z + q.w * P.z + P_cross_Q.z;
        return pq;
}

double mli_Quaternion_product_complex_conjugate(const struct mli_Quaternion p)
{
        return p.w * p.w + p.x * p.x + p.y * p.y + p.z * p.z;
}

double mli_Quaternion_norm(const struct mli_Quaternion q)
{
        return sqrt(mli_Quaternion_product_complex_conjugate(q));
}

struct mli_Quaternion mli_Quaternion_set_rotaxis_and_angle(
        const struct mli_Vec rot_axis,
        const double angle)
{
        const struct mli_Vec normed_rot_axis = mli_Vec_normalized(rot_axis);
        struct mli_Quaternion quat;
        const double angle_half = .5 * angle;
        const double sin_angle_half = sin(angle_half);
        quat.w = cos(angle_half);
        quat.x = normed_rot_axis.x * sin_angle_half;
        quat.y = normed_rot_axis.y * sin_angle_half;
        quat.z = normed_rot_axis.z * sin_angle_half;
        return quat;
}

struct mli_Mat mli_Quaternion_to_matrix(const struct mli_Quaternion quat)
{
        struct mli_Mat o;
        const double w2 = quat.w * quat.w;
        const double x2 = quat.x * quat.x;
        const double y2 = quat.y * quat.y;
        const double z2 = quat.z * quat.z;
        const double _2wx = 2. * quat.w * quat.x;
        const double _2wy = 2. * quat.w * quat.y;
        const double _2wz = 2. * quat.w * quat.z;
        /* const double 2xw */
        const double _2xy = 2. * quat.x * quat.y;
        const double _2xz = 2. * quat.x * quat.z;
        /* const double 2yw */
        /* const double 2yx */
        const double _2yz = 2. * quat.y * quat.z;
        o.r00 = w2 + x2 - y2 - z2;
        o.r01 = _2xy - _2wz;
        o.r02 = _2xz + _2wy;
        o.r10 = _2xy + _2wz;
        o.r11 = w2 - x2 + y2 - z2;
        o.r12 = _2yz - _2wx;
        o.r20 = _2xz - _2wy;
        o.r21 = _2yz + _2wx;
        o.r22 = w2 - x2 - y2 + z2;
        return o;
}

struct mli_Quaternion mli_Quaternion_set_tait_bryan(
        const double rx,
        const double ry,
        const double rz)
{
        const struct mli_Quaternion qz = mli_Quaternion_set_rotaxis_and_angle(
                mli_Vec_init(0, 0, 1), -rz);
        const struct mli_Quaternion qy = mli_Quaternion_set_rotaxis_and_angle(
                mli_Vec_init(0, 1, 0), -ry);
        const struct mli_Quaternion qx = mli_Quaternion_set_rotaxis_and_angle(
                mli_Vec_init(1, 0, 0), -rx);
        const struct mli_Quaternion qz_qy = mli_Quaternion_product(qz, qy);
        return mli_Quaternion_product(qz_qy, qx);
}
