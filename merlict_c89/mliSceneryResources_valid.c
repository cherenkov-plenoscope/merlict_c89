/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "mliSceneryResources_valid.h"
#include "mliObject.h"

int _mliSceneryResources_valid_media(const struct mliSceneryResources *res)
{
        uint64_t i;
        for (i = 0; i < res->num_media; i++) {
                if (res->media[i].refraction >= res->num_functions)
                        return 0;
                if (res->media[i].refraction >= res->num_functions)
                        return 0;
        }
        return 1;
}

int _mliSceneryResources_valid_surfaces(const struct mliSceneryResources *res)
{
        uint64_t i;
        for (i = 0; i < res->num_surfaces; i++) {
                switch (res->surfaces[i].material) {
                case MLI_MATERIAL_PHONG:
                        break;
                case MLI_MATERIAL_TRANSPARENT:
                        break;
                default:
                        return 0;
                        break;
                }
                if (res->surfaces[i].color >= res->num_colors)
                        return 0;
                if (res->surfaces[i].specular_reflection >= res->num_functions)
                        return 0;
                if (res->surfaces[i].diffuse_reflection >= res->num_functions)
                        return 0;
        }
        return 1;
}

int mliSceneryResources_valid(const struct mliSceneryResources *res)
{
        mli_check(
                res->default_medium <= res->num_media,
                "Expected default-medium to reference a valid medium.");

        mli_check(
                _mliSceneryResources_valid_media(res),
                "Expected media to be valid.");

        mli_check(
                _mliSceneryResources_valid_surfaces(res),
                "Expected surfaces to be valid.");

        return 1;
error:
        return 0;
}
