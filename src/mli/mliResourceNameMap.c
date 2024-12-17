/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "mliResourceNameMap.h"
#include "../chk/chk.h"

struct mli_ResourceNameMap mli_ResourceNameMap_init(void)
{
        struct mli_ResourceNameMap nm;
        nm.media = mli_Map_init();
        nm.surfaces = mli_Map_init();
        nm.boundary_layers = mli_Map_init();
        return nm;
}

int mli_ResourceNameMap_malloc(struct mli_ResourceNameMap *namemap)
{
        mli_ResourceNameMap_free(namemap);
        chk_mem(mli_Map_malloc(&namemap->media));
        chk_mem(mli_Map_malloc(&namemap->surfaces));
        chk_mem(mli_Map_malloc(&namemap->boundary_layers));
        return 1;
chk_error:
        return 0;
}

void mli_ResourceNameMap_free(struct mli_ResourceNameMap *namemap)
{
        mli_Map_free(&namemap->media);
        mli_Map_free(&namemap->surfaces);
        mli_Map_free(&namemap->boundary_layers);
}
