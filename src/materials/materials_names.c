/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "materials_names.h"
#include "../chk/chk.h"

struct mli_materials_Names mli_materials_Names_init(void)
{
        struct mli_materials_Names nm;
        nm.media = mli_Map_init();
        nm.surfaces = mli_Map_init();
        nm.boundary_layers = mli_Map_init();
        return nm;
}

int mli_materials_Names_malloc(struct mli_materials_Names *namemap)
{
        mli_materials_Names_free(namemap);
        chk_mem(mli_Map_malloc(&namemap->media));
        chk_mem(mli_Map_malloc(&namemap->surfaces));
        chk_mem(mli_Map_malloc(&namemap->boundary_layers));
        return 1;
chk_error:
        return 0;
}

void mli_materials_Names_free(struct mli_materials_Names *namemap)
{
        mli_Map_free(&namemap->media);
        mli_Map_free(&namemap->surfaces);
        mli_Map_free(&namemap->boundary_layers);
}
