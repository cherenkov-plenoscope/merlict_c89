/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "mliResourceNameMap.h"
#include "../chk/chk.h"

struct mli_ResourceNameMap mli_ResourceNameMap_init(void)
{
        struct mli_ResourceNameMap nm;
        nm.media = mliDynMap_init();
        nm.surfaces = mliDynMap_init();
        nm.boundary_layers = mliDynMap_init();
        return nm;
}

int mli_ResourceNameMap_malloc(struct mli_ResourceNameMap *namemap)
{
        mli_ResourceNameMap_free(namemap);
        chk_mem(mliDynMap_malloc(&namemap->media));
        chk_mem(mliDynMap_malloc(&namemap->surfaces));
        chk_mem(mliDynMap_malloc(&namemap->boundary_layers));
        return 1;
chk_error:
        return 0;
}

void mli_ResourceNameMap_free(struct mli_ResourceNameMap *namemap)
{
        mliDynMap_free(&namemap->media);
        mliDynMap_free(&namemap->surfaces);
        mliDynMap_free(&namemap->boundary_layers);
}
