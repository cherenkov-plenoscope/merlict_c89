/* Copyright 2019 Sebastian Achim Mueller */
#ifndef MERLICT_MLIHOMTRA_H_
#define MERLICT_MLIHOMTRA_H_

#include <math.h>
#include <stdint.h>
#include "mliVec.h"
#include "mliRotMat.h"
#include "mliQuaternion.h"

struct mliHomTraComp {
        struct mliVec trans;
        struct mliQuaternion rot;
};

struct mliHomTra {
        struct mliVec trans;
        struct mliRotMat rot;
};

struct mliHomTra mliHomTra_from_compact(const struct mliHomTraComp trafo)
{
        struct mliHomTra t;
        t.trans = trafo.trans;
        t.rot = mliQuaternion_to_matrix(trafo.rot);
        return t;
}

struct mliVec mli_transform_orientation(
        const struct mliRotMat *rot,
        const struct mliVec in)
{
        struct mliVec out;
        out.x = in.x * rot->r00 + in.y * rot->r01 + in.z * rot->r02;
        out.y = in.x * rot->r10 + in.y * rot->r11 + in.z * rot->r12;
        out.z = in.x * rot->r20 + in.y * rot->r21 + in.z * rot->r22;
        return out;
}

struct mliVec mli_transform_orientation_inverse(
        const struct mliRotMat *rot,
        const struct mliVec in)
{
        struct mliVec out;
        out.x = in.x * rot->r00 + in.y * rot->r10 + in.z * rot->r20;
        out.y = in.x * rot->r01 + in.y * rot->r11 + in.z * rot->r21;
        out.z = in.x * rot->r02 + in.y * rot->r12 + in.z * rot->r22;
        return out;
}

struct mliVec mli_transform_position(
        const struct mliRotMat *rot,
        const struct mliVec trans,
        const struct mliVec in)
{
        struct mliVec out;
        out.x = in.x * rot->r00 + in.y * rot->r01 + in.z * rot->r02 + trans.x;
        out.y = in.x * rot->r10 + in.y * rot->r11 + in.z * rot->r12 + trans.y;
        out.z = in.x * rot->r20 + in.y * rot->r21 + in.z * rot->r22 + trans.z;
        return out;
}

struct mliVec mli_transform_position_inverse(
        const struct mliRotMat *rot,
        const struct mliVec trans,
        const struct mliVec in)
{
        struct mliVec out;
        out.x = in.x * rot->r00 + in.y * rot->r10 + in.z * rot->r20 -
                (rot->r00 * trans.x + rot->r10 * trans.y + rot->r20 * trans.z);
        out.y = in.x * rot->r01 + in.y * rot->r11 + in.z * rot->r21 -
                (rot->r01 * trans.x + rot->r11 * trans.y + rot->r21 * trans.z);
        out.z = in.x * rot->r02 + in.y * rot->r12 + in.z * rot->r22 -
                (rot->r02 * trans.x + rot->r12 * trans.y + rot->r22 * trans.z);
        return out;
}

struct mliRay mli_transform_ray(
        const struct mliRotMat *rot,
        const struct mliVec trans,
        const struct mliRay in)
{
        struct mliRay out;
        out.support = mli_transform_position(rot, trans, in.support);
        out.direction = mli_transform_orientation(rot, in.direction);
        return out;
}

struct mliRay mli_transform_ray_inverse(
        const struct mliRotMat *rot,
        const struct mliVec trans,
        const struct mliRay in)
{
        struct mliRay out;
        out.support = mli_transform_position_inverse(rot, trans, in.support);
        out.direction = mli_transform_orientation_inverse(rot, in.direction);
        return out;
}

struct mliRay mliHomTra_ray(const struct mliHomTra *t, const struct mliRay in)
{
        return mli_transform_ray(&t->rot, t->trans, in);
}

struct mliRay mliHomTra_ray_inverse(
        const struct mliHomTra *t,
        const struct mliRay in)
{
        return mli_transform_ray_inverse(&t->rot, t->trans, in);
}

struct mliVec mliHomTra_pos(const struct mliHomTra *t, const struct mliVec in)
{
        return mli_transform_position(&t->rot, t->trans, in);
}

struct mliVec mliHomTra_pos_inverse(
        const struct mliHomTra *t,
        const struct mliVec in)
{
        return mli_transform_position_inverse(&t->rot, t->trans, in);
}

struct mliVec mliHomTra_dir(const struct mliHomTra *t, const struct mliVec in)
{
        return mli_transform_orientation(&t->rot, in);
}

struct mliVec mliHomTra_dir_inverse(
        const struct mliHomTra *t,
        const struct mliVec in)
{
        return mli_transform_orientation_inverse(&t->rot, in);
}

int mliHomTraComp_is_equal(
        const struct mliHomTraComp a,
        const struct mliHomTraComp b)
{
        if (!mliVec_is_equal(a.trans, b.trans))
                return 0;
        if (!mliQuaternion_is_equal(a.rot, b.rot))
                return 0;
        return 1;
}

struct mliHomTraComp mliHomTraComp_sequence(
        const struct mliHomTraComp a,
        const struct mliHomTraComp b)
{
        struct mliHomTraComp s;
        struct mliRotMat rot_a = mliQuaternion_to_matrix(a.rot);
        s.trans = mli_transform_orientation(&rot_a, a.trans);
        s.trans = mliVec_add(s.trans, b.trans);
        s.rot = mliQuaternion_product(a.rot, b.rot);
        return s;
}

void mliHomTra_print(const struct mliHomTra h)
{
        printf("[%.1f, %.1f, %.1f | %.1f]\n",
               h.rot.r00,
               h.rot.r01,
               h.rot.r02,
               h.trans.x);
        printf("[%.1f, %.1f, %.1f | %.1f]\n",
               h.rot.r10,
               h.rot.r11,
               h.rot.r12,
               h.trans.y);
        printf("[%.1f, %.1f, %.1f | %.1f]\n",
               h.rot.r20,
               h.rot.r21,
               h.rot.r22,
               h.trans.z);
        printf("[  0,   0,   0 |   1]\n");
}

#endif
