/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "materials_names.h"
#include "../chk/chk.h"

struct mli_materials_Names mli_materials_Names_init(void)
{
        struct mli_materials_Names nm;
        nm.spectra = mli_Map_init();
        nm.media = mli_Map_init();
        nm.surfaces = mli_Map_init();
        nm.boundary_layers = mli_Map_init();
        return nm;
}

chk_rc mli_materials_Names_malloc(struct mli_materials_Names *namemap)
{
        mli_materials_Names_free(namemap);
        chk_mem(mli_Map_malloc(&namemap->spectra));
        chk_mem(mli_Map_malloc(&namemap->media));
        chk_mem(mli_Map_malloc(&namemap->surfaces));
        chk_mem(mli_Map_malloc(&namemap->boundary_layers));
        return CHK_SUCCESS;
chk_error:
        return CHK_FAIL;
}

void mli_materials_Names_free(struct mli_materials_Names *namemap)
{
        mli_Map_free(&namemap->spectra);
        mli_Map_free(&namemap->media);
        mli_Map_free(&namemap->surfaces);
        mli_Map_free(&namemap->boundary_layers);
}
