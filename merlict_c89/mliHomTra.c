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
        const struct mliRotMat *rotation,
        const struct mliVec in)
{
        struct mliVec out;
        out.x = in.x * rotation->r00 + in.y * rotation->r01 +
                in.z * rotation->r02;
        out.y = in.x * rotation->r10 + in.y * rotation->r11 +
                in.z * rotation->r12;
        out.z = in.x * rotation->r20 + in.y * rotation->r21 +
                in.z * rotation->r22;
        return out;
}

struct mliVec mli_transform_orientation_inverse(
        const struct mliRotMat *rotation,
        const struct mliVec in)
{
        struct mliVec out;
        out.x = in.x * rotation->r00 + in.y * rotation->r10 +
                in.z * rotation->r20;
        out.y = in.x * rotation->r01 + in.y * rotation->r11 +
                in.z * rotation->r21;
        out.z = in.x * rotation->r02 + in.y * rotation->r12 +
                in.z * rotation->r22;
        return out;
}

struct mliVec mli_transform_position(
        const struct mliRotMat *rotation,
        const struct mliVec translation,
        const struct mliVec in)
{
        struct mliVec out;
        out.x = in.x * rotation->r00 + in.y * rotation->r01 +
                in.z * rotation->r02 + translation.x;
        out.y = in.x * rotation->r10 + in.y * rotation->r11 +
                in.z * rotation->r12 + translation.y;
        out.z = in.x * rotation->r20 + in.y * rotation->r21 +
                in.z * rotation->r22 + translation.z;
        return out;
}

struct mliVec mli_transform_position_inverse(
        const struct mliRotMat *rotation,
        const struct mliVec translation,
        const struct mliVec in)
{
        struct mliVec out;
        out.x = in.x * rotation->r00 + in.y * rotation->r10 +
                in.z * rotation->r20 -
                (rotation->r00 * translation.x + rotation->r10 * translation.y +
                 rotation->r20 * translation.z);
        out.y = in.x * rotation->r01 + in.y * rotation->r11 +
                in.z * rotation->r21 -
                (rotation->r01 * translation.x + rotation->r11 * translation.y +
                 rotation->r21 * translation.z);
        out.z = in.x * rotation->r02 + in.y * rotation->r12 +
                in.z * rotation->r22 -
                (rotation->r02 * translation.x + rotation->r12 * translation.y +
                 rotation->r22 * translation.z);
        return out;
}

struct mliRay mli_transform_ray(
        const struct mliRotMat *rotation,
        const struct mliVec translation,
        const struct mliRay in)
{
        struct mliRay out;
        out.support = mli_transform_position(rotation, translation, in.support);
        out.direction = mli_transform_orientation(rotation, in.direction);
        return out;
}

struct mliRay mli_transform_ray_inverse(
        const struct mliRotMat *rotation,
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

int mliHomTraComp_is_equal(
        const struct mliHomTraComp a,
        const struct mliHomTraComp b)
{
        if (!mliVec_is_equal(a.translation, b.translation))
                return 0;
        if (!mliQuaternion_is_equal(a.rotation, b.rotation))
                return 0;
        return 1;
}

struct mliHomTraComp mliHomTraComp_sequence(
        const struct mliHomTraComp a,
        const struct mliHomTraComp b)
{
        struct mliHomTraComp s;
        struct mliRotMat rot_a = mliQuaternion_to_matrix(a.rotation);
        s.translation = mli_transform_orientation(&rot_a, a.translation);
        s.translation = mliVec_add(s.translation, b.translation);
        s.rotation = mliQuaternion_product(a.rotation, b.rotation);
        return s;
}
