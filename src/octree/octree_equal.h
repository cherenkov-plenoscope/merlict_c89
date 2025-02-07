/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLI_OCTREE_EQUAL_H_
#define MLI_OCTREE_EQUAL_H_

#include "octree.h"
#include "../bool/bool.h"

mli_bool mli_OcTree_equal(
        const struct mli_OcTree *a,
        const struct mli_OcTree *b);
#endif
