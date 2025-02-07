/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLI_ACCELERATOR_EQUAL_H_
#define MLI_ACCELERATOR_EQUAL_H_

#include "accelerator.h"
#include "../bool/bool.h"

mli_bool mli_Accelerator_equal(
        const struct mli_Accelerator *a,
        const struct mli_Accelerator *b);
#endif
