/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLIHOMTRA_H_
#define MLIHOMTRA_H_

#include "../vec/vec.h"
#include "mliRay.h"
#include "mliMat.h"
#include "../quaternion/quaternion.h"

struct mliHomTraComp {
        struct mli_Vec translation;
        struct mli_Quaternion rotation;
};

struct mliHomTra {
        struct mli_Vec translation;
        struct mliMat rotation;
};

void mliHomTra_print(const struct mliHomTra h);
struct mliHomTraComp mliHomTraComp_set(
        const struct mli_Vec translation,
        const struct mli_Quaternion rotation);
struct mliHomTraComp mliHomTraComp_sequence(
        const struct mliHomTraComp a,
        const struct mliHomTraComp b);
int mliHomTraComp_equal(
        const struct mliHomTraComp a,
        const struct mliHomTraComp b);
struct mli_Vec mliHomTra_dir_inverse(
        const struct mliHomTra *t,
        const struct mli_Vec in);
struct mli_Vec mliHomTra_dir(
        const struct mliHomTra *t,
        const struct mli_Vec in);
struct mli_Vec mliHomTra_pos_inverse(
        const struct mliHomTra *t,
        const struct mli_Vec in);
struct mli_Vec mliHomTra_pos(
        const struct mliHomTra *t,
        const struct mli_Vec in);
struct mliRay mliHomTra_ray_inverse(
        const struct mliHomTra *t,
        const struct mliRay in);
struct mliRay mliHomTra_ray(const struct mliHomTra *t, const struct mliRay in);
struct mliRay mli_transform_ray_inverse(
        const struct mliMat *rotation,
        const struct mli_Vec translation,
        const struct mliRay in);
struct mliRay mli_transform_ray(
        const struct mliMat *rotation,
        const struct mli_Vec translation,
        const struct mliRay in);
struct mli_Vec mli_transform_position_inverse(
        const struct mliMat *rotation,
        const struct mli_Vec translation,
        const struct mli_Vec in);
struct mli_Vec mli_transform_position(
        const struct mliMat *rotation,
        const struct mli_Vec translation,
        const struct mli_Vec in);
struct mli_Vec mli_transform_orientation_inverse(
        const struct mliMat *rotation,
        const struct mli_Vec in);
struct mli_Vec mli_transform_orientation(
        const struct mliMat *rotation,
        const struct mli_Vec in);
struct mliHomTra mliHomTra_from_compact(const struct mliHomTraComp trafo);
#endif
