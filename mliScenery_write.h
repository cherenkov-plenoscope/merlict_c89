/* Copyright 2019 Sebastian Achim Mueller */
#ifndef MERLICT_MLISCENERY_WRITE_H_
#define MERLICT_MLISCENERY_WRITE_H_

#include <math.h>
#include <stdint.h>
#include "mli_debug.h"
#include "mliScenery.h"
#include "mliSceneryResources_write.h"

#define MLI_SCENERY_MAGIC 43180u

int _mliScenery_write_vertices_and_triangles(
        const struct mliScenery *scenery,
        FILE *f)
{
        mli_fwrite(
                scenery->vertices,
                sizeof(struct mliVec),
                scenery->num_vertices,
                f);
        mli_fwrite(
                scenery->triangles,
                sizeof(struct mliFace),
                scenery->num_triangles,
                f);
        mli_fwrite(
                scenery->triangles_boundary_layers,
                sizeof(struct mliBoundaryLayer),
                scenery->num_triangles,
                f);
        return 1;
error:
        return 0;
}

int _mliScenery_write_spherical_cap_hex(
        const struct mliScenery *scenery,
        FILE *f)
{
        mli_fwrite(
                scenery->spherical_cap_hex,
                sizeof(struct mliSphericalCapHex),
                scenery->num_spherical_cap_hex,
                f);
        mli_fwrite(
                scenery->spherical_cap_hex_boundary_layers,
                sizeof(struct mliBoundaryLayer),
                scenery->num_spherical_cap_hex,
                f);
        mli_fwrite(
                scenery->spherical_cap_hex_T,
                sizeof(struct mliHomTraComp),
                scenery->num_spherical_cap_hex,
                f);
        return 1;
error:
        return 0;
}

int _mliScenery_write_spheres(const struct mliScenery *scenery, FILE *f)
{
        mli_fwrite(scenery->spheres, sizeof(double), scenery->num_spheres, f);
        mli_fwrite(
                scenery->spheres_boundary_layers,
                sizeof(struct mliBoundaryLayer),
                scenery->num_spheres,
                f);
        mli_fwrite(
                scenery->spheres_T,
                sizeof(struct mliHomTraComp),
                scenery->num_spheres,
                f);
        return 1;
error:
        return 0;
}

int _mliScenery_write_cylinders(const struct mliScenery *scenery, FILE *f)
{
        mli_fwrite(
                scenery->cylinders,
                sizeof(struct mliCylinder),
                scenery->num_cylinders,
                f);
        mli_fwrite(
                scenery->cylinders_boundary_layers,
                sizeof(struct mliBoundaryLayer),
                scenery->num_cylinders,
                f);
        mli_fwrite(
                scenery->cylinders_T,
                sizeof(struct mliHomTraComp),
                scenery->num_cylinders,
                f);
        return 1;
error:
        return 0;
}

int _mliScenery_write_hexagons(const struct mliScenery *scenery, FILE *f)
{
        mli_fwrite(
                scenery->hexagons,
                sizeof(struct mliHexagon),
                scenery->num_hexagons,
                f);
        mli_fwrite(
                scenery->hexagons_boundary_layers,
                sizeof(struct mliBoundaryLayer),
                scenery->num_hexagons,
                f);
        mli_fwrite(
                scenery->hexagons_T,
                sizeof(struct mliHomTraComp),
                scenery->num_hexagons,
                f);
        return 1;
error:
        return 0;
}

int _mliScenery_write_bicircleplanes(const struct mliScenery *scenery, FILE *f)
{
        mli_fwrite(
                scenery->bicircleplanes,
                sizeof(struct mliBiCirclePlane),
                scenery->num_bicircleplanes,
                f);
        mli_fwrite(
                scenery->bicircleplanes_boundary_layers,
                sizeof(struct mliBoundaryLayer),
                scenery->num_bicircleplanes,
                f);
        mli_fwrite(
                scenery->bicircleplanes_T,
                sizeof(struct mliHomTraComp),
                scenery->num_bicircleplanes,
                f);
        return 1;
error:
        return 0;
}

int _mliScenery_write_discs(const struct mliScenery *scenery, FILE *f)
{
        mli_fwrite(
                scenery->discs, sizeof(struct mliDisc), scenery->num_discs, f);
        mli_fwrite(
                scenery->discs_boundary_layers,
                sizeof(struct mliBoundaryLayer),
                scenery->num_discs,
                f);
        mli_fwrite(
                scenery->discs_T,
                sizeof(struct mliHomTraComp),
                scenery->num_discs,
                f);
        return 1;
error:
        return 0;
}

int mliScenery_write_capacity_to_file(const struct mliScenery *scenery, FILE *f)
{
        mli_fwrite(&scenery->num_vertices, sizeof(uint32_t), 1u, f);
        mli_fwrite(&scenery->num_triangles, sizeof(uint32_t), 1u, f);
        mli_fwrite(&scenery->num_spherical_cap_hex, sizeof(uint32_t), 1u, f);
        mli_fwrite(&scenery->num_spheres, sizeof(uint32_t), 1u, f);
        mli_fwrite(&scenery->num_cylinders, sizeof(uint32_t), 1u, f);
        mli_fwrite(&scenery->num_hexagons, sizeof(uint32_t), 1u, f);
        mli_fwrite(&scenery->num_bicircleplanes, sizeof(uint32_t), 1u, f);
        mli_fwrite(&scenery->num_discs, sizeof(uint32_t), 1u, f);
        return 1;
error:
        return 0;
}

int _mliScenery_write_user_ids(const struct mliScenery *scenery, FILE *f)
{
        mli_fwrite(
                scenery->user_ids,
                sizeof(uint32_t),
                scenery->num_primitives,
                f);
        return 1;
error:
        return 0;
}

int mliScenery_write_to_path(const struct mliScenery *scenery, const char *path)
{
        FILE *f;
        uint64_t magic = MLI_SCENERY_MAGIC;
        f = fopen(path, "w");
        mli_check(f != NULL, "Can not open Scenery-file for writing.");

        /* magic identifier */
        mli_fwrite(&magic, sizeof(uint64_t), 1u, f);

        mli_check(
                mliScenery_write_capacity_to_file(scenery, f),
                "Failed to write Scenery's capacity to file.");

        mli_check(
                mliSceneryResources_write_capacity_to_file(
                        &scenery->resources, f),
                "Failed to write SceneryResources' capacity to file.");

        mli_check(
                mliSceneryResources_append_to_file(&scenery->resources, f),
                "Can not write SceneryResources to file.");

        mli_fwrite(&scenery->default_medium, sizeof(uint32_t), 1u, f);

        mli_c(_mliScenery_write_vertices_and_triangles(scenery, f));
        mli_c(_mliScenery_write_spherical_cap_hex(scenery, f));
        mli_c(_mliScenery_write_spheres(scenery, f));
        mli_c(_mliScenery_write_cylinders(scenery, f));
        mli_c(_mliScenery_write_hexagons(scenery, f));
        mli_c(_mliScenery_write_bicircleplanes(scenery, f));
        mli_c(_mliScenery_write_discs(scenery, f));

        mli_c(_mliScenery_write_user_ids(scenery, f));
        fclose(f);
        return 1;
error:
        if (f != NULL) {
                fclose(f);
        }
        return 0;
}

#endif
