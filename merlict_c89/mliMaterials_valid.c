/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "mliMaterials_valid.h"
#include "mliObject.h"

int _mliMaterials_valid_colors(const struct mliMaterials *materials)
{
        uint32_t i;
        for (i = 0; i < materials->num_colors; i++) {
                mli_check(mliColor_is_valid_8bit_range(materials->colors[i]),
                        "Expected 0.0 <= color < 256.0.");
        }
        return 1;
error:
        mli_log_err_vargs(("colors[%u] is invalid.", i));
        return 0;
}

int _mliMaterials_valid_functions(const struct mliMaterials *materials)
{
        uint32_t i;
        for (i = 0; i < materials->num_functions; i++) {
                mli_check(mliFunc_is_valid(&materials->functions[i]),
                        "Expected function to be valid.");
        }
        return 1;
error:
        mli_log_err_vargs(("function[%u] is invalid.", i));
        return 0;
}

int _mliMaterials_valid_media(const struct mliMaterials *res)
{
        uint32_t i;
        for (i = 0; i < res->num_media; i++) {
                mli_check(res->media[i].refraction < res->num_functions,
                        "Refraction-reference is invalid.");
                mli_check(res->media[i].absorbtion < res->num_functions,
                        "Absorbtion-reference is invalid.");
        }
        return 1;
error:
        mli_log_err_vargs(("media[%u] is invalid.", i));
        return 0;
}

int _mliMaterials_valid_surfaces(const struct mliMaterials *res)
{
        uint32_t i;
        for (i = 0; i < res->num_surfaces; i++) {
                mli_check(
                        (res->surfaces[i].material == MLI_MATERIAL_PHONG)
                        ||
                        (res->surfaces[i].material == MLI_MATERIAL_TRANSPARENT),
                        "Material-type is unknown."
                );
                mli_check(
                        res->surfaces[i].color < res->num_colors,
                        "Color-reference is invalid.");
                mli_check(
                        res->surfaces[i].specular_reflection < res->num_functions,
                        "Specular-reflection-reference is invalid.");
                mli_check(
                        res->surfaces[i].diffuse_reflection < res->num_functions,
                        "Diffuse-reflection-reference is invalid.");
        }
        return 1;
error:
        mli_log_err_vargs(("surface[%u] is invalid.", i));
        return 0;
}

int mliMaterials_valid(const struct mliMaterials *res)
{
        mli_check(
                res->default_medium <= res->num_media,
                "Expected default-medium to reference a valid medium.");
        mli_check(
                _mliMaterials_valid_colors(res),
                "Expected colors to be valid.");
        mli_check(
                _mliMaterials_valid_functions(res),
                "Expected functions to be valid.");

        mli_check(
                _mliMaterials_valid_media(res),
                "Expected media to be valid.");

        mli_check(
                _mliMaterials_valid_surfaces(res),
                "Expected surfaces to be valid.");

        return 1;
error:
        return 0;
}
