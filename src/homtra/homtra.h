/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLI_HOMTRA_H_
#define MLI_HOMTRA_H_

#include "../vec/vec.h"
#include "../ray/ray.h"
#include "../mat/mat.h"
#include "../quaternion/quaternion.h"

struct mli_HomTraComp {
        struct mli_Vec translation;
        struct mli_Quaternion rotation;
};

struct mli_HomTra {
        struct mli_Vec translation;
        struct mli_Mat rotation;
};

void mli_HomTraComp_print(const struct mli_HomTra h);
struct mli_HomTraComp mli_HomTraComp_set(
        const struct mli_Vec translation,
        const struct mli_Quaternion rotation);
struct mli_HomTraComp mli_HomTraComp_sequence(
        const struct mli_HomTraComp a,
        const struct mli_HomTraComp b);
int mli_HomTraComp_equal(
        const struct mli_HomTraComp a,
        const struct mli_HomTraComp b);
struct mli_Vec mli_HomTraComp_dir_inverse(
        const struct mli_HomTra *t,
        const struct mli_Vec in);
struct mli_Vec mli_HomTraComp_dir(
        const struct mli_HomTra *t,
        const struct mli_Vec in);
struct mli_Vec mli_HomTraComp_pos_inverse(
        const struct mli_HomTra *t,
        const struct mli_Vec in);
struct mli_Vec mli_HomTraComp_pos(
        const struct mli_HomTra *t,
        const struct mli_Vec in);
struct mliRay mli_HomTraComp_ray_inverse(
        const struct mli_HomTra *t,
        const struct mliRay in);
struct mliRay mli_HomTraComp_ray(
        const struct mli_HomTra *t,
        const struct mliRay in);
struct mliRay mli_transform_ray_inverse(
        const struct mli_Mat *rotation,
        const struct mli_Vec translation,
        const struct mliRay in);
struct mliRay mli_transform_ray(
        const struct mli_Mat *rotation,
        const struct mli_Vec translation,
        const struct mliRay in);
struct mli_Vec mli_transform_position_inverse(
        const struct mli_Mat *rotation,
        const struct mli_Vec translation,
        const struct mli_Vec in);
struct mli_Vec mli_transform_position(
        const struct mli_Mat *rotation,
        const struct mli_Vec translation,
        const struct mli_Vec in);
struct mli_Vec mli_transform_orientation_inverse(
        const struct mli_Mat *rotation,
        const struct mli_Vec in);
struct mli_Vec mli_transform_orientation(
        const struct mli_Mat *rotation,
        const struct mli_Vec in);
struct mli_HomTra mli_HomTraComp_from_compact(
        const struct mli_HomTraComp trafo);
#endif
