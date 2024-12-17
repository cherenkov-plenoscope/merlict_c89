/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "homtra.h"

struct mli_HomTraComp mli_HomTraComp_set(
        const struct mli_Vec translation,
        const struct mli_Quaternion rotation)
{
        struct mli_HomTraComp comp;
        comp.translation = translation;
        comp.rotation = rotation;
        return comp;
}

struct mli_HomTra mli_HomTraComp_from_compact(const struct mli_HomTraComp trafo)
{
        struct mli_HomTra t;
        t.translation = trafo.translation;
        t.rotation = mli_Quaternion_to_matrix(trafo.rotation);
        return t;
}

struct mli_Vec mli_transform_orientation(
        const struct mli_Mat *rotation,
        const struct mli_Vec ori)
{
        struct mli_Vec out;
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

struct mli_Vec mli_transform_orientation_inverse(
        const struct mli_Mat *rotation,
        const struct mli_Vec ori)
{
        struct mli_Vec out;
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

struct mli_Vec mli_transform_position(
        const struct mli_Mat *rotation,
        const struct mli_Vec translation,
        const struct mli_Vec pos)
{
        struct mli_Vec out;
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

struct mli_Vec mli_transform_position_inverse(
        const struct mli_Mat *rotation,
        const struct mli_Vec translation,
        const struct mli_Vec pos)
{
        struct mli_Vec out;
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
        const struct mli_Mat *rotation,
        const struct mli_Vec translation,
        const struct mliRay in)
{
        struct mliRay out;
        out.support = mli_transform_position(rotation, translation, in.support);
        out.direction = mli_transform_orientation(rotation, in.direction);
        return out;
}

struct mliRay mli_transform_ray_inverse(
        const struct mli_Mat *rotation,
        const struct mli_Vec translation,
        const struct mliRay in)
{
        struct mliRay out;
        out.support = mli_transform_position_inverse(
                rotation, translation, in.support);
        out.direction =
                mli_transform_orientation_inverse(rotation, in.direction);
        return out;
}

struct mliRay mli_HomTraComp_ray(
        const struct mli_HomTra *t,
        const struct mliRay in)
{
        return mli_transform_ray(&t->rotation, t->translation, in);
}

struct mliRay mli_HomTraComp_ray_inverse(
        const struct mli_HomTra *t,
        const struct mliRay in)
{
        return mli_transform_ray_inverse(&t->rotation, t->translation, in);
}

struct mli_Vec mli_HomTraComp_pos(
        const struct mli_HomTra *t,
        const struct mli_Vec in)
{
        return mli_transform_position(&t->rotation, t->translation, in);
}

struct mli_Vec mli_HomTraComp_pos_inverse(
        const struct mli_HomTra *t,
        const struct mli_Vec in)
{
        return mli_transform_position_inverse(&t->rotation, t->translation, in);
}

struct mli_Vec mli_HomTraComp_dir(
        const struct mli_HomTra *t,
        const struct mli_Vec in)
{
        return mli_transform_orientation(&t->rotation, in);
}

struct mli_Vec mli_HomTraComp_dir_inverse(
        const struct mli_HomTra *t,
        const struct mli_Vec in)
{
        return mli_transform_orientation_inverse(&t->rotation, in);
}

int mli_HomTraComp_equal(
        const struct mli_HomTraComp a,
        const struct mli_HomTraComp b)
{
        if (!mli_Vec_equal(a.translation, b.translation))
                return 0;
        if (!mli_Quaternion_equal(a.rotation, b.rotation))
                return 0;
        return 1;
}

struct mli_HomTraComp mli_HomTraComp_sequence(
        const struct mli_HomTraComp a,
        const struct mli_HomTraComp b)
{
        struct mli_HomTra b_;
        struct mli_HomTraComp s;
        b_ = mli_HomTraComp_from_compact(b);
        s.translation = mli_HomTraComp_pos(&b_, a.translation);
        s.rotation = mli_Quaternion_product(b.rotation, a.rotation);
        return s;
}
