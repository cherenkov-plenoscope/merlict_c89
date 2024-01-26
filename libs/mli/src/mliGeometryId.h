/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLIGEOMETRYID_H_
#define MLIGEOMETRYID_H_

#include <stdint.h>

struct mliGeometryId {
        uint32_t robj;
        uint32_t face;
};

struct mliGeometryId mliGeometryId_init(void);
#endif
