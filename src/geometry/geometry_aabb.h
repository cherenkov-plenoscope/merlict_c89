/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLI_GEOMETRY_AABB_H_
#define MLI_GEOMETRY_AABB_H_

#include <stdint.h>
#include "geometry.h"
#include "../bool/bool.h"
#include "../accelerator/accelerator.h"
#include "../aabb/aabb.h"

mli_bool mli_Geometry_robject_has_overlap_aabb_void(
        const void *accgeo,
        const uint32_t robject_idx,
        const struct mli_AABB aabb);

mli_bool mli_Geometry_robject_has_overlap_aabb(
        const struct mli_GeometryAndAccelerator *accgeo,
        const uint32_t robject_idx,
        const struct mli_AABB aabb);

#endif
