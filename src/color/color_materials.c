/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "color_materials.h"
#include <stdlib.h>
#include "../chk/chk.h"

struct mli_ColorMaterials mli_ColorMaterials_init(void)
{
        struct mli_ColorMaterials colmat;

        colmat.num_media = 0u;
        colmat.media = NULL;

        colmat.num_surfaces = 0u;
        colmat.surfaces = NULL;
        return colmat;
}

void mli_ColorMaterials_free(struct mli_ColorMaterials *colmat)
{
        free(colmat->media);
        free(colmat->surfaces);
        (*colmat) = mli_ColorMaterials_init();
}

int mli_ColorMaterials_malloc(
        struct mli_ColorMaterials *colmat,
        const struct mli_MaterialsCapacity rescap)
{
        mli_ColorMaterials_free(colmat);
        colmat->num_surfaces = rescap.num_surfaces;
        colmat->num_media = rescap.num_media;

        chk_malloc(
                colmat->surfaces,
                struct mli_ColorSurface,
                colmat->num_surfaces);
        chk_malloc(colmat->media, struct mli_ColorMedium, colmat->num_media);

        return 1;
chk_error:
        mli_ColorMaterials_free(colmat);
        return 0;
}

int mli_ColorMaterials_malloc_from_Materials(
        struct mli_ColorMaterials *colmat,
        const struct mli_Materials *mat,
        const struct mli_ColorObserver *colobs)
{
        uint64_t i;
        struct mli_MaterialsCapacity cap;
        cap.num_media = mat->num_media;
        cap.num_surfaces = mat->num_surfaces;

        chk_msg(mli_ColorMaterials_malloc(colmat, cap),
                "Can't malloc ColorMaterials from Materials.");

        for (i = 0; i < mat->num_surfaces; i++) {
                chk_msg(mli_ColorObserver_evaluate(
                                colobs,
                                &mat->surfaces[i].specular_reflection,
                                &colmat->surfaces[i].specular_reflection),
                        "Can't evaluate specular_reflection colors.");
                chk_msg(mli_ColorObserver_evaluate(
                                colobs,
                                &mat->surfaces[i].diffuse_reflection,
                                &colmat->surfaces[i].diffuse_reflection),
                        "Can't evaluate diffuse_reflection colors.");
        }

        for (i = 0; i < mat->num_media; i++) {
                chk_msg(mli_ColorObserver_evaluate(
                                colobs,
                                &mat->media[i].refraction,
                                &colmat->media[i].refraction),
                        "Can't evaluate refraction colors.");
                chk_msg(mli_ColorObserver_evaluate(
                                colobs,
                                &mat->media[i].absorbtion,
                                &colmat->media[i].absorbtion),
                        "Can't evaluate absorbtion colors.");
        }

        return 1;
chk_error:
        mli_ColorMaterials_free(colmat);
        return 0;
}
