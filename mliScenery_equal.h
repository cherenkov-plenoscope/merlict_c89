/* Copyright 2019 Sebastian Achim Mueller */
#ifndef MERLICT_MLISCENERY_EQUAL_H_
#define MERLICT_MLISCENERY_EQUAL_H_

#include "mliScenery.h"

int mliScenery_is_equal(const struct mliScenery *a, const struct mliScenery *b)
{
    uint64_t i;
    if (a->num_functions != b->num_functions) return 0;
    if (a->num_colors != b->num_colors ) return 0;
    if (a->num_surfaces != b->num_surfaces ) return 0;
    if (a->num_vertices != b->num_vertices ) return 0;
    if (a->num_triangles != b->num_triangles ) return 0;
    if (a->num_spherical_cap_hex != b->num_spherical_cap_hex ) return 0;
    if (a->num_spheres != b->num_spheres) return 0;
    if (a->num_cylinders != b->num_cylinders) return 0;
    if (a->num_hexagons != b->num_hexagons) return 0;
    if (a->num_bicircleplanes != b->num_bicircleplanes) return 0;
    if (a->num_discs != b->num_discs) return 0;

    for (i = 0; i < a->num_functions; i++) {
        if (!mliFunc_is_equal(a->functions[i], b->functions[i])) return 0;}
    for (i = 0; i < a->num_colors; i++) {
        if (!mliColor_is_equal(a->colors[i], b->colors[i])) return 0;}
    for (i = 0; i < a->num_surfaces; i++) {
        if (!mliSurface_is_equal(a->surfaces[i], b->surfaces[i])) return 0;}
    for (i = 0; i < a->num_vertices; i++) {
        if (!mliVec_is_equal(a->vertices[i], b->vertices[i])) return 0;}
    for (i = 0; i < a->num_triangles; i++) {
        if (!mliFace_is_equal(a->triangles[i], b->triangles[i]))
            return 0;
        if (!mliSurfaces_is_equal(
            a->triangles_surfaces[i],
            b->triangles_surfaces[i]))
            return 0;
    }
    for (i = 0; i < a->num_spherical_cap_hex; i++) {
        if (!mliSphericalCapHex_is_equal(
                a->spherical_cap_hex[i],
                b->spherical_cap_hex[i]))
            return 0;
        if (!mliSurfaces_is_equal(
                a->spherical_cap_hex_surfaces[i],
                b->spherical_cap_hex_surfaces[i]))
            return 0;
        if(!mliHomTraComp_is_equal(
            a->spherical_cap_hex_T[i],
            b->spherical_cap_hex_T[i]))
            return 0;
    }
    for (i = 0; i < a->num_spheres; i++) {
        if (!mliSphere_is_equal(a->spheres[i], b->spheres[i]))
            return 0;
        if (!mliSurfaces_is_equal(
                a->spheres_surfaces[i],
                b->spheres_surfaces[i]))
            return 0;
        if(!mliHomTraComp_is_equal(
            a->spheres_T[i],
            b->spheres_T[i]))
            return 0;
    }
    for (i = 0; i < a->num_cylinders; i++) {
        if (!mliCylinder_is_equal(a->cylinders[i], b->cylinders[i]))
            return 0;
        if (!mliSurfaces_is_equal(
                a->cylinders_surfaces[i],
                b->cylinders_surfaces[i]))
            return 0;
        if(!mliHomTraComp_is_equal(
            a->cylinders_T[i],
            b->cylinders_T[i]))
            return 0;
    }
    for (i = 0; i < a->num_hexagons; i++) {
        if (!mliHexagon_is_equal(a->hexagons[i], b->hexagons[i]))
            return 0;
        if (!mliSurfaces_is_equal(
                a->hexagons_surfaces[i],
                b->hexagons_surfaces[i]))
            return 0;
        if(!mliHomTraComp_is_equal(
            a->hexagons_T[i],
            b->hexagons_T[i]))
            return 0;
    }
    for (i = 0; i < a->num_bicircleplanes; i++) {
        if (!mliBiCirclePlane_is_equal(
            a->bicircleplanes[i],
            b->bicircleplanes[i])
        ) {
            return 0;
        }
        if (!mliSurfaces_is_equal(
                a->bicircleplanes_surfaces[i],
                b->bicircleplanes_surfaces[i]))
            return 0;
        if(!mliHomTraComp_is_equal(
            a->bicircleplanes_T[i],
            b->bicircleplanes_T[i]))
            return 0;
    }
    for (i = 0; i < a->num_discs; i++) {
        if (!mliDisc_is_equal(a->discs[i], b->discs[i])) {
            return 0;
        }
        if (!mliSurfaces_is_equal(a->discs_surfaces[i], b->discs_surfaces[i]))
            return 0;
        if(!mliHomTraComp_is_equal(a->discs_T[i], b->discs_T[i]))
            return 0;
    }
    return 1;
}

#endif
