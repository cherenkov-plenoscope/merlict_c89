/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLI_MEDIUM_SERIALIZE_H_
#define MLI_MEDIUM_SERIALIZE_H_

#include "medium.h"
#include "../io/io.h"

int mli_Medium_to_io(const struct mli_Medium *med, struct mli_IO *f);
int mli_Medium_from_io(struct mli_Medium *med, struct mli_IO *f);
#endif
