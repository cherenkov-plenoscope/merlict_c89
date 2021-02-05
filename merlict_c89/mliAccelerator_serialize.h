/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MERLICT_C89_MLIACCELERATOR_SERIALIZE_H_
#define MERLICT_C89_MLIACCELERATOR_SERIALIZE_H_

#include <stdio.h>
#include "mliAccelerator.h"

int mliAccelerator_malloc_fread(struct mliAccelerator *accel, FILE *f);
int mliAccelerator_fwrite(const struct mliAccelerator *accel, FILE *f);
#endif
