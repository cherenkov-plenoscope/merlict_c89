/* Copyright 2019 Sebastian Achim Mueller */
#ifndef MERLICT_MLISCENERY_VALID_H_
#define MERLICT_MLISCENERY_VALID_H_

#include "mliScenery.h"
#include "mliSceneryResources_valid.h"

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
                    scenery->resources.num_surfaces)
                        return 0;
                if (scenery->triangles_boundary_layers[i].outer.surface >=
                    scenery->resources.num_surfaces)
                        return 0;
        }
        return 1;
}

int _mliScenery_valid_spherical_cap_hex(const struct mliScenery *scenery)
{
        uint64_t i;
        for (i = 0; i < scenery->num_spherical_cap_hex; i++) {
                if (scenery->spherical_cap_hex_boundary_layers[i]
                            .inner.surface >= scenery->resources.num_surfaces)
                        return 0;
                if (scenery->spherical_cap_hex_boundary_layers[i]
                            .outer.surface >= scenery->resources.num_surfaces)
                        return 0;
        }
        return 1;
}

int _mliScenery_valid_spheres(const struct mliScenery *scenery)
{
        uint64_t i;
        for (i = 0; i < scenery->num_spheres; i++) {
                if (scenery->spheres_boundary_layers[i].inner.surface >=
                    scenery->resources.num_surfaces)
                        return 0;
                if (scenery->spheres_boundary_layers[i].outer.surface >=
                    scenery->resources.num_surfaces)
                        return 0;
        }
        return 1;
}

int _mliScenery_valid_cylinders(const struct mliScenery *scenery)
{
        uint64_t i;
        for (i = 0; i < scenery->num_cylinders; i++) {
                if (scenery->cylinders_boundary_layers[i].inner.surface >=
                    scenery->resources.num_surfaces)
                        return 0;
                if (scenery->cylinders_boundary_layers[i].outer.surface >=
                    scenery->resources.num_surfaces)
                        return 0;
        }
        return 1;
}

int _mliScenery_valid_hexagons(const struct mliScenery *scenery)
{
        uint64_t i;
        for (i = 0; i < scenery->num_hexagons; i++) {
                if (scenery->hexagons_boundary_layers[i].inner.surface >=
                    scenery->resources.num_surfaces)
                        return 0;
                if (scenery->hexagons_boundary_layers[i].outer.surface >=
                    scenery->resources.num_surfaces)
                        return 0;
        }
        return 1;
}

int _mliScenery_valid_bicircleplanes(const struct mliScenery *scenery)
{
        uint64_t i;
        for (i = 0; i < scenery->num_bicircleplanes; i++) {
                if (scenery->bicircleplanes_boundary_layers[i].inner.surface >=
                    scenery->resources.num_surfaces)
                        return 0;
                if (scenery->bicircleplanes_boundary_layers[i].outer.surface >=
                    scenery->resources.num_surfaces)
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
                    scenery->resources.num_surfaces)
                        return 0;
                if (scenery->discs_boundary_layers[i].outer.surface >=
                    scenery->resources.num_surfaces)
                        return 0;
                if (!mliDisc_is_valid(scenery->discs[i]))
                        return 0;
        }
        return 1;
}

int _mliScenery_valid_user_ids(const struct mliScenery *scenery)
{
        if (scenery->num_primitives != mliScenery_num_primitives(scenery))
                return 0;
        return 1;
}

int mliScenery_valid(const struct mliScenery *scenery)
{
        if (!mliSceneryResources_valid(&scenery->resources))
                return 0;
        if (scenery->default_medium >= scenery->resources.num_media)
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
        if (!_mliScenery_valid_user_ids(scenery))
                return 0;
        return 1;
}

#endif
