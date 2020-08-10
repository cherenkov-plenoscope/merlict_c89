/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "mliScenery.h"

struct mliSceneryCapacity mliSceneryCapacity_init(void)
{
        struct mliSceneryCapacity cap;
        cap.num_vertices = 0u;
        cap.num_triangles = 0u;
        cap.num_spherical_cap_hex = 0u;
        cap.num_spheres = 0u;
        cap.num_cylinders = 0u;
        cap.num_hexagons = 0u;
        cap.num_bicircleplanes = 0u;
        cap.num_discs = 0u;
        return cap;
}

struct mliIndexStarts mliScenery_index_starts(const struct mliScenery *scenery)
{
        struct mliIndexStarts s;
        s.triangles = 0;
        s.spherical_cap_hex = scenery->num_triangles;
        s.spheres = s.spherical_cap_hex + scenery->num_spherical_cap_hex;
        s.cylinders = s.spheres + scenery->num_spheres;
        s.hexagons = s.cylinders + scenery->num_cylinders;
        s.bicircleplanes = s.hexagons + scenery->num_hexagons;
        s.discs = s.bicircleplanes + scenery->num_bicircleplanes;
        s._next = s.discs + scenery->num_discs;
        return s;
}

uint64_t mliScenery_num_primitives(const struct mliScenery *scenery)
{
        struct mliIndexStarts starts = mliScenery_index_starts(scenery);
        return starts._next;
}

struct mliScenery mliScenery_init(void)
{
        struct mliScenery s;
        s.resources = mliSceneryResources_init();
        s.default_medium = 0u;

        s.num_vertices = 0u;
        s.vertices = NULL;

        s.num_triangles = 0u;
        s.triangles = NULL;
        s.triangles_boundary_layers = NULL;

        s.num_spherical_cap_hex = 0u;
        s.spherical_cap_hex = NULL;
        s.spherical_cap_hex_boundary_layers = NULL;
        s.spherical_cap_hex_T = NULL;

        s.num_spheres = 0u;
        s.spheres = NULL;
        s.spheres_boundary_layers = NULL;
        s.spheres_T = NULL;

        s.num_cylinders = 0u;
        s.cylinders = NULL;
        s.cylinders_boundary_layers = NULL;
        s.cylinders_T = NULL;

        s.num_hexagons = 0u;
        s.hexagons = NULL;
        s.hexagons_boundary_layers = NULL;
        s.hexagons_T = NULL;

        s.num_bicircleplanes = 0u;
        s.bicircleplanes = NULL;
        s.bicircleplanes_boundary_layers = NULL;
        s.bicircleplanes_T = NULL;

        s.num_discs = 0u;
        s.discs = NULL;
        s.discs_boundary_layers = NULL;
        s.discs_T = NULL;

        s.num_primitives = 0;
        s.user_ids = NULL;
        return s;
}

void _mliScenery_free_vertices_and_triangles(struct mliScenery *scenery)
{
        free(scenery->vertices);
        free(scenery->triangles);
        free(scenery->triangles_boundary_layers);
}

void _mliScenery_free_spherical_cap_hex(struct mliScenery *scenery)
{
        free(scenery->spherical_cap_hex);
        free(scenery->spherical_cap_hex_boundary_layers);
        free(scenery->spherical_cap_hex_T);
}

void _mliScenery_free_spheres(struct mliScenery *scenery)
{
        free(scenery->spheres);
        free(scenery->spheres_boundary_layers);
        free(scenery->spheres_T);
}

void _mliScenery_free_cylinders(struct mliScenery *scenery)
{
        free(scenery->cylinders);
        free(scenery->cylinders_boundary_layers);
        free(scenery->cylinders_T);
}

void _mliScenery_free_hexagons(struct mliScenery *scenery)
{
        free(scenery->hexagons);
        free(scenery->hexagons_boundary_layers);
        free(scenery->hexagons_T);
}

void _mliScenery_free_bicircleplanes(struct mliScenery *scenery)
{
        free(scenery->bicircleplanes);
        free(scenery->bicircleplanes_boundary_layers);
        free(scenery->bicircleplanes_T);
}

void _mliScenery_free_discs(struct mliScenery *scenery)
{
        free(scenery->discs);
        free(scenery->discs_boundary_layers);
        free(scenery->discs_T);
}

void _mliScenery_free_user_ids(struct mliScenery *scenery)
{
        free(scenery->user_ids);
}

void mliScenery_free(struct mliScenery *scenery)
{
        mliSceneryResources_free(&scenery->resources);
        _mliScenery_free_vertices_and_triangles(scenery);
        _mliScenery_free_spherical_cap_hex(scenery);
        _mliScenery_free_spheres(scenery);
        _mliScenery_free_cylinders(scenery);
        _mliScenery_free_hexagons(scenery);
        _mliScenery_free_bicircleplanes(scenery);
        _mliScenery_free_discs(scenery);

        _mliScenery_free_user_ids(scenery);
        (*scenery) = mliScenery_init();
}

int _mliScenery_malloc_vertices_and_triangles(struct mliScenery *s)
{
        mli_malloc(s->vertices, struct mliVec, s->num_vertices);
        mli_malloc(s->triangles, struct mliFace, s->num_triangles);
        mli_malloc(
                s->triangles_boundary_layers,
                struct mliBoundaryLayer,
                s->num_triangles);
        return 1;
error:
        return 0;
}

int _mliScenery_malloc_spherical_cap_hex(struct mliScenery *s)
{
        mli_malloc(
                s->spherical_cap_hex,
                struct mliSphericalCapHex,
                s->num_spherical_cap_hex);
        mli_malloc(
                s->spherical_cap_hex_boundary_layers,
                struct mliBoundaryLayer,
                s->num_spherical_cap_hex);
        mli_malloc(
                s->spherical_cap_hex_T,
                struct mliHomTraComp,
                s->num_spherical_cap_hex);
        return 1;
error:
        return 0;
}

int _mliScenery_malloc_spheres(struct mliScenery *s)
{
        mli_malloc(s->spheres, struct mliSphere, s->num_spheres);
        mli_malloc(
                s->spheres_boundary_layers,
                struct mliBoundaryLayer,
                s->num_spheres);
        mli_malloc(s->spheres_T, struct mliHomTraComp, s->num_spheres);
        return 1;
error:
        return 0;
}

int _mliScenery_malloc_cylinders(struct mliScenery *s)
{
        mli_malloc(s->cylinders, struct mliCylinder, s->num_cylinders);
        mli_malloc(
                s->cylinders_boundary_layers,
                struct mliBoundaryLayer,
                s->num_cylinders);
        mli_malloc(s->cylinders_T, struct mliHomTraComp, s->num_cylinders);
        return 1;
error:
        return 0;
}

int _mliScenery_malloc_hexagons(struct mliScenery *s)
{
        mli_malloc(s->hexagons, struct mliHexagon, s->num_hexagons);
        mli_malloc(
                s->hexagons_boundary_layers,
                struct mliBoundaryLayer,
                s->num_hexagons);
        mli_malloc(s->hexagons_T, struct mliHomTraComp, s->num_hexagons);
        return 1;
error:
        return 0;
}

int _mliScenery_malloc_bicircleplane(struct mliScenery *s)
{
        mli_malloc(
                s->bicircleplanes,
                struct mliBiCirclePlane,
                s->num_bicircleplanes);
        mli_malloc(
                s->bicircleplanes_boundary_layers,
                struct mliBoundaryLayer,
                s->num_bicircleplanes);
        mli_malloc(
                s->bicircleplanes_T,
                struct mliHomTraComp,
                s->num_bicircleplanes);
        return 1;
error:
        return 0;
}

int _mliScenery_malloc_discs(struct mliScenery *s)
{
        mli_malloc(s->discs, struct mliDisc, s->num_discs);
        mli_malloc(
                s->discs_boundary_layers,
                struct mliBoundaryLayer,
                s->num_discs);
        mli_malloc(s->discs_T, struct mliHomTraComp, s->num_discs);
        return 1;
error:
        return 0;
}

int _mliScenery_malloc_user_ids(struct mliScenery *s)
{
        uint64_t i;
        mli_malloc(s->user_ids, uint32_t, s->num_primitives);
        for (i = 0; i < s->num_primitives; i++) {
                s->user_ids[i] = 0u;
        }
        return 1;
error:
        return 0;
}

int mliScenery_malloc(struct mliScenery *s, const struct mliSceneryCapacity c)
{
        mliScenery_free(s);
        s->num_vertices = c.num_vertices;
        s->num_triangles = c.num_triangles;
        s->num_spherical_cap_hex = c.num_spherical_cap_hex;
        s->num_spheres = c.num_spheres;
        s->num_cylinders = c.num_cylinders;
        s->num_hexagons = c.num_hexagons;
        s->num_bicircleplanes = c.num_bicircleplanes;
        s->num_discs = c.num_discs;

        s->num_primitives = mliScenery_num_primitives(s);
        mli_check_mem(_mliScenery_malloc_vertices_and_triangles(s));
        mli_check_mem(_mliScenery_malloc_spherical_cap_hex(s));
        mli_check_mem(_mliScenery_malloc_spheres(s));
        mli_check_mem(_mliScenery_malloc_cylinders(s));
        mli_check_mem(_mliScenery_malloc_hexagons(s));
        mli_check_mem(_mliScenery_malloc_bicircleplane(s));
        mli_check_mem(_mliScenery_malloc_discs(s));
        mli_check_mem(_mliScenery_malloc_user_ids(s));
        return 1;
error:
        mliScenery_free(s);
        return 0;
}