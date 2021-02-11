/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MERLICT_C89_MLIOCTREE_SERIALIZE_H_
#define MERLICT_C89_MLIOCTREE_SERIALIZE_H_

#include <stdio.h>
#include "mliOcTree.h"

int mliOcTree_fwrite(const struct mliOcTree *octree, FILE *f);
int mliOcTree_malloc_fread(struct mliOcTree *octree, FILE *f);

int mliOcTree_write_to_path(const struct mliOcTree *octree, const char *path);
int mliOcTree_malloc_from_path(struct mliOcTree *octree, const char *path);
#endif
