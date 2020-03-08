/* Copyright 2019 Sebastian Achim Mueller */
#ifndef MERLICT_MLISCENERY_VALID_H_
#define MERLICT_MLISCENERY_VALID_H_

#include "mliScenery.h"

int _mliScenery_valid_media(const struct mliScenery *scenery)
{
        uint64_t i;
        for (i = 0; i < scenery->num_media; i++) {
                if (scenery->media[i].refraction >= scenery->num_functions)
                        return 0;
                if (scenery->media[i].refraction >= scenery->num_functions)
                        return 0;
        }
        return 1;
}

int _mliScenery_valid_surfaces(const struct mliScenery *scenery)
{
        uint64_t i;
        for (i = 0; i < scenery->num_surfaces; i++) {
                switch (scenery->surfaces[i].material) {
                        case MLI_MATERIAL_PHONG: break;
                        case MLI_MATERIAL_TRANSPARENT: break;
                        default: return 0; break;
                }
                if (scenery->surfaces[i].color >= scenery->num_colors)
                        return 0;
                if (scenery->surfaces[i].boundary_layer_specular_reflection >=
                        scenery->num_functions)
                        return 0;
                if (scenery->surfaces[i].boundary_layer_diffuse_reflection >=
                        scenery->num_functions)
                        return 0;
        }
        return 1;
}

int _mliScenery_valid_triangles(const struct mliScenery *scenery)
{
        uint64_t i;
        for (i = 0; i < scenery->num_triangles; i++) {
                if (scenery->triangles[i].a >= scenery->num_vertices)
                        return 0;
                if (scenery->triangles[i].b >= scenery->num_vertices)
                        return 0;
                if (scenery->triangles[i].c >= scenery->num_vertices)
                        return 0;
                if (scenery->triangles_boundary_layers[i].inner.surface >=
                        scenery->num_surfaces)
                        return 0;
                if (scenery->triangles_boundary_layers[i].outer.surface >=
                        scenery->num_surfaces)
                        return 0;}
        return 1;
}

int _mliScenery_valid_spherical_cap_hex(const struct mliScenery *scenery)
{
        uint64_t i;
        for (i = 0; i < scenery->num_spherical_cap_hex; i++) {
                if (scenery->
                        spherical_cap_hex_boundary_layers[i].inner.surface >=
                        scenery->num_surfaces)
                        return 0;
                if (scenery->
                        spherical_cap_hex_boundary_layers[i].outer.surface >=
                        scenery->num_surfaces)
                        return 0;
        }
        return 1;
}

int _mliScenery_valid_spheres(const struct mliScenery *scenery)
{
        uint64_t i;
        for (i = 0; i < scenery->num_spheres; i++) {
                if (scenery->spheres_boundary_layers[i].inner.surface >=
                        scenery->num_surfaces)
                        return 0;
                if (scenery->spheres_boundary_layers[i].outer.surface >=
                        scenery->num_surfaces)
                        return 0;
        }
        return 1;
}

int _mliScenery_valid_cylinders(const struct mliScenery *scenery)
{
        uint64_t i;
        for (i = 0; i < scenery->num_cylinders; i++) {
                if (scenery->cylinders_boundary_layers[i].inner.surface >=
                        scenery->num_surfaces)
                        return 0;
                if (scenery->cylinders_boundary_layers[i].outer.surface >=
                        scenery->num_surfaces)
                        return 0;
        }
        return 1;
}

int _mliScenery_valid_hexagons(const struct mliScenery *scenery)
{
        uint64_t i;
        for (i = 0; i < scenery->num_hexagons; i++) {
                if (scenery->hexagons_boundary_layers[i].inner.surface >=
                        scenery->num_surfaces)
                        return 0;
                if (scenery->hexagons_boundary_layers[i].outer.surface >=
                        scenery->num_surfaces)
                        return 0;
        }
        return 1;
}

int _mliScenery_valid_bicircleplanes(const struct mliScenery *scenery)
{
        uint64_t i;
        for (i = 0; i < scenery->num_bicircleplanes; i++) {
                if (scenery->bicircleplanes_boundary_layers[i].inner.surface >=
                        scenery->num_surfaces)
                        return 0;
                if (scenery->bicircleplanes_boundary_layers[i].outer.surface >=
                        scenery->num_surfaces)
                        return 0;
                if (!mliBiCirclePlane_is_valid(scenery->bicircleplanes[i]))
                        return 0;
        }
        return 1;
}

int _mliScenery_valid_disc(const struct mliScenery *scenery)
{
        uint64_t i;
        for (i = 0; i < scenery->num_discs; i++) {
                if (scenery->discs_boundary_layers[i].inner.surface >=
                        scenery->num_surfaces)
                        return 0;
                if (scenery->discs_boundary_layers[i].outer.surface >=
                        scenery->num_surfaces)
                        return 0;
                if (!mliDisc_is_valid(scenery->discs[i]))
                        return 0;
        }
        return 1;
}

int mliScenery_valid(const struct mliScenery *scenery)
{
        if (!_mliScenery_valid_media(scenery))
                return 0;
        if (!_mliScenery_valid_surfaces(scenery))
                return 0;
        if (!_mliScenery_valid_triangles(scenery))
                return 0;
        if (!_mliScenery_valid_spherical_cap_hex(scenery))
                return 0;
        if (!_mliScenery_valid_spheres(scenery))
                return 0;
        if (!_mliScenery_valid_cylinders(scenery))
                return 0;
        if (!_mliScenery_valid_hexagons(scenery))
                return 0;
        if (!_mliScenery_valid_bicircleplanes(scenery))
                return 0;
        if (!_mliScenery_valid_disc(scenery))
                return 0;
        return 1;
}

#endif
