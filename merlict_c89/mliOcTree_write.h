/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MERLICT_C89_MLIOCTREE_WRITE_H_
#define MERLICT_C89_MLIOCTREE_WRITE_H_

#include "mliOcTree.h"

int mliOcTree_write_to_path(const struct mliOcTree *octree, const char *path);
int mliOcTree_fwrite(const struct mliOcTree *octree, FILE *f);
#endif
