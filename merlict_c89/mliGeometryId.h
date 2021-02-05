/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MERLICT_C89_MLIGEOMETRYID_H_
#define MERLICT_C89_MLIGEOMETRYID_H_

#include <stdint.h>

struct mliGeometryId {
        uint32_t robj;
        uint32_t face;
};

struct mliGeometryId mliGeometryId_init(void);
#endif
