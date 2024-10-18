/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "mliMaterials.h"
#include <stdlib.h>
#include "chk.h"

struct mliMaterialsCapacity mliMaterialsCapacity_init(void)
{
        struct mliMaterialsCapacity cap;
        cap.num_surfaces = 0;
        cap.num_media = 0;
        cap.num_boundary_layers = 0;
        return cap;
}

struct mliMaterials mliMaterials_init(void)
{
        struct mliMaterials res;
        res.default_medium = 0u;

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

void mliMaterials_free(struct mliMaterials *res)
{
        uint64_t i;
        for (i = 0; i < res->num_media; i++) {
                mliMedium_free(&(res->media[i]));
        }
        free(res->media);
        free(res->medium_names);

        for (i = 0; i < res->num_surfaces; i++) {
                mliSurface_free(&(res->surfaces[i]));
        }
        free(res->surfaces);
        free(res->surface_names);

        free(res->boundary_layers);
        free(res->boundary_layer_names);

        (*res) = mliMaterials_init();
}

int mliMaterials_malloc(
        struct mliMaterials *res,
        const struct mliMaterialsCapacity rescap)
{
        uint64_t i;
        mliMaterials_free(res);
        res->num_surfaces = rescap.num_surfaces;
        res->num_media = rescap.num_media;
        res->num_boundary_layers = rescap.num_boundary_layers;

        chk_malloc(res->media, struct mliMedium, res->num_media);
        chk_malloc(res->medium_names, struct mliName, res->num_media);
        for (i = 0; i < res->num_media; i++) {
                res->media[i] = mliMedium_init();
                res->medium_names[i] = mliName_init();
        }

        chk_malloc(res->surfaces, struct mliSurface, res->num_surfaces);
        chk_malloc(res->surface_names, struct mliName, res->num_surfaces);
        for (i = 0; i < res->num_surfaces; i++) {
                res->surfaces[i] = mliSurface_init();
                res->surface_names[i] = mliName_init();
        }

        chk_malloc(
                res->boundary_layers,
                struct mliBoundaryLayer,
                res->num_boundary_layers);
        chk_malloc(
                res->boundary_layer_names,
                struct mliName,
                res->num_boundary_layers);
        for (i = 0; i < res->num_boundary_layers; i++) {
                res->boundary_layer_names[i] = mliName_init();
        }

        return 1;
chk_error:
        mliMaterials_free(res);
        return 0;
}

void mliMaterials_info_fprint(FILE *f, const struct mliMaterials *res)
{
        uint32_t i = 0;
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
                fprintf(f, "%24s ", res->medium_names[i].cstr);
                fprintf(f, "%12d ", res->media[i].absorbtion.num_points);
                fprintf(f, "%12d ", res->media[i].refraction.num_points);
                if (i == res->default_medium) {
                        fprintf(f, "%12s", "True");
                }
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
                fprintf(f, "%24s ", res->surface_names[i].cstr);
                if (res->surfaces[i].material == MLI_MATERIAL_TRANSPARENT) {
                        fprintf(f, "%12s ", "transparent");
                } else if (res->surfaces[i].material == MLI_MATERIAL_PHONG) {
                        fprintf(f, "%12s ", "Phong");
                } else {
                        fprintf(f, "%12s ", "UNKNOWN");
                }
                fprintf(f,
                        "%6d ",
                        res->surfaces[i].specular_reflection.num_points);
                fprintf(f,
                        "%6d ",
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
                fprintf(f, "%24s ", res->boundary_layer_names[i].cstr);

                fprintf(f, "%8d ", res->boundary_layers[i].inner.medium);
                fprintf(f, "%8d ", res->boundary_layers[i].inner.surface);

                fprintf(f, "%8d ", res->boundary_layers[i].outer.medium);
                fprintf(f, "%8d ", res->boundary_layers[i].outer.surface);

                fprintf(f, "\n");
        }
}
