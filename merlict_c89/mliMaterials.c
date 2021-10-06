/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "mliMaterials.h"

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
error:
        mliMaterials_free(res);
        return 0;
}

void mliMaterials_info_fprint(FILE *f, const struct mliMaterials *res)
{
        uint32_t i = 0;
        fprintf(f, "Materials:\n");

        fprintf(f, "    media:\n");
        for (i = 0; i < res->num_media; i++) {
                fprintf(f, "        ");
                fprintf(f, "% 3d, %-32s  ", i, res->medium_names[i].c_str);
                fprintf(f,
                        "absorbtion (%d), refraction (%d)\n",
                        res->media[i].absorbtion.num_points,
                        res->media[i].refraction.num_points);
        }

        fprintf(f, "    surfaces:\n");
        for (i = 0; i < res->num_surfaces; i++) {
                fprintf(f,
                        "        % 3d, %-32s  ",
                        i,
                        res->surface_names[i].c_str);
                fprintf(f, "model: ");
                if (res->surfaces[i].material == MLI_MATERIAL_TRANSPARENT) {
                        fprintf(f, "transparent,  ");
                } else if (res->surfaces[i].material == MLI_MATERIAL_PHONG) {
                        fprintf(f, "Phong      ,  ");
                } else {
                        fprintf(f, "UNKNOWN    ,  ");
                }
                fprintf(f,
                        "specular-refl. (%d),  "
                        "diffuse-refl. (%d), ",
                        res->surfaces[i].specular_reflection.num_points,
                        res->surfaces[i].diffuse_reflection.num_points);
                fprintf(f, "color: [%1.1f, %1.1f, %1.1f]\n",
                        res->surfaces[i].color.r,
                        res->surfaces[i].color.g,
                        res->surfaces[i].color.b);
        }

        fprintf(f, "    boundary_layers:\n");
        for (i = 0; i < res->num_boundary_layers; i++) {
                fprintf(f, "        ");
                fprintf(f,
                        "% 3d, %-32s  ",
                        i,
                        res->boundary_layer_names[i].c_str);
                fprintf(f,
                        "inner (medium: % 3d, surface : % 3d)",
                        res->boundary_layers[i].inner.medium,
                        res->boundary_layers[i].inner.surface);
                fprintf(f, ",  ");
                fprintf(f,
                        "outer (medium: % 3d, surface : % 3d)",
                        res->boundary_layers[i].outer.medium,
                        res->boundary_layers[i].outer.surface);
                fprintf(f, "\n");
        }

        fprintf(f,
                "    default medium: %s\n",
                res->medium_names[res->default_medium].c_str);
}
