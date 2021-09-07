/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "mliDynArray_template.h"

MLIDYNARRAY_IMPLEMENTATION(mli, Uint32, uint32_t)
MLIDYNARRAY_IMPLEMENTATION(mli, Float, float)
MLIDYNARRAY_IMPLEMENTATION(mli, Double, double)
MLIDYNARRAY_IMPLEMENTATION(mli, Color, struct mliColor)
MLIDYNARRAY_TEST_IMPLEMENTATION(mli, Color, struct mliColor)

MLIDYNARRAY_IMPLEMENTATION(mli, ColorPtr, struct mliColor *)
MLIDYNARRAY_IMPLEMENTATION(mli, Vec, struct mliVec)
MLIDYNARRAY_IMPLEMENTATION(mli, Face, struct mliFace)
