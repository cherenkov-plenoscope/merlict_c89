/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "materials.h"
#include <stdlib.h>
#include "../chk/chk.h"
#include "../func/func_fprint.h"

struct mli_MaterialsCapacity mli_MaterialsCapacity_init(void)
{
        struct mli_MaterialsCapacity cap;
        cap.num_spectra = 0;
        cap.num_surfaces = 0;
        cap.num_media = 0;
        cap.num_boundary_layers = 0;
        return cap;
}

struct mli_Materials mli_Materials_init(void)
{
        struct mli_Materials res;
        res.default_medium = 0u;

        res.spectra = mli_SpectrumArray_init();
        res.surfaces2 = mli_SurfaceArray_init();
        res.media2 = mli_MediumArray_init();

        res.layers2 = mli_BoundaryLayer2Array_init();

        res.num_media = 0u;
        res.media = NULL;
        res.medium_names = NULL;

        res.num_surfaces = 0u;
        res.surfaces = NULL;
        res.surface_names = NULL;

        res.num_boundary_layers = 0u;
        res.boundary_layers = NULL;
        res.boundary_layer_names = NULL;
        return res;
}

void mli_Materials_free(struct mli_Materials *res)
{
        uint64_t i;
        mli_SpectrumArray_free(&res->spectra);
        mli_SurfaceArray_free(&res->surfaces2);
        mli_MediumArray_free(&res->media2);

        mli_BoundaryLayer2Array_free(&res->layers2);

        for (i = 0; i < res->num_media; i++) {
                mli_Medium_free(&(res->media[i]));
                mli_String_free(&(res->medium_names[i]));
        }
        free(res->media);
        free(res->medium_names);

        for (i = 0; i < res->num_surfaces; i++) {
                mli_Surface_free(&(res->surfaces[i]));
                mli_String_free(&res->surface_names[i]);
        }
        free(res->surfaces);
        free(res->surface_names);

        for (i = 0; i < res->num_boundary_layers; i++) {
                mli_String_free(&res->boundary_layer_names[i]);
        }
        free(res->boundary_layers);
        free(res->boundary_layer_names);

        (*res) = mli_Materials_init();
}

int mli_Materials_malloc(
        struct mli_Materials *res,
        const struct mli_MaterialsCapacity rescap)
{
        uint64_t i;
        mli_Materials_free(res);
        res->num_surfaces = rescap.num_surfaces;
        res->num_media = rescap.num_media;
        res->num_boundary_layers = rescap.num_boundary_layers;

        chk(mli_SpectrumArray_malloc(&res->spectra, rescap.num_spectra));
        for (i = 0; i < res->spectra.size; i++) {
                res->spectra.array[i] = mli_Spectrum_init();
        }

        chk(mli_SurfaceArray_malloc(&res->surfaces2, rescap.num_surfaces));
        for (i = 0; i < res->surfaces2.size; i++) {
                res->surfaces2.array[i] = mli_BoundaryLayer_Surface_init();
        }

        chk(mli_MediumArray_malloc(&res->media2, rescap.num_media));
        for (i = 0; i < res->media2.size; i++) {
                res->media2.array[i] = mli_BoundaryLayer_Medium_init();
        }

        chk(mli_BoundaryLayer2Array_malloc(
                &res->layers2, rescap.num_boundary_layers));
        for (i = 0; i < res->layers2.size; i++) {
                res->layers2.array[i] = mli_BoundaryLayer2_init();
        }

        chk_malloc(res->media, struct mli_Medium, res->num_media);
        chk_malloc(res->medium_names, struct mli_String, res->num_media);
        for (i = 0; i < res->num_media; i++) {
                res->media[i] = mli_Medium_init();
                res->medium_names[i] = mli_String_init();
        }

        chk_malloc(res->surfaces, struct mli_Surface, res->num_surfaces);
        chk_malloc(res->surface_names, struct mli_String, res->num_surfaces);
        for (i = 0; i < res->num_surfaces; i++) {
                res->surfaces[i] = mli_Surface_init();
                res->surface_names[i] = mli_String_init();
        }

        chk_malloc(
                res->boundary_layers,
                struct mli_BoundaryLayer,
                res->num_boundary_layers);
        chk_malloc(
                res->boundary_layer_names,
                struct mli_String,
                res->num_boundary_layers);
        for (i = 0; i < res->num_boundary_layers; i++) {
                res->boundary_layer_names[i] = mli_String_init();
        }

        return 1;
chk_error:
        mli_Materials_free(res);
        return 0;
}

void mli_Materials_info_fprint(FILE *f, const struct mli_Materials *res)
{
        uint32_t i = 0;
        struct mli_Func_fprint_Config pltcfg;

        fprintf(f, "materials\n");
        fprintf(f, "---------\n");
        fprintf(f, "\n");

        fprintf(f, "    media\n");
        fprintf(f, "    ");
        for (i = 0; i < 70; i++) {
                fprintf(f, "-");
        }
        fprintf(f, "\n");
        fprintf(f, "    ");
        fprintf(f, "%3s ", "#");
        fprintf(f, "%24s ", "name");
        fprintf(f, "%12s ", "absorbtion");
        fprintf(f, "%12s ", "refraction");
        fprintf(f, "%12s ", "default");
        fprintf(f, "\n");
        fprintf(f, "    ");
        for (i = 0; i < 70; i++) {
                fprintf(f, "-");
        }
        fprintf(f, "\n");
        for (i = 0; i < res->num_media; i++) {
                fprintf(f, "    ");
                fprintf(f, "% 3d ", i);
                fprintf(f, "%24s ", res->medium_names[i].array);
                fprintf(f, "%12ld ", res->media[i].absorbtion.num_points);
                fprintf(f, "%12ld ", res->media[i].refraction.num_points);
                if (i == res->default_medium) {
                        fprintf(f, "%12s", "True");
                }
                fprintf(f, "\n");
                fprintf(f, "    ");
                fprintf(f, "absorbtion\n");
                pltcfg.x_start = 200e-9;
                pltcfg.x_stop = 1200e-9;
                pltcfg.x_num = 54;
                pltcfg.y_start = 0.0;
                pltcfg.y_stop = 5.0;
                pltcfg.y_num = 12;
                mli_Func_fprint(f, &res->media[i].absorbtion, pltcfg);
                fprintf(f, "\n");

                fprintf(f, "\n");
                fprintf(f, "    ");
                fprintf(f, "refraction\n");
                pltcfg.x_start = 200e-9;
                pltcfg.x_stop = 1200e-9;
                pltcfg.x_num = 54;
                pltcfg.y_start = 0.0;
                pltcfg.y_stop = 2.0;
                pltcfg.y_num = 12;
                mli_Func_fprint(f, &res->media[i].refraction, pltcfg);
                fprintf(f, "\n");
        }
        fprintf(f, "\n");

        fprintf(f, "    surfaces\n");
        fprintf(f, "    ");
        for (i = 0; i < 70; i++) {
                fprintf(f, "-");
        }
        fprintf(f, "\n");
        fprintf(f, "    ");
        fprintf(f, "%48s ", "spec.");
        fprintf(f, "%6s ", "diff.");
        fprintf(f, "\n");
        fprintf(f, "    ");
        fprintf(f, "%3s ", "#");
        fprintf(f, "%24s ", "name");
        fprintf(f, "%12s ", "material");
        fprintf(f, "%6s ", "refl.");
        fprintf(f, "%6s ", "refl.");
        fprintf(f, "\n");
        fprintf(f, "    ");
        for (i = 0; i < 70; i++) {
                fprintf(f, "-");
        }
        fprintf(f, "\n");
        for (i = 0; i < res->num_surfaces; i++) {
                fprintf(f, "    ");
                fprintf(f, "% 3d ", i);
                fprintf(f, "%24s ", res->surface_names[i].array);
                if (res->surfaces[i].material == MLI_SURFACE_TRANSPARENT) {
                        fprintf(f, "%12s ", "transparent");
                } else if (res->surfaces[i].material == MLI_SURFACE_PHONG) {
                        fprintf(f, "%12s ", "Phong");
                } else {
                        fprintf(f, "%12s ", "UNKNOWN");
                }
                fprintf(f,
                        "%6ld ",
                        res->surfaces[i].specular_reflection.num_points);
                fprintf(f,
                        "%6ld ",
                        res->surfaces[i].diffuse_reflection.num_points);
                fprintf(f, "\n");
        }
        fprintf(f, "\n");

        fprintf(f, "    boundary_layers\n");
        fprintf(f, "    ");
        for (i = 0; i < 70; i++) {
                fprintf(f, "-");
        }
        fprintf(f, "\n");
        fprintf(f, "    ");
        fprintf(f, "%36s ", "inner");
        fprintf(f, "%17s ", "outer");
        fprintf(f, "\n");
        fprintf(f, "    ");
        fprintf(f, "%3s ", "#");
        fprintf(f, "%24s ", "name");
        fprintf(f, "%8s ", "medium");
        fprintf(f, "%8s ", "surface");
        fprintf(f, "%8s ", "medium");
        fprintf(f, "%8s ", "surface");
        fprintf(f, "\n");
        fprintf(f, "    ");
        for (i = 0; i < 70; i++) {
                fprintf(f, "-");
        }
        fprintf(f, "\n");

        for (i = 0; i < res->num_boundary_layers; i++) {
                fprintf(f, "    ");
                fprintf(f, "% 3d ", i);
                fprintf(f, "%24s ", res->boundary_layer_names[i].array);

                fprintf(f, "%8d ", res->boundary_layers[i].inner.medium);
                fprintf(f, "%8d ", res->boundary_layers[i].inner.surface);

                fprintf(f, "%8d ", res->boundary_layers[i].outer.medium);
                fprintf(f, "%8d ", res->boundary_layers[i].outer.surface);

                fprintf(f, "\n");
        }
}
