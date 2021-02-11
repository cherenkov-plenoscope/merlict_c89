/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MERLICT_C89_MLIOCTREE_VALID_H_
#define MERLICT_C89_MLIOCTREE_VALID_H_

#include "mliOcTree.h"

int mliOcTree_valid(const struct mliOcTree *octree);
int mliOcTree_valid_wrt_links(
        const struct mliOcTree *octree,
        const uint32_t num_links);
#endif
