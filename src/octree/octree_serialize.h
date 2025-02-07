/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLI_OCTREE_SERIALIZE_H_
#define MLI_OCTREE_SERIALIZE_H_

#include "../io/io.h"
#include "../chk/chk.h"
#include "octree.h"

chk_rc mli_OcTree_to_io(const struct mli_OcTree *octree, struct mli_IO *f);
chk_rc mli_OcTree_from_io(struct mli_OcTree *octree, struct mli_IO *f);

#endif
