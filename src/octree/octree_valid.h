/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLI_OCTREE_VALID_H_
#define MLI_OCTREE_VALID_H_

#include <stdint.h>
#include "octree.h"

int mli_OcTree_valid(const struct mli_OcTree *octree);
int mli_OcTree_valid_wrt_links(
        const struct mli_OcTree *octree,
        const uint32_t num_links);
#endif
