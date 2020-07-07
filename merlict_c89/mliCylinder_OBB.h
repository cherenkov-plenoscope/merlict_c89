/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MERLICT_C89_MLICYLINDER_OBB_H_
#define MERLICT_C89_MLICYLINDER_OBB_H_

#include <math.h>

#include "mliScenery.h"
#include "mliOBB.h"
#include "mliCylinder.h"
#include "mliSphere_OBB.h"
#include "mliRay_OBB.h"

struct mliOBB mliCylinder_obb(
        const struct mliCylinder cylinder,
        const struct mliHomTraComp local2root_comp);
int mliCylinder_has_overlap_obb(
        const struct mliCylinder cylinder,
        const struct mliHomTraComp local2root_comp,
        const struct mliOBB obb);
struct mliVec mliCylinder_end_root(
        const struct mliCylinder cylinder,
        const struct mliHomTra *local2root);
struct mliVec mliCylinder_start_root(
        const struct mliCylinder cylinder,
        const struct mliHomTra *local2root);
struct mliVec mliCylinder_end_local(const struct mliCylinder cylinder);
struct mliVec mliCylinder_start_local(const struct mliCylinder cylinder);
#endif
