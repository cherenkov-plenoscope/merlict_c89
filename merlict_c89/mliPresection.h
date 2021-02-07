/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MERLICT_C89_MLIPRESECTION_H_
#define MERLICT_C89_MLIPRESECTION_H_

#include "mliVec.h"
#include "mliGeometryId.h"

struct mliPresection {
        struct mliGeometryId geometry_id;
        struct mliVec position_local;
        double distance_of_ray;
};

struct mliPresection mliPresection_init(void);
#endif
