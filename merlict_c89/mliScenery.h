/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MERLICT_C89_MLISCENERY_H_
#define MERLICT_C89_MLISCENERY_H_

#include <math.h>
#include <stdint.h>

#include "mli_debug.h"
#include "mliVec.h"
#include "mli_primitive_codes.h"
#include "mliHomTra.h"
#include "mliColor.h"
#include "mliFunc.h"
#include "mliMesh.h"
#include "mliSphericalCapHex.h"
#include "mliSphere.h"
#include "mliCylinder.h"
#include "mliHexagon.h"
#include "mliBiCirclePlane.h"
#include "mliDisc.h"
#include "mliSurface.h"
#include "mliMedium.h"
#include "mliBoundaryLayer.h"
#include "mliSceneryResources.h"

struct mliIndexStarts {
        uint64_t triangles;
        uint64_t spherical_cap_hex;
        uint64_t spheres;
        uint64_t cylinders;
        uint64_t hexagons;
        uint64_t bicircleplanes;
        uint64_t discs;
        uint64_t _next;
};

struct mliSceneryCapacity {
        uint32_t num_vertices;
        uint32_t num_triangles;
        uint32_t num_spherical_cap_hex;
        uint32_t num_spheres;
        uint32_t num_cylinders;
        uint32_t num_hexagons;
        uint32_t num_bicircleplanes;
        uint32_t num_discs;
};

struct mliScenery {
        struct mliSceneryResources resources;
        uint32_t default_medium;

        uint32_t num_vertices;
        struct mliVec *vertices;

        uint32_t num_triangles;
        struct mliFace *triangles;
        struct mliBoundaryLayer *triangles_boundary_layers;

        uint32_t num_spherical_cap_hex;
        struct mliSphericalCapHex *spherical_cap_hex;
        struct mliBoundaryLayer *spherical_cap_hex_boundary_layers;
        struct mliHomTraComp *spherical_cap_hex_T;

        uint32_t num_spheres;
        struct mliSphere *spheres;
        struct mliBoundaryLayer *spheres_boundary_layers;
        struct mliHomTraComp *spheres_T;

        uint32_t num_cylinders;
        struct mliCylinder *cylinders;
        struct mliBoundaryLayer *cylinders_boundary_layers;
        struct mliHomTraComp *cylinders_T;

        uint32_t num_hexagons;
        struct mliHexagon *hexagons;
        struct mliBoundaryLayer *hexagons_boundary_layers;
        struct mliHomTraComp *hexagons_T;

        uint32_t num_bicircleplanes;
        struct mliBiCirclePlane *bicircleplanes;
        struct mliBoundaryLayer *bicircleplanes_boundary_layers;
        struct mliHomTraComp *bicircleplanes_T;

        uint32_t num_discs;
        struct mliDisc *discs;
        struct mliBoundaryLayer *discs_boundary_layers;
        struct mliHomTraComp *discs_T;

        uint64_t num_primitives;
        uint32_t *user_ids;
};

int mliScenery_malloc(struct mliScenery *s, const struct mliSceneryCapacity c);
int _mliScenery_malloc_user_ids(struct mliScenery *s);
int _mliScenery_malloc_discs(struct mliScenery *s);
int _mliScenery_malloc_bicircleplane(struct mliScenery *s);
int _mliScenery_malloc_hexagons(struct mliScenery *s);
int _mliScenery_malloc_cylinders(struct mliScenery *s);
int _mliScenery_malloc_spheres(struct mliScenery *s);
int _mliScenery_malloc_spherical_cap_hex(struct mliScenery *s);
int _mliScenery_malloc_vertices_and_triangles(struct mliScenery *s);
void mliScenery_free(struct mliScenery *scenery);
void _mliScenery_free_user_ids(struct mliScenery *scenery);
void _mliScenery_free_discs(struct mliScenery *scenery);
void _mliScenery_free_bicircleplanes(struct mliScenery *scenery);
void _mliScenery_free_hexagons(struct mliScenery *scenery);
void _mliScenery_free_cylinders(struct mliScenery *scenery);
void _mliScenery_free_spheres(struct mliScenery *scenery);
void _mliScenery_free_spherical_cap_hex(struct mliScenery *scenery);
void _mliScenery_free_vertices_and_triangles(struct mliScenery *scenery);
struct mliScenery mliScenery_init(void);
uint64_t mliScenery_num_primitives(const struct mliScenery *scenery);
struct mliIndexStarts mliScenery_index_starts(const struct mliScenery *scenery);
struct mliSceneryCapacity mliSceneryCapacity_init(void);
#endif
