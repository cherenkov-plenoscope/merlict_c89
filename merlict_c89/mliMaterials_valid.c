/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "mliMaterials_valid.h"
#include "mliObject.h"

int _mliMaterials_valid_colors(const struct mliMaterials *materials)
{
        uint32_t i = 0u;
        for (i = 0; i < materials->num_colors; i++) {
                mli_check(mliColor_is_valid_8bit_range(materials->colors[i]),
                        "Expected 0.0 <= color < 256.0.");
                mli_check(mliName_valid(&materials->color_names[i]),
                        "Name is invalid.");
        }
        return 1;
error:
        mli_log_err_vargs(("colors[%u] is invalid.", i));
        return 0;
}

int _mliMaterials_valid_functions(const struct mliMaterials *materials)
{
        uint32_t i = 0u;
        for (i = 0; i < materials->num_functions; i++) {
                mli_check(mliFunc_is_valid(&materials->functions[i]),
                        "Expected function to be valid.");
                mli_check(mliName_valid(&materials->function_names[i]),
                        "Name is invalid.");
        }
        return 1;
error:
        mli_log_err_vargs(("function[%u] is invalid.", i));
        return 0;
}

int _mliMaterials_valid_media(const struct mliMaterials *materials)
{
        uint32_t i = 0u;
        for (i = 0; i < materials->num_media; i++) {
                mli_check(
                        materials->media[i].refraction <
                        materials->num_functions,
                        "Refraction-reference is invalid.");
                mli_check(
                        materials->media[i].absorbtion <
                        materials->num_functions,
                        "Absorbtion-reference is invalid.");
                mli_check(mliName_valid(&materials->medium_names[i]),
                        "Name is invalid.");
        }
        return 1;
error:
        mli_log_err_vargs(("media[%u] is invalid.", i));
        return 0;
}

int _mliMaterials_valid_surfaces(const struct mliMaterials *materials)
{
        uint32_t i = 0u;
        for (i = 0; i < materials->num_surfaces; i++) {
                mli_check(
                        (materials->surfaces[i].material ==
                                MLI_MATERIAL_PHONG)
                        ||
                        (materials->surfaces[i].material ==
                                MLI_MATERIAL_TRANSPARENT),
                        "Material-type is unknown."
                );
                mli_check(
                        materials->surfaces[i].color < materials->num_colors,
                        "Color-reference is invalid.");
                mli_check(
                        materials->surfaces[i].specular_reflection <
                        materials->num_functions,
                        "Specular-reflection-reference is invalid.");
                mli_check(
                        materials->surfaces[i].diffuse_reflection <
                        materials->num_functions,
                        "Diffuse-reflection-reference is invalid.");
                mli_check(mliName_valid(&materials->surface_names[i]),
                        "Name is invalid.");
        }
        return 1;
error:
        mli_log_err_vargs(("surface[%u] is invalid.", i));
        return 0;
}

int mliMaterials_valid(const struct mliMaterials *materials)
{
        mli_check(
                materials->default_medium <= materials->num_media,
                "Expected default-medium to reference a valid medium.");
        mli_check(
                _mliMaterials_valid_colors(materials),
                "Expected colors to be valid.");
        mli_check(
                _mliMaterials_valid_functions(materials),
                "Expected functions to be valid.");
        mli_check(
                _mliMaterials_valid_media(materials),
                "Expected media to be valid.");
        mli_check(
                _mliMaterials_valid_surfaces(materials),
                "Expected surfaces to be valid.");
        return 1;
error:
        return 0;
}
