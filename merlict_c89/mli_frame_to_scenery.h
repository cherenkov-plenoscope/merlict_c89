/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MERLICT_C89_MLI_FRAME_TO_SCENERY_H_
#define MERLICT_C89_MLI_FRAME_TO_SCENERY_H_

#include <math.h>
#include <stdint.h>

#include "mli_debug.h"
#include "mliScenery.h"
#include "mliUserScenery.h"

struct mliPrimitiveCount {
        uint32_t vertices;
        uint32_t triangles;
        uint32_t spherical_cap_hex;
        uint32_t spheres;
        uint32_t cylinders;
        uint32_t hexagons;
        uint32_t bicircleplanes;
        uint32_t discs;
};

int mliScenery_malloc_from_json_path(
        struct mliScenery *scenery,
        const char *path);
int mliScenery_malloc_from_mliUserScenery(
        struct mliScenery *scenery,
        struct mliUserScenery *uscn);
int __mliScenery_set_primitive(
        struct mliScenery *scenery,
        const struct mliFrame *frame,
        struct mliPrimitiveCount *count);
void _mli_set_user_id(
        struct mliScenery *scenery,
        const uint64_t primitive_type,
        const uint64_t primitive_count,
        const uint64_t user_id);
int __mliScenery_set_primitive_capacity(
        struct mliSceneryCapacity *scn_cap,
        const struct mliFrame *frame);
struct mliPrimitiveCount mliPrimitiveCount_init(void);
#endif
