/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "mliColorMaterials.h"
#include <stdlib.h>
#include "../../chk/src/chk.h"

struct mliColorMaterials mliColorMaterials_init(void)
{
        struct mliColorMaterials colmat;

        colmat.num_media = 0u;
        colmat.media = NULL;

        colmat.num_surfaces = 0u;
        colmat.surfaces = NULL;
        return colmat;
}

void mliColorMaterials_free(struct mliColorMaterials *colmat)
{
        free(colmat->media);
        free(colmat->surfaces);
        (*colmat) = mliColorMaterials_init();
}

int mliColorMaterials_malloc(
        struct mliColorMaterials *colmat,
        const struct mliMaterialsCapacity rescap)
{
        mliColorMaterials_free(colmat);
        colmat->num_surfaces = rescap.num_surfaces;
        colmat->num_media = rescap.num_media;

        chk_malloc(
                colmat->surfaces, struct mliColorSurface, colmat->num_surfaces);
        chk_malloc(colmat->media, struct mliColorMedium, colmat->num_media);

        return 1;
chk_error:
        mliColorMaterials_free(colmat);
        return 0;
}

int mliColorMaterials_malloc_from_Materials(
        struct mliColorMaterials *colmat,
        const struct mliMaterials *mat,
        const struct mliColorObserver *colobs)
{
        uint64_t i;
        struct mliMaterialsCapacity cap;
        cap.num_media = mat->num_media;
        cap.num_surfaces = mat->num_surfaces;

        chk_msg(mliColorMaterials_malloc(colmat, cap),
                "Can't malloc ColorMaterials from Materials.");

        for (i = 0; i < mat->num_surfaces; i++) {
                chk_msg(mliColorObserver_evaluate(
                                colobs,
                                &mat->surfaces[i].specular_reflection,
                                &colmat->surfaces[i].specular_reflection),
                        "Can't evaluate specular_reflection colors.");
                chk_msg(mliColorObserver_evaluate(
                                colobs,
                                &mat->surfaces[i].diffuse_reflection,
                                &colmat->surfaces[i].diffuse_reflection),
                        "Can't evaluate diffuse_reflection colors.");
        }

        for (i = 0; i < mat->num_media; i++) {
                chk_msg(mliColorObserver_evaluate(
                                colobs,
                                &mat->media[i].refraction,
                                &colmat->media[i].refraction),
                        "Can't evaluate refraction colors.");
                chk_msg(mliColorObserver_evaluate(
                                colobs,
                                &mat->media[i].absorbtion,
                                &colmat->media[i].absorbtion),
                        "Can't evaluate absorbtion colors.");
        }

        return 1;
chk_error:
        mliColorMaterials_free(colmat);
        return 0;
}
