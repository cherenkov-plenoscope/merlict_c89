/* Copyright Sebastian Achim Mueller */
#ifndef MTL_VEC_DUMMY_TESTING_H_
#define MTL_VEC_DUMMY_TESTING_H_

#include "vector.h"
#include "vector_testing.h"

struct mtlDummy {
        float r;
        float g;
        float b;
};

MLI_VECTOR_DEFINITON(mtlDynDummy, struct mtlDummy)
MLI_VECTOR_DEFINITON(mtlDynDummyPtr, struct mtlDummy *)

MTL_VEC_TESTING_DEFINITON(mtlDynDummy, struct mtlDummy)
MTL_VEC_TESTING_DEFINITON(mtlDynDummyPtr, struct mtlDummy *)

MLI_VECTOR_DEFINITON(mtl_VectorChar, char)

#endif
