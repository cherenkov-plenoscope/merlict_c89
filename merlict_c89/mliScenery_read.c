/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "mliScenery_read.h"

int _mliScenery_read_vertices_and_triangles(struct mliScenery *scenery, FILE *f)
{
        mli_fread(
                scenery->vertices,
                sizeof(struct mliVec),
                scenery->num_vertices,
                f);
        mli_fread(
                scenery->triangles,
                sizeof(struct mliFace),
                scenery->num_triangles,
                f);
        mli_fread(
                scenery->triangles_boundary_layers,
                sizeof(struct mliBoundaryLayer),
                scenery->num_triangles,
                f);
        return 1;
error:
        return 0;
}

int _mliScenery_read_spherical_cap_hex(struct mliScenery *scenery, FILE *f)
{
        mli_fread(
                scenery->spherical_cap_hex,
                sizeof(struct mliSphericalCapHex),
                scenery->num_spherical_cap_hex,
                f);
        mli_fread(
                scenery->spherical_cap_hex_boundary_layers,
                sizeof(struct mliBoundaryLayer),
                scenery->num_spherical_cap_hex,
                f);
        mli_fread(
                scenery->spherical_cap_hex_T,
                sizeof(struct mliHomTraComp),
                scenery->num_spherical_cap_hex,
                f);
        return 1;
error:
        return 0;
}

int _mliScenery_read_spheres(struct mliScenery *scenery, FILE *f)
{
        mli_fread(scenery->spheres, sizeof(double), scenery->num_spheres, f);
        mli_fread(
                scenery->spheres_boundary_layers,
                sizeof(struct mliBoundaryLayer),
                scenery->num_spheres,
                f);
        mli_fread(
                scenery->spheres_T,
                sizeof(struct mliHomTraComp),
                scenery->num_spheres,
                f);
        return 1;
error:
        return 0;
}

int _mliScenery_read_cylinders(struct mliScenery *scenery, FILE *f)
{
        mli_fread(
                scenery->cylinders,
                sizeof(struct mliCylinder),
                scenery->num_cylinders,
                f);
        mli_fread(
                scenery->cylinders_boundary_layers,
                sizeof(struct mliBoundaryLayer),
                scenery->num_cylinders,
                f);
        mli_fread(
                scenery->cylinders_T,
                sizeof(struct mliHomTraComp),
                scenery->num_cylinders,
                f);
        return 1;
error:
        return 0;
}

int _mliScenery_read_hexagons(struct mliScenery *scenery, FILE *f)
{
        mli_fread(
                scenery->hexagons,
                sizeof(struct mliHexagon),
                scenery->num_hexagons,
                f);
        mli_fread(
                scenery->hexagons_boundary_layers,
                sizeof(struct mliBoundaryLayer),
                scenery->num_hexagons,
                f);
        mli_fread(
                scenery->hexagons_T,
                sizeof(struct mliHomTraComp),
                scenery->num_hexagons,
                f);
        return 1;
error:
        return 0;
}

int _mliScenery_read_bicircleplanes(struct mliScenery *scenery, FILE *f)
{
        mli_fread(
                scenery->bicircleplanes,
                sizeof(struct mliBiCirclePlane),
                scenery->num_bicircleplanes,
                f);
        mli_fread(
                scenery->bicircleplanes_boundary_layers,
                sizeof(struct mliBoundaryLayer),
                scenery->num_bicircleplanes,
                f);
        mli_fread(
                scenery->bicircleplanes_T,
                sizeof(struct mliHomTraComp),
                scenery->num_bicircleplanes,
                f);
        return 1;
error:
        return 0;
}

int _mliScenery_read_discs(struct mliScenery *scenery, FILE *f)
{
        mli_fread(
                scenery->discs, sizeof(struct mliDisc), scenery->num_discs, f);
        mli_fread(
                scenery->discs_boundary_layers,
                sizeof(struct mliBoundaryLayer),
                scenery->num_discs,
                f);
        mli_fread(
                scenery->discs_T,
                sizeof(struct mliHomTraComp),
                scenery->num_discs,
                f);
        return 1;
error:
        return 0;
}

int _mliScenery_read_user_ids(struct mliScenery *scenery, FILE *f)
{
        mli_fread(
                scenery->user_ids,
                sizeof(uint32_t),
                scenery->num_primitives,
                f);
        return 1;
error:
        return 0;
}

int mliScenery_read_capacity_from_file(struct mliSceneryCapacity *cap, FILE *f)
{
        mli_fread(&cap->num_vertices, sizeof(uint32_t), 1u, f);
        mli_fread(&cap->num_triangles, sizeof(uint32_t), 1u, f);
        mli_fread(&cap->num_spherical_cap_hex, sizeof(uint32_t), 1u, f);
        mli_fread(&cap->num_spheres, sizeof(uint32_t), 1u, f);
        mli_fread(&cap->num_cylinders, sizeof(uint32_t), 1u, f);
        mli_fread(&cap->num_hexagons, sizeof(uint32_t), 1u, f);
        mli_fread(&cap->num_bicircleplanes, sizeof(uint32_t), 1u, f);
        mli_fread(&cap->num_discs, sizeof(uint32_t), 1u, f);
        return 1;
error:
        return 0;
}

int mliScenery_read_from_path(struct mliScenery *scenery, const char *path)
{
        char line[1024];
        struct mliSceneryCapacity scenery_capacity = mliSceneryCapacity_init();
        struct mliSceneryResourcesCapacity resource_capacity =
                mliSceneryResourcesCapacity_init();
        FILE *f;
        f = fopen(path, "r");
        mli_check(f != NULL, "Can not open Scenery-file for reading.");

        /* identifier */
        mli_check(fgets(line, 1024, f), "Can not read identifier 1st line.");
        mli_check(
                strcmp(line, "merlict_c89\n") == 0,
                "Expected starts with 'merlict_c89\\n'.");
        mli_check(fgets(line, 1024, f), "Can not read identifier 2nd line.");
        mli_check(
                strncmp(line, "MLI_VERSION", 11) == 0,
                "Expected starts with 'MLI_VERSION'.");
        mli_check(fgets(line, 1024, f), "Can not read identifier 3rd line.");
        mli_check(
                strcmp(line, "scenery\n") == 0,
                "Expected starts with 'scenery\\n'.");

        mli_check(
                mliScenery_read_capacity_from_file(&scenery_capacity, f),
                "Failed to read Scenery's capacity from file");

        mli_check(
                mliSceneryResources_read_capacity_from_file(
                        &resource_capacity, f),
                "Failed to read SceneryResources' capacity from file");

        mli_check_mem(mliScenery_malloc(scenery, scenery_capacity));
        mli_check_mem(mliSceneryResources_malloc(
                &scenery->resources, resource_capacity));
        mli_check(
                mliSceneryResources_read_from_file(&scenery->resources, f),
                "Failed to read resources from file.");

        mli_fread(&scenery->default_medium, sizeof(uint32_t), 1u, f);

        mli_c(_mliScenery_read_vertices_and_triangles(scenery, f));
        mli_c(_mliScenery_read_spherical_cap_hex(scenery, f));
        mli_c(_mliScenery_read_spheres(scenery, f));
        mli_c(_mliScenery_read_cylinders(scenery, f));
        mli_c(_mliScenery_read_hexagons(scenery, f));
        mli_c(_mliScenery_read_bicircleplanes(scenery, f));
        mli_c(_mliScenery_read_discs(scenery, f));

        mli_c(_mliScenery_read_user_ids(scenery, f));

        fclose(f);
        return 1;
error:
        if (f != NULL) {
                fclose(f);
        }
        mliScenery_free(scenery);
        return 0;
}