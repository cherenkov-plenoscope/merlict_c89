/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLI_MEDIUM_SERIALIZE_H_
#define MLI_MEDIUM_SERIALIZE_H_

#include "medium.h"
#include "../io/io.h"

int mliMedium_to_io(const struct mliMedium *med, struct mli_IO *f);
int mliMedium_from_io(struct mliMedium *med, struct mli_IO *f);
#endif
