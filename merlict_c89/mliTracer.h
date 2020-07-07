/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MERLICT_C89_MLITRACER_H_
#define MERLICT_C89_MLITRACER_H_

#include <math.h>
#include <stdint.h>

#include "mliVec.h"
#include "mliRay.h"
#include "mliHomTra.h"
#include "mliColor.h"
#include "mliFunc.h"
#include "mliScenery.h"
#include "mliScenery_object_interface.h"
#include "mliIntersection.h"
#include "mliOcTree.h"
#include "mli_ray_octree_traversal.h"
#include "mli_intersection_and_scenery.h"

struct mliColor mli_trace(
        const struct mliScenery *scenery,
        const struct mliOcTree *octree,
        const struct mliRay ray);
int mli_first_casual_intersection(
        const struct mliScenery *scenery,
        const struct mliOcTree *octree,
        const struct mliRay ray,
        struct mliIntersection *intersection);
#endif
