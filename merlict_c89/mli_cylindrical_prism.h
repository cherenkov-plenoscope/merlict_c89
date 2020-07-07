/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MERLICT_C89_MLI_CYLINDRICAL_PRISM_H_
#define MERLICT_C89_MLI_CYLINDRICAL_PRISM_H_

#include <math.h>
#include <stdint.h>

#include "mliVec.h"

int mli_inside_cylindrical_prism(const struct mliVec v, const double radius);
#endif
