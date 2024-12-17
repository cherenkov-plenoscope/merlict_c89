/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLIACCELERATOR_SERIALIZE_H_
#define MLIACCELERATOR_SERIALIZE_H_

#include "../io/io.h"
#include "mliAccelerator.h"

int mliAccelerator_from_io(struct mliAccelerator *accel, struct mli_IO *f);
int mliAccelerator_to_io(const struct mliAccelerator *accel, struct mli_IO *f);
#endif
