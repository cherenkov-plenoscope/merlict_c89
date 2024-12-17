/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLI_OCTREE_VALID_H_
#define MLI_OCTREE_VALID_H_

#include <stdint.h>
#include "octree.h"

int mliOcTree_valid(const struct mliOcTree *octree);
int mliOcTree_valid_wrt_links(
        const struct mliOcTree *octree,
        const uint32_t num_links);
#endif
