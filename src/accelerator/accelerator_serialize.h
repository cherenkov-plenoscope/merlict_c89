/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLI_ACCELERATOR_SERIALIZE_H_
#define MLI_ACCELERATOR_SERIALIZE_H_

#include "../chk/chk.h"
#include "../io/io.h"
#include "accelerator.h"

chk_rc mli_Accelerator_from_io(struct mli_Accelerator *self, struct mli_IO *f);
chk_rc mli_Accelerator_to_io(
        const struct mli_Accelerator *self,
        struct mli_IO *f);
#endif
