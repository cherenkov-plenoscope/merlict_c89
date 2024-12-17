/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLIOCTREE_SERIALIZE_H_
#define MLIOCTREE_SERIALIZE_H_

#include "../io/io.h"
#include "mliOcTree.h"

int mliOcTree_fwrite(const struct mliOcTree *octree, struct mli_IO *f);
int mliOcTree_malloc_fread(struct mliOcTree *octree, struct mli_IO *f);

#endif
