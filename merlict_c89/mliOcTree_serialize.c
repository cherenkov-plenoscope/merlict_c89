/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "mliOcTree_serialize.h"

int mliOcTree_fwrite(const struct mliOcTree *octree, FILE *f)
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

int mliOcTree_malloc_fread(struct mliOcTree *octree, FILE *f)
{
        char line[256];
        uint64_t num_nodes;
        uint64_t num_leafs;
        uint64_t num_object_links;
        memset(line, '\0', sizeof(line));

        /* identifier */
        mli_check(
                fgets(line, sizeof(line), f),
                "Can not read identifier 1st line.");
        mli_check(
                strcmp(line, "merlict_c89\n") == 0,
                "Expected starts with 'merlict_c89\\n'.");
        mli_check(
                fgets(line, sizeof(line), f),
                "Can not read identifier 2nd line.");
        mli_check(
                strncmp(line, "MLI_VERSION", 11) == 0,
                "Expected starts with 'MLI_VERSION'.");
        mli_check(
                fgets(line, sizeof(line), f),
                "Can not read identifier 3rd line.");
        mli_check(
                strcmp(line, "octree\n") == 0,
                "Expected starts with 'octree\\n'.");

        /* capacity */
        mli_fread(&num_nodes, sizeof(uint64_t), 1u, f);
        mli_fread(&num_leafs, sizeof(uint64_t), 1u, f);
        mli_fread(&num_object_links, sizeof(uint64_t), 1u, f);

        mli_check(
                mliOcTree_malloc(
                        octree, num_nodes, num_leafs, num_object_links),
                "Can not malloc octree from file.");

        mli_fread(octree->nodes, sizeof(struct mliNode), octree->num_nodes, f);
        mli_fread(
                octree->leafs.adresses,
                sizeof(struct mliLeafAddress),
                octree->leafs.num_leafs,
                f);
        mli_fread(
                octree->leafs.object_links,
                sizeof(uint32_t),
                octree->leafs.num_object_links,
                f);

        /* mliCube */
        mli_fread(&octree->cube.lower.x, sizeof(double), 1u, f);
        mli_fread(&octree->cube.lower.y, sizeof(double), 1u, f);
        mli_fread(&octree->cube.lower.z, sizeof(double), 1u, f);
        mli_fread(&octree->cube.edge_length, sizeof(double), 1u, f);

        /* root type */
        mli_fread(&octree->root_type, sizeof(uint8_t), 1u, f);

        return 1;
error:
        return 0;
}

int mliOcTree_write_to_path(const struct mliOcTree *octree, const char *path)
{
        FILE *f;
        f = fopen(path, "w");
        mli_check(f != NULL, "Can not open octree-file for writing.");

        mli_check(
                mliOcTree_fwrite(octree, f),
                "Can not write octree to file.");

        fclose(f);
        return 1;
error:
        if (f != NULL) {
                fclose(f);
        }
        return 0;
}

int mliOcTree_malloc_from_path(
        struct mliOcTree *octree,
        const char *path)
{
        FILE *f;
        f = fopen(path, "r");
        mli_check(f != NULL, "Can not open octree-file for reading.");

        mli_check(
                mliOcTree_malloc_fread(octree, f),
                "Can not read octree to file.");

        fclose(f);
        return 1;
error:
        if (f != NULL) {
                fclose(f);
        }
        return 0;
}
