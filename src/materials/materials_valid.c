/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "materials_valid.h"
#include "../object/object.h"
#include "../chk/chk.h"

int mli_Materials_valid_media(const struct mli_Materials *materials)
{
        uint32_t i = 0u;
        for (i = 0; i < materials->num_media; i++) {
                chk_msg(mli_Func_is_valid(&materials->media[i].refraction),
                        "Expected refraction of medium to be valid.");
                chk_msg(mli_Func_is_valid(&materials->media[i].absorbtion),
                        "Expected refraction of medium to be valid.");
        }
        return 1;
chk_error:
        fprintf(stderr, "In materials.media[%u]\n", i);
        return 0;
}

int mli_Materials_valid_spectra(const struct mli_Materials *materials)
{
        uint32_t i = 0u;
        for (i = 0; i < materials->spectra.size; i++) {
                chk_msg(mli_Func_is_valid(
                                &materials->spectra.array[i].spectrum),
                        "Expected spectrum function to be valid.");
        }
        return 1;
chk_error:
        fprintf(stderr, "In materials.spectra[%u]\n", i);
        return 0;
}

int mli_Materials_valid_default_medium(const struct mli_Materials *materials)
{
        chk_msg(materials->default_refraction < materials->spectra.size,
                "Expected default_refraction < spectra.size.");
        chk_msg(materials->default_absorbtion < materials->spectra.size,
                "Expected default_absorbtion < spectra.size.");
        return 1;
chk_error:
        return 0;
}

int mli_Materials_valid_surfaces(const struct mli_Materials *materials)
{
        uint32_t i = 0u;
        for (i = 0; i < materials->num_surfaces; i++) {
                chk_msg((materials->surfaces[i].material ==
                         MLI_SURFACE_PHONG) ||
                                (materials->surfaces[i].material ==
                                 MLI_SURFACE_TRANSPARENT),
                        "Material-type is unknown.");
                chk_msg(mli_Func_is_valid(
                                &materials->surfaces[i].specular_reflection),
                        "Expected specular_reflection of surface to be valid.");
                chk_msg(mli_Func_is_valid(
                                &materials->surfaces[i].diffuse_reflection),
                        "Expected diffuse_reflection of surface to be valid.");
        }
        return 1;
chk_error:
        fprintf(stderr, "In materials.surface[%u]\n", i);
        return 0;
}

int mli_Materials_valid_boundary_layers(const struct mli_Materials *materials)
{
        uint32_t i = 0u;
        for (i = 0; i < materials->num_boundary_layers; i++) {
                chk_msg(materials->boundary_layers[i].inner.surface <
                                materials->num_surfaces,
                        "inner.surface is invalid.");
                chk_msg(materials->boundary_layers[i].outer.surface <
                                materials->num_surfaces,
                        "outer.surface is invalid.");
                chk_msg(materials->boundary_layers[i].inner.medium <
                                materials->num_media,
                        "inner.medium is invalid.");
                chk_msg(materials->boundary_layers[i].outer.medium <
                                materials->num_media,
                        "outer.medium is invalid.");

                chk_msg(mli_String_valid(
                                &materials->boundary_layer_names[i], 1),
                        "materials.boundary_layer_name is invalid.");
        }
        return 1;
chk_error:
        fprintf(stderr, "In materials.boundary_layers[%u]\n", i);
        return 0;
}

int mli_Materials_valid(const struct mli_Materials *materials)
{
        chk_msg(materials->default_medium <= materials->num_media,
                "Expected default-medium to reference a valid medium.");
        chk_msg(mli_Materials_valid_spectra(materials),
                "Expected spectra to be valid.");
        chk_msg(mli_Materials_valid_default_medium(materials),
                "Expected default medium to be valid.");
        chk_msg(mli_Materials_valid_media(materials),
                "Expected media to be valid.");
        chk_msg(mli_Materials_valid_surfaces(materials),
                "Expected surfaces to be valid.");
        chk_msg(mli_Materials_valid_boundary_layers(materials),
                "Expected boundary_layers to be valid.");
        return 1;
chk_error:
        return 0;
}
