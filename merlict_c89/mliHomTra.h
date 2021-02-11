/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MERLICT_C89_MLIHOMTRA_H_
#define MERLICT_C89_MLIHOMTRA_H_

#include <math.h>
#include <stdint.h>

#include "mliVec.h"
#include "mliRay.h"
#include "mliRotMat.h"
#include "mliQuaternion.h"

struct mliHomTraComp {
        struct mliVec translation;
        struct mliQuaternion rotation;
};

struct mliHomTra {
        struct mliVec translation;
        struct mliRotMat rotation;
};

void mliHomTra_print(const struct mliHomTra h);
struct mliHomTraComp mliHomTraComp_set(
        const struct mliVec translation,
        const struct mliQuaternion rotation);
struct mliHomTraComp mliHomTraComp_sequence(
        const struct mliHomTraComp a,
        const struct mliHomTraComp b);
int mliHomTraComp_equal(
        const struct mliHomTraComp a,
        const struct mliHomTraComp b);
struct mliVec mliHomTra_dir_inverse(
        const struct mliHomTra *t,
        const struct mliVec in);
struct mliVec mliHomTra_dir(const struct mliHomTra *t, const struct mliVec in);
struct mliVec mliHomTra_pos_inverse(
        const struct mliHomTra *t,
        const struct mliVec in);
struct mliVec mliHomTra_pos(const struct mliHomTra *t, const struct mliVec in);
struct mliRay mliHomTra_ray_inverse(
        const struct mliHomTra *t,
        const struct mliRay in);
struct mliRay mliHomTra_ray(const struct mliHomTra *t, const struct mliRay in);
struct mliRay mli_transform_ray_inverse(
        const struct mliRotMat *rotation,
        const struct mliVec translation,
        const struct mliRay in);
struct mliRay mli_transform_ray(
        const struct mliRotMat *rotation,
        const struct mliVec translation,
        const struct mliRay in);
struct mliVec mli_transform_position_inverse(
        const struct mliRotMat *rotation,
        const struct mliVec translation,
        const struct mliVec in);
struct mliVec mli_transform_position(
        const struct mliRotMat *rotation,
        const struct mliVec translation,
        const struct mliVec in);
struct mliVec mli_transform_orientation_inverse(
        const struct mliRotMat *rotation,
        const struct mliVec in);
struct mliVec mli_transform_orientation(
        const struct mliRotMat *rotation,
        const struct mliVec in);
struct mliHomTra mliHomTra_from_compact(const struct mliHomTraComp trafo);
#endif
