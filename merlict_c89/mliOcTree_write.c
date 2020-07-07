/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "mliOcTree_write.h"

/* Copyright 2019 Sebastian Achim Mueller */

int mliOcTree_write_to_file(struct mliOcTree *octree, FILE *f)
{
        /* identifier */
        mli_c(fprintf(f, "merlict_c89\n"));
        mli_c(
                fprintf(f,
                        "MLI_VERSION %d.%d.%d\n",
                        MLI_VERSION_MAYOR,
                        MLI_VERSION_MINOR,
                        MLI_VERSION_PATCH));
        mli_c(fprintf(f, "octree\n"));

        /* capacity */
        mli_fwrite(&octree->num_nodes, sizeof(uint64_t), 1u, f);
        mli_fwrite(&octree->leafs.num_leafs, sizeof(uint64_t), 1u, f);
        mli_fwrite(&octree->leafs.num_object_links, sizeof(uint64_t), 1u, f);

        /* nodes */
        mli_fwrite(octree->nodes, sizeof(struct mliNode), octree->num_nodes, f);

        /* leaf addresses */
        mli_fwrite(
                octree->leafs.adresses,
                sizeof(struct mliLeafAddress),
                octree->leafs.num_leafs,
                f);

        /* leaf links */
        mli_fwrite(
                octree->leafs.object_links,
                sizeof(uint32_t),
                octree->leafs.num_object_links,
                f);

        /* mliCube */
        mli_fwrite(&octree->cube.lower.x, sizeof(double), 1u, f);
        mli_fwrite(&octree->cube.lower.y, sizeof(double), 1u, f);
        mli_fwrite(&octree->cube.lower.z, sizeof(double), 1u, f);
        mli_fwrite(&octree->cube.edge_length, sizeof(double), 1u, f);

        /* root type */
        mli_fwrite(&octree->root_type, sizeof(uint8_t), 1u, f);

        return 1;
error:
        return 0;
}

int mliOcTree_write_to_path(struct mliOcTree *octree, const char *path)
{
        FILE *f;
        f = fopen(path, "w");
        mli_check(f != NULL, "Can not open octree-file for writing.");

        mli_check(
                mliOcTree_write_to_file(octree, f),
                "Can not write octree to file.");

        fclose(f);
        return 1;
error:
        if (f != NULL) {
                fclose(f);
        }
        return 0;
}
