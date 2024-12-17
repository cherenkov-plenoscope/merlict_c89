/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "geometry_id.h"

struct mli_GeometryId mli_GeometryId_init(void)
{
        struct mli_GeometryId id;
        id.robj = 0u;
        id.face = 0u;
        return id;
}
