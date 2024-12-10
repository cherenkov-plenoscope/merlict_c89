/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLI_RESOURCENAMEMAP_H_
#define MLI_RESOURCENAMEMAP_H_

#include "mliDynMap.h"

struct mli_ResourceNameMap {
        struct mliDynMap media;
        struct mliDynMap surfaces;
        struct mliDynMap boundary_layers;
};
struct mli_ResourceNameMap mli_ResourceNameMap_init(void);
int mli_ResourceNameMap_malloc(struct mli_ResourceNameMap *namemap);
void mli_ResourceNameMap_free(struct mli_ResourceNameMap *namemap);

#endif
