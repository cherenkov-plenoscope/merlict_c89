/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "mliHomTra.h"

struct mliHomTraComp mliHomTraComp_set(
        const struct mliVec translation,
        const struct mliQuaternion rotation)
{
        struct mliHomTraComp comp;
        comp.translation = translation;
        comp.rotation = rotation;
        return comp;
}

struct mliHomTra mliHomTra_from_compact(const struct mliHomTraComp trafo)
{
        struct mliHomTra t;
        t.translation = trafo.translation;
        t.rotation = mliQuaternion_to_matrix(trafo.rotation);
        return t;
}

struct mliVec mli_transform_orientation(
        const struct mliMat *rotation,
        const struct mliVec ori)
{
        struct mliVec out;
        out.x =
                (ori.x * rotation->r00 + ori.y * rotation->r01 +
                 ori.z * rotation->r02);
        out.y =
                (ori.x * rotation->r10 + ori.y * rotation->r11 +
                 ori.z * rotation->r12);
        out.z =
                (ori.x * rotation->r20 + ori.y * rotation->r21 +
                 ori.z * rotation->r22);
        return out;
}

struct mliVec mli_transform_orientation_inverse(
        const struct mliMat *rotation,
        const struct mliVec ori)
{
        struct mliVec out;
        out.x =
                (ori.x * rotation->r00 + ori.y * rotation->r10 +
                 ori.z * rotation->r20);
        out.y =
                (ori.x * rotation->r01 + ori.y * rotation->r11 +
                 ori.z * rotation->r21);
        out.z =
                (ori.x * rotation->r02 + ori.y * rotation->r12 +
                 ori.z * rotation->r22);
        return out;
}

struct mliVec mli_transform_position(
        const struct mliMat *rotation,
        const struct mliVec translation,
        const struct mliVec pos)
{
        struct mliVec out;
        out.x = (pos.x * rotation->r00 + pos.y * rotation->r01 +
                 pos.z * rotation->r02) +
                translation.x;
        out.y = (pos.x * rotation->r10 + pos.y * rotation->r11 +
                 pos.z * rotation->r12) +
                translation.y;
        out.z = (pos.x * rotation->r20 + pos.y * rotation->r21 +
                 pos.z * rotation->r22) +
                translation.z;
        return out;
}

struct mliVec mli_transform_position_inverse(
        const struct mliMat *rotation,
        const struct mliVec translation,
        const struct mliVec pos)
{
        struct mliVec out;
        out.x = (pos.x * rotation->r00 + pos.y * rotation->r10 +
                 pos.z * rotation->r20) -
                (rotation->r00 * translation.x + rotation->r10 * translation.y +
                 rotation->r20 * translation.z);
        out.y = (pos.x * rotation->r01 + pos.y * rotation->r11 +
                 pos.z * rotation->r21) -
                (rotation->r01 * translation.x + rotation->r11 * translation.y +
                 rotation->r21 * translation.z);
        out.z = (pos.x * rotation->r02 + pos.y * rotation->r12 +
                 pos.z * rotation->r22) -
                (rotation->r02 * translation.x + rotation->r12 * translation.y +
                 rotation->r22 * translation.z);
        return out;
}

struct mliRay mli_transform_ray(
        const struct mliMat *rotation,
        const struct mliVec translation,
        const struct mliRay in)
{
        struct mliRay out;
        out.support = mli_transform_position(rotation, translation, in.support);
        out.direction = mli_transform_orientation(rotation, in.direction);
        return out;
}

struct mliRay mli_transform_ray_inverse(
        const struct mliMat *rotation,
        const struct mliVec translation,
        const struct mliRay in)
{
        struct mliRay out;
        out.support = mli_transform_position_inverse(
                rotation, translation, in.support);
        out.direction =
                mli_transform_orientation_inverse(rotation, in.direction);
        return out;
}

struct mliRay mliHomTra_ray(const struct mliHomTra *t, const struct mliRay in)
{
        return mli_transform_ray(&t->rotation, t->translation, in);
}

struct mliRay mliHomTra_ray_inverse(
        const struct mliHomTra *t,
        const struct mliRay in)
{
        return mli_transform_ray_inverse(&t->rotation, t->translation, in);
}

struct mliVec mliHomTra_pos(const struct mliHomTra *t, const struct mliVec in)
{
        return mli_transform_position(&t->rotation, t->translation, in);
}

struct mliVec mliHomTra_pos_inverse(
        const struct mliHomTra *t,
        const struct mliVec in)
{
        return mli_transform_position_inverse(&t->rotation, t->translation, in);
}

struct mliVec mliHomTra_dir(const struct mliHomTra *t, const struct mliVec in)
{
        return mli_transform_orientation(&t->rotation, in);
}

struct mliVec mliHomTra_dir_inverse(
        const struct mliHomTra *t,
        const struct mliVec in)
{
        return mli_transform_orientation_inverse(&t->rotation, in);
}

int mliHomTraComp_equal(
        const struct mliHomTraComp a,
        const struct mliHomTraComp b)
{
        if (!mliVec_equal(a.translation, b.translation))
                return 0;
        if (!mliQuaternion_equal(a.rotation, b.rotation))
                return 0;
        return 1;
}

struct mliHomTraComp mliHomTraComp_sequence(
        const struct mliHomTraComp a,
        const struct mliHomTraComp b)
{
        struct mliHomTra b_;
        struct mliHomTraComp s;
        b_ = mliHomTra_from_compact(b);
        s.translation = mliHomTra_pos(&b_, a.translation);
        s.rotation = mliQuaternion_product(b.rotation, a.rotation);
        return s;
}
