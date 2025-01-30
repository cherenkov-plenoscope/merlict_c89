/* Copyright 2018-2024 Sebastian Achim Mueller */
#ifndef MLI_BOUNDARYLAYER_MEDIUM_H_
#define MLI_BOUNDARYLAYER_MEDIUM_H_

#include "boundarylayer_medium_transparent.h"
#include "../string/string.h"

#define MLI_BOUNDARYLAYER_MEDIUM_TYPE_NONE 0

union mli_BoundaryLayer_MediumData {
        struct mli_BoundaryLayerMedium_Transparent transparent;
};

struct mli_BoundaryLayer_Medium {
        uint64_t type;
        union mli_BoundaryLayer_MediumData data;
};

struct mli_BoundaryLayer_Medium mli_BoundaryLayer_Medium_init(void);

int mli_BoundaryLayer_Medium_type_to_string(
        const uint64_t type,
        struct mli_String *s);
int mli_BoundaryLayer_Medium_type_from_string(
        const struct mli_String *s,
        uint64_t *id);

#endif
