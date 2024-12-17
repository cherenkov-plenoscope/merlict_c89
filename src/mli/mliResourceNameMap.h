/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLI_RESOURCENAMEMAP_H_
#define MLI_RESOURCENAMEMAP_H_

#include "../map/map.h"

struct mli_ResourceNameMap {
        struct mli_Map media;
        struct mli_Map surfaces;
        struct mli_Map boundary_layers;
};
struct mli_ResourceNameMap mli_ResourceNameMap_init(void);
int mli_ResourceNameMap_malloc(struct mli_ResourceNameMap *namemap);
void mli_ResourceNameMap_free(struct mli_ResourceNameMap *namemap);

#endif
