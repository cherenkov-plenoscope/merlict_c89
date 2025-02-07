/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLI_MATERIALS_SERIALIZE_H_
#define MLI_MATERIALS_SERIALIZE_H_

#include "../io/io.h"
#include "../chk/chk.h"
#include "materials.h"

chk_rc mli_Materials_to_io(const struct mli_Materials *self, struct mli_IO *f);
chk_rc mli_Materials_from_io(struct mli_Materials *self, struct mli_IO *f);
#endif
