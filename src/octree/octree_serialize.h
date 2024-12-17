/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLI_OCTREE_SERIALIZE_H_
#define MLI_OCTREE_SERIALIZE_H_

#include "../io/io.h"
#include "octree.h"

int mliOcTree_to_io(const struct mliOcTree *octree, struct mli_IO *f);
int mliOcTree_from_io(struct mliOcTree *octree, struct mli_IO *f);

#endif
