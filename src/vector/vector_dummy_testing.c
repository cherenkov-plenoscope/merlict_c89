/* Copyright Sebastian Achim Mueller */
#include "vector_dummy_testing.h"

MLI_VECTOR_IMPLEMENTATION(mtlDynDummy, struct mtlDummy)
MLI_VECTOR_IMPLEMENTATION(mtlDynDummyPtr, struct mtlDummy *)

MTL_VEC_TESTING_IMPLEMENTATION(mtlDynDummy, struct mtlDummy)
MTL_VEC_TESTING_IMPLEMENTATION(mtlDynDummyPtr, struct mtlDummy *)

MLI_VECTOR_IMPLEMENTATION_ZERO_TERMINATION(mtl_VectorChar, char)
