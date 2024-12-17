/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLI_ACCELERATOR_SERIALIZE_H_
#define MLI_ACCELERATOR_SERIALIZE_H_

#include "../io/io.h"
#include "accelerator.h"

int mli_Accelerator_from_io(struct mli_Accelerator *self, struct mli_IO *f);
int mli_Accelerator_to_io(
        const struct mli_Accelerator *self,
        struct mli_IO *f);
#endif
