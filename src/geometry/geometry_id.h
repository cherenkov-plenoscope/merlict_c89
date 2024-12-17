/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLI_GEOMETRYID_H_
#define MLI_GEOMETRYID_H_

#include <stdint.h>

struct mli_GeometryId {
        uint32_t robj;
        uint32_t face;
};

struct mli_GeometryId mli_GeometryId_init(void);
#endif
