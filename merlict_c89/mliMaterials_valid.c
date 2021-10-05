/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "mliMaterials_valid.h"
#include "mliObject.h"

int _mliMaterials_valid_media(const struct mliMaterials *materials)
{
        uint32_t i = 0u;
        for (i = 0; i < materials->num_media; i++) {
                chk_msg(mliFunc_is_valid(&materials->media[i].refraction),
                        "Expected refraction of medium to be valid.");
                chk_msg(mliFunc_is_valid(&materials->media[i].absorbtion),
                        "Expected refraction of medium to be valid.");
        }
        return 1;
error:
        fprintf(stderr, "In materials.media[%u]\n", i);
        return 0;
}

int _mliMaterials_valid_surfaces(const struct mliMaterials *materials)
{
        uint32_t i = 0u;
        for (i = 0; i < materials->num_surfaces; i++) {
                chk_msg((materials->surfaces[i].material ==
                         MLI_MATERIAL_PHONG) ||
                                (materials->surfaces[i].material ==
                                 MLI_MATERIAL_TRANSPARENT),
                        "Material-type is unknown.");
                chk_msg(mliFunc_is_valid(&materials->surfaces[i].specular_reflection),
                        "Expected specular_reflection of surface to be valid.");
                chk_msg(mliFunc_is_valid(&materials->surfaces[i].diffuse_reflection),
                        "Expected diffuse_reflection of surface to be valid.");
                chk_msg(mliColor_is_valid_8bit_range(materials->surfaces[i].color),
                        "Expected 0.0 <= color < 256.0.");
        }
        return 1;
error:
        fprintf(stderr, "In materials.surface[%u]\n", i);
        return 0;
}

int _mliMaterials_valid_boundary_layers(const struct mliMaterials *materials)
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

                chk_msg(mliName_valid(&materials->boundary_layer_names[i]),
                        "Name is invalid.");
        }
        return 1;
error:
        fprintf(stderr, "In materials.boundary_layers[%u]\n", i);
        return 0;
}

int mliMaterials_valid(const struct mliMaterials *materials)
{
        chk_msg(materials->default_medium <= materials->num_media,
                "Expected default-medium to reference a valid medium.");
        chk_msg(_mliMaterials_valid_media(materials),
                "Expected media to be valid.");
        chk_msg(_mliMaterials_valid_surfaces(materials),
                "Expected surfaces to be valid.");
        chk_msg(_mliMaterials_valid_boundary_layers(materials),
                "Expected boundary_layers to be valid.");
        return 1;
error:
        return 0;
}
