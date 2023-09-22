/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "mliGeometryId.h"

struct mliGeometryId mliGeometryId_init(void)
{
        struct mliGeometryId id;
        id.robj = 0u;
        id.face = 0u;
        return id;
}
