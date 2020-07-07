/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "mliScenery_equal.h"

int _mliScenery_vertices_equal(
        const struct mliScenery *a,
        const struct mliScenery *b)
{
        uint64_t i;
        if (a->num_vertices != b->num_vertices)
                return 0;
        for (i = 0; i < a->num_vertices; i++) {
                if (!mliVec_is_equal(a->vertices[i], b->vertices[i]))
                        return 0;
        }
        return 1;
}

int _mliScenery_triangles_equal(
        const struct mliScenery *a,
        const struct mliScenery *b)
{
        uint64_t i;
        if (a->num_triangles != b->num_triangles)
                return 0;
        for (i = 0; i < a->num_triangles; i++) {
                if (!mliFace_is_equal(a->triangles[i], b->triangles[i]))
                        return 0;
                if (!mliBoundaryLayer_is_equal(
                            a->triangles_boundary_layers[i],
                            b->triangles_boundary_layers[i]))
                        return 0;
        }
        return 1;
}

int _mliScenery_spherical_cap_hex_equal(
        const struct mliScenery *a,
        const struct mliScenery *b)
{
        uint64_t i;
        if (a->num_spherical_cap_hex != b->num_spherical_cap_hex)
                return 0;
        for (i = 0; i < a->num_spherical_cap_hex; i++) {
                if (!mliSphericalCapHex_is_equal(
                            a->spherical_cap_hex[i], b->spherical_cap_hex[i]))
                        return 0;
                if (!mliBoundaryLayer_is_equal(
                            a->spherical_cap_hex_boundary_layers[i],
                            b->spherical_cap_hex_boundary_layers[i]))
                        return 0;
                if (!mliHomTraComp_is_equal(
                            a->spherical_cap_hex_T[i],
                            b->spherical_cap_hex_T[i]))
                        return 0;
        }
        return 1;
}

int _mliScenery_spheres_equal(
        const struct mliScenery *a,
        const struct mliScenery *b)
{
        uint64_t i;
        if (a->num_spheres != b->num_spheres)
                return 0;
        for (i = 0; i < a->num_spheres; i++) {
                if (!mliSphere_is_equal(a->spheres[i], b->spheres[i]))
                        return 0;
                if (!mliBoundaryLayer_is_equal(
                            a->spheres_boundary_layers[i],
                            b->spheres_boundary_layers[i]))
                        return 0;
                if (!mliHomTraComp_is_equal(a->spheres_T[i], b->spheres_T[i]))
                        return 0;
        }
        return 1;
}

int _mliScenery_cylinders_equal(
        const struct mliScenery *a,
        const struct mliScenery *b)
{
        uint64_t i;
        if (a->num_cylinders != b->num_cylinders)
                return 0;
        for (i = 0; i < a->num_cylinders; i++) {
                if (!mliCylinder_is_equal(a->cylinders[i], b->cylinders[i]))
                        return 0;
                if (!mliBoundaryLayer_is_equal(
                            a->cylinders_boundary_layers[i],
                            b->cylinders_boundary_layers[i]))
                        return 0;
                if (!mliHomTraComp_is_equal(
                            a->cylinders_T[i], b->cylinders_T[i]))
                        return 0;
        }
        return 1;
}

int _mliScenery_hexagons_equal(
        const struct mliScenery *a,
        const struct mliScenery *b)
{
        uint64_t i;
        if (a->num_hexagons != b->num_hexagons)
                return 0;
        for (i = 0; i < a->num_hexagons; i++) {
                if (!mliHexagon_is_equal(a->hexagons[i], b->hexagons[i]))
                        return 0;
                if (!mliBoundaryLayer_is_equal(
                            a->hexagons_boundary_layers[i],
                            b->hexagons_boundary_layers[i]))
                        return 0;
                if (!mliHomTraComp_is_equal(a->hexagons_T[i], b->hexagons_T[i]))
                        return 0;
        }
        return 1;
}

int _mliScenery_bicircleplanes_equal(
        const struct mliScenery *a,
        const struct mliScenery *b)
{
        uint64_t i;
        if (a->num_bicircleplanes != b->num_bicircleplanes)
                return 0;
        for (i = 0; i < a->num_bicircleplanes; i++) {
                if (!mliBiCirclePlane_is_equal(
                            a->bicircleplanes[i], b->bicircleplanes[i]))
                        return 0;
                if (!mliBoundaryLayer_is_equal(
                            a->bicircleplanes_boundary_layers[i],
                            b->bicircleplanes_boundary_layers[i]))
                        return 0;
                if (!mliHomTraComp_is_equal(
                            a->bicircleplanes_T[i], b->bicircleplanes_T[i]))
                        return 0;
        }
        return 1;
}

int _mliScenery_discs_equal(
        const struct mliScenery *a,
        const struct mliScenery *b)
{
        uint64_t i;
        if (a->num_discs != b->num_discs)
                return 0;
        for (i = 0; i < a->num_discs; i++) {
                if (!mliDisc_is_equal(a->discs[i], b->discs[i]))
                        return 0;
                if (!mliBoundaryLayer_is_equal(
                            a->discs_boundary_layers[i],
                            b->discs_boundary_layers[i]))
                        return 0;
                if (!mliHomTraComp_is_equal(a->discs_T[i], b->discs_T[i]))
                        return 0;
        }
        return 1;
}

int _mliScenery_user_ids_equal(
        const struct mliScenery *a,
        const struct mliScenery *b)
{
        uint64_t i;
        if (a->num_primitives != b->num_primitives)
                return 0;
        for (i = 0; i < a->num_primitives; i++) {
                if (a->user_ids[i] != b->user_ids[i])
                        return 0;
        }
        return 1;
}

int mliScenery_is_equal(const struct mliScenery *a, const struct mliScenery *b)
{
        if (!mliSceneryResources_equal(&a->resources, &b->resources))
                return 0;
        if (a->default_medium != b->default_medium)
                return 0;
        if (!_mliScenery_triangles_equal(a, b))
                return 0;
        if (!_mliScenery_spherical_cap_hex_equal(a, b))
                return 0;
        if (!_mliScenery_spheres_equal(a, b))
                return 0;
        if (!_mliScenery_cylinders_equal(a, b))
                return 0;
        if (!_mliScenery_hexagons_equal(a, b))
                return 0;
        if (!_mliScenery_bicircleplanes_equal(a, b))
                return 0;
        if (!_mliScenery_discs_equal(a, b))
                return 0;
        if (!_mliScenery_user_ids_equal(a, b))
                return 0;
        return 1;
}
