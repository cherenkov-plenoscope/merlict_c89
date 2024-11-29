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

MTL_VEC_DEFINITON(mtl, Dummy, struct mtlDummy)
MTL_VEC_DEFINITON(mtl, DummyPtr, struct mtlDummy *)

MTL_VEC_TESTING_DEFINITON(mtl, Dummy, struct mtlDummy)
MTL_VEC_TESTING_DEFINITON(mtl, DummyPtr, struct mtlDummy *)

#endif
