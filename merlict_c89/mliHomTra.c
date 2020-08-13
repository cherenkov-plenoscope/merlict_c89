/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "mliHomTra.h"

struct mliHomTraComp mliHomTraComp_set(
        const struct mliVec translation,
        const struct mliQuaternion rot)
{
        struct mliHomTraComp comp;
        comp.translation = translation;
        comp.rot = rot;
        return comp;
}

struct mliHomTra mliHomTra_from_compact(const struct mliHomTraComp trafo)
{
        struct mliHomTra t;
        t.translation = trafo.translation;
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
        const struct mliVec translation,
        const struct mliVec in)
{
        struct mliVec out;
        out.x = in.x * rot->r00 + in.y * rot->r01 + in.z * rot->r02 + translation.x;
        out.y = in.x * rot->r10 + in.y * rot->r11 + in.z * rot->r12 + translation.y;
        out.z = in.x * rot->r20 + in.y * rot->r21 + in.z * rot->r22 + translation.z;
        return out;
}

struct mliVec mli_transform_position_inverse(
        const struct mliRotMat *rot,
        const struct mliVec translation,
        const struct mliVec in)
{
        struct mliVec out;
        out.x = in.x * rot->r00 + in.y * rot->r10 + in.z * rot->r20 -
                (rot->r00 * translation.x + rot->r10 * translation.y + rot->r20 * translation.z);
        out.y = in.x * rot->r01 + in.y * rot->r11 + in.z * rot->r21 -
                (rot->r01 * translation.x + rot->r11 * translation.y + rot->r21 * translation.z);
        out.z = in.x * rot->r02 + in.y * rot->r12 + in.z * rot->r22 -
                (rot->r02 * translation.x + rot->r12 * translation.y + rot->r22 * translation.z);
        return out;
}

struct mliRay mli_transform_ray(
        const struct mliRotMat *rot,
        const struct mliVec translation,
        const struct mliRay in)
{
        struct mliRay out;
        out.support = mli_transform_position(rot, translation, in.support);
        out.direction = mli_transform_orientation(rot, in.direction);
        return out;
}

struct mliRay mli_transform_ray_inverse(
        const struct mliRotMat *rot,
        const struct mliVec translation,
        const struct mliRay in)
{
        struct mliRay out;
        out.support = mli_transform_position_inverse(rot, translation, in.support);
        out.direction = mli_transform_orientation_inverse(rot, in.direction);
        return out;
}

struct mliRay mliHomTra_ray(const struct mliHomTra *t, const struct mliRay in)
{
        return mli_transform_ray(&t->rot, t->translation, in);
}

struct mliRay mliHomTra_ray_inverse(
        const struct mliHomTra *t,
        const struct mliRay in)
{
        return mli_transform_ray_inverse(&t->rot, t->translation, in);
}

struct mliVec mliHomTra_pos(const struct mliHomTra *t, const struct mliVec in)
{
        return mli_transform_position(&t->rot, t->translation, in);
}

struct mliVec mliHomTra_pos_inverse(
        const struct mliHomTra *t,
        const struct mliVec in)
{
        return mli_transform_position_inverse(&t->rot, t->translation, in);
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
        if (!mliVec_is_equal(a.translation, b.translation))
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
        s.translation = mli_transform_orientation(&rot_a, a.translation);
        s.translation = mliVec_add(s.translation, b.translation);
        s.rot = mliQuaternion_product(a.rot, b.rot);
        return s;
}
