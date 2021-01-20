/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MERLICT_C89_MLIOBJECT_OBB_H_
#define MERLICT_C89_MLIOBJECT_OBB_H_

#include "mliHomTra.h"
#include "mliObject.h"
#include "mliOBB.h"

int mliObject_has_overlap_obb(
        const struct mliObject *obj,
        const struct mliHomTraComp local2root_comp,
        const struct mliOBB obb);

struct mliOBB mliObject_obb(
        const struct mliObject *obj,
        const struct mliHomTraComp local2root_comp);

#endif
