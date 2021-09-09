/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "mliMaterials_valid.h"
#include "mliObject.h"

int _mliMaterials_valid_colors(const struct mliMaterials *materials)
{
        uint32_t i = 0u;
        for (i = 0; i < materials->num_colors; i++) {
                chk_msg(
                        mliColor_is_valid_8bit_range(materials->colors[i]),
                        "Expected 0.0 <= color < 256.0.");
                chk_msg(
                        mliName_valid(&materials->color_names[i]),
                        "Name is invalid.");
        }
        return 1;
error:
        chk_eprintf("colors[%u] is invalid.", i);
        return 0;
}

int _mliMaterials_valid_functions(const struct mliMaterials *materials)
{
        uint32_t i = 0u;
        for (i = 0; i < materials->num_functions; i++) {
                chk_msg(
                        mliFunc_is_valid(&materials->functions[i]),
                        "Expected function to be valid.");
                chk_msg(
                        mliName_valid(&materials->function_names[i]),
                        "Name is invalid.");
        }
        return 1;
error:
        chk_eprintf("function[%u] is invalid.", i);
        return 0;
}

int _mliMaterials_valid_media(const struct mliMaterials *materials)
{
        uint32_t i = 0u;
        for (i = 0; i < materials->num_media; i++) {
                chk_msg(
                        materials->media[i].refraction <
                                materials->num_functions,
                        "Refraction-reference is invalid.");
                chk_msg(
                        materials->media[i].absorbtion <
                                materials->num_functions,
                        "Absorbtion-reference is invalid.");
                chk_msg(
                        mliName_valid(&materials->medium_names[i]),
                        "Name is invalid.");
        }
        return 1;
error:
        chk_eprintf("media[%u] is invalid.", i);
        return 0;
}

int _mliMaterials_valid_surfaces(const struct mliMaterials *materials)
{
        uint32_t i = 0u;
        for (i = 0; i < materials->num_surfaces; i++) {
                chk_msg(
                        (materials->surfaces[i].material ==
                         MLI_MATERIAL_PHONG) ||
                                (materials->surfaces[i].material ==
                                 MLI_MATERIAL_TRANSPARENT),
                        "Material-type is unknown.");
                chk_msg(
                        materials->surfaces[i].color < materials->num_colors,
                        "Color-reference is invalid.");
                chk_msg(
                        materials->surfaces[i].specular_reflection <
                                materials->num_functions,
                        "Specular-reflection-reference is invalid.");
                chk_msg(
                        materials->surfaces[i].diffuse_reflection <
                                materials->num_functions,
                        "Diffuse-reflection-reference is invalid.");
                chk_msg(
                        mliName_valid(&materials->surface_names[i]),
                        "Name is invalid.");
        }
        return 1;
error:
        chk_eprintf("surface[%u] is invalid.", i);
        return 0;
}

int _mliMaterials_valid_boundary_layers(const struct mliMaterials *materials)
{
        uint32_t i = 0u;
        for (i = 0; i < materials->num_boundary_layers; i++) {
                chk_msg(
                        materials->boundary_layers[i].inner.surface <
                                materials->num_surfaces,
                        "inner.surface is invalid.");
                chk_msg(
                        materials->boundary_layers[i].outer.surface <
                                materials->num_surfaces,
                        "outer.surface is invalid.");
                chk_msg(
                        materials->boundary_layers[i].inner.medium <
                                materials->num_media,
                        "inner.medium is invalid.");
                chk_msg(
                        materials->boundary_layers[i].outer.medium <
                                materials->num_media,
                        "outer.medium is invalid.");

                chk_msg(
                        mliName_valid(&materials->boundary_layer_names[i]),
                        "Name is invalid.");
        }
        return 1;
error:
        chk_eprintf("boundary_layers[%u] is invalid.", i);
        return 0;
}

int mliMaterials_valid(const struct mliMaterials *materials)
{
        chk_msg(
                materials->default_medium <= materials->num_media,
                "Expected default-medium to reference a valid medium.");
        chk_msg(
                _mliMaterials_valid_colors(materials),
                "Expected colors to be valid.");
        chk_msg(
                _mliMaterials_valid_functions(materials),
                "Expected functions to be valid.");
        chk_msg(
                _mliMaterials_valid_media(materials),
                "Expected media to be valid.");
        chk_msg(
                _mliMaterials_valid_surfaces(materials),
                "Expected surfaces to be valid.");
        chk_msg(
                _mliMaterials_valid_boundary_layers(materials),
                "Expected boundary_layers to be valid.");
        return 1;
error:
        return 0;
}
