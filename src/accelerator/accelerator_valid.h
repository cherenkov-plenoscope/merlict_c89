/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLI_ACCELERATOR_VALID_H_
#define MLI_ACCELERATOR_VALID_H_

#include "accelerator.h"

int mli_Accelerator_valid(const struct mli_Accelerator *self);
int mli_Accelerator_valid_wrt_Geometry(
        const struct mli_Accelerator *self,
        const struct mli_Geometry *geometry);
#endif
