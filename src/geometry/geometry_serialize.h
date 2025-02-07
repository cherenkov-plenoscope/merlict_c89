/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLI_GEOMETRY_SERIALIZE_H_
#define MLI_GEOMETRY_SERIALIZE_H_

#include "../chk/chk.h"
#include "../io/io.h"
#include "geometry.h"

chk_rc mli_Geometry_to_io(const struct mli_Geometry *scenery, struct mli_IO *f);
chk_rc mli_Geometry_from_io(struct mli_Geometry *scenery, struct mli_IO *f);
#endif
