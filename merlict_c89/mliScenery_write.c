/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "mliScenery_write.h"

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
        f = fopen(path, "w");
        mli_check(f != NULL, "Can not open Scenery-file for writing.");

        /* identifier */
        mli_c(fprintf(f, "merlict_c89\n"));
        mli_c(
                fprintf(f,
                        "MLI_VERSION %d.%d.%d\n",
                        MLI_VERSION_MAYOR,
                        MLI_VERSION_MINOR,
                        MLI_VERSION_PATCH));
        mli_c(fprintf(f, "scenery\n"));

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

        mli_c(_mliScenery_write_user_ids(scenery, f));
        fclose(f);
        return 1;
error:
        if (f != NULL) {
                fclose(f);
        }
        return 0;
}
