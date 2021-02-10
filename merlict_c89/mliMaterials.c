/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "mliMaterials.h"
#include "mli_debug.h"

struct mliMaterialsCapacity mliMaterialsCapacity_init(void)
{
        struct mliMaterialsCapacity cap;
        cap.num_functions = 0;
        cap.num_colors = 0;
        cap.num_surfaces = 0;
        cap.num_media = 0;
        cap.num_boundary_layers = 0;
        return cap;
}

struct mliMaterials mliMaterials_init(void)
{
        struct mliMaterials res;
        res.default_medium = 0u;

        res.num_functions = 0u;
        res.functions = NULL;
        res.function_names = NULL;

        res.num_colors = 0u;
        res.colors = NULL;
        res.color_names = NULL;

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
        for (i = 0; i < res->num_functions; i++) {
                mliFunc_free(&(res->functions[i]));
        }
        free(res->functions);
        free(res->function_names);

        free(res->colors);
        free(res->color_names);

        free(res->media);
        free(res->medium_names);

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
        res->num_functions = rescap.num_functions;
        res->num_colors = rescap.num_colors;
        res->num_surfaces = rescap.num_surfaces;
        res->num_media = rescap.num_media;
        res->num_boundary_layers = rescap.num_boundary_layers;

        mli_malloc(res->functions, struct mliFunc, res->num_functions);
        mli_malloc(res->function_names, struct mliName, res->num_functions);
        for (i = 0; i < res->num_functions; i++) {
                res->functions[i] = mliFunc_init();
                res->function_names[i] = mliName_init();
        }

        mli_malloc(res->colors, struct mliColor, res->num_colors);
        mli_malloc(res->color_names, struct mliName, res->num_colors);
        for (i = 0; i < res->num_colors; i++) {
                res->color_names[i] = mliName_init();
        }

        mli_malloc(res->media, struct mliMedium, res->num_media);
        mli_malloc(res->medium_names, struct mliName, res->num_media);
        for (i = 0; i < res->num_media; i++) {
                res->medium_names[i] = mliName_init();
        }

        mli_malloc(res->surfaces, struct mliSurface, res->num_surfaces);
        mli_malloc(res->surface_names, struct mliName, res->num_surfaces);
        for (i = 0; i < res->num_surfaces; i++) {
                res->surface_names[i] = mliName_init();
        }

        mli_malloc(
                res->boundary_layers,
                struct mliBoundaryLayer,
                res->num_boundary_layers);
        mli_malloc(
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

        fprintf(f, "    functions:\n");
        for (i = 0; i < res->num_functions; i++) {
                fprintf(f, "        % 3d, %-32s  ",
                        i, res->function_names[i].c_str);
                fprintf(f, "x: [% 1.3e, % 1.3e)\n",
                        res->functions[i].x[0],
                        res->functions[i].x[res->functions[i].num_points - 1]);
        }

        fprintf(f, "    media:\n");
        for (i = 0; i < res->num_media; i++) {
                fprintf(f, "        ");
                fprintf(f, "% 3d, %-32s  ", i, res->medium_names[i].c_str);
                fprintf(f, "absorbtion: % 3d, refraction: % 3d\n",
                        res->media[i].absorbtion, res->media[i].refraction);
        }

        fprintf(f, "    surfaces:\n");
        for (i = 0; i < res->num_surfaces; i++) {
                fprintf(f, "        % 3d, %-32s  ",
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
                        "specular-refl.: % 3d,  "
                        "diffuse-refl.: % 3d,  ",
                        res->surfaces[i].specular_reflection,
                        res->surfaces[i].diffuse_reflection);
                fprintf(f, "color: % 3d\n", res->surfaces[i].color);
        }

        fprintf(f, "    boundary_layers:\n");
        for (i = 0; i < res->num_boundary_layers; i++) {
                fprintf(f, "        ");
                fprintf(f, "% 3d, %-32s  ", i, res->boundary_layer_names[i].c_str);
                fprintf(f, "inner (medium: % 3d, surface : % 3d)",
                        res->boundary_layers[i].inner.medium,
                        res->boundary_layers[i].inner.surface);
                fprintf(f, ",  ");
                fprintf(f, "outer (medium: % 3d, surface : % 3d)",
                        res->boundary_layers[i].outer.medium,
                        res->boundary_layers[i].outer.surface);
                fprintf(f, "\n");
        }

        fprintf(f, "    colors:\n");
        for (i = 0; i < res->num_colors; i++) {
                fprintf(f, "        % 3d, %-32s  ",
                        i,
                        res->color_names[i].c_str);
                fprintf(f, "rgb/8bit [%3.0f, %3.0f, %3.0f]\n",
                        res->colors[i].r, res->colors[i].g, res->colors[i].b);
        }

        fprintf(f, "    default medium: %s\n",
                res->medium_names[res->default_medium].c_str);
}
