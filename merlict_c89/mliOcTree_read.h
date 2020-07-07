/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MERLICT_C89_MLIOCTREE_READ_H_
#define MERLICT_C89_MLIOCTREE_READ_H_

#include <math.h>
#include <stdint.h>

#include "mli_debug.h"
#include "mliOcTree.h"

int mliOcTree_read_and_malloc_from_path(
        struct mliOcTree *octree,
        const char *path);
int mliOcTree_read_and_malloc_from_file(struct mliOcTree *octree, FILE *f);
#endif
