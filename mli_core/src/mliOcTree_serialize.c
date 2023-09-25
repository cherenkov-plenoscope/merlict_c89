/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "mliOcTree_serialize.h"
#include "chk_debug.h"

int mliOcTree_fwrite(const struct mliOcTree *octree, FILE *f)
{
        struct mliMagicId magic;

        /* magic identifier */
        chk(mliMagicId_set(&magic, "mliOcTree"));
        chk_fwrite(&magic, sizeof(struct mliMagicId), 1u, f);

        /* capacity */
        chk_fwrite(&octree->num_nodes, sizeof(uint64_t), 1u, f);
        chk_fwrite(&octree->leafs.num_leafs, sizeof(uint64_t), 1u, f);
        chk_fwrite(&octree->leafs.num_object_links, sizeof(uint64_t), 1u, f);

        /* nodes */
        chk_fwrite(octree->nodes, sizeof(struct mliNode), octree->num_nodes, f);

        /* leaf addresses */
        chk_fwrite(
                octree->leafs.adresses,
                sizeof(struct mliLeafAddress),
                octree->leafs.num_leafs,
                f);

        /* leaf links */
        chk_fwrite(
                octree->leafs.object_links,
                sizeof(uint32_t),
                octree->leafs.num_object_links,
                f);

        /* mliCube */
        chk_fwrite(&octree->cube.lower.x, sizeof(double), 1u, f);
        chk_fwrite(&octree->cube.lower.y, sizeof(double), 1u, f);
        chk_fwrite(&octree->cube.lower.z, sizeof(double), 1u, f);
        chk_fwrite(&octree->cube.edge_length, sizeof(double), 1u, f);

        /* root type */
        chk_fwrite(&octree->root_type, sizeof(uint8_t), 1u, f);

        return 1;
error:
        return 0;
}

int mliOcTree_malloc_fread(struct mliOcTree *octree, FILE *f)
{
        uint64_t num_nodes;
        uint64_t num_leafs;
        uint64_t num_object_links;
        struct mliMagicId magic;

        /* magic identifier */
        chk_fread(&magic, sizeof(struct mliMagicId), 1u, f);
        chk(mliMagicId_has_word(&magic, "mliOcTree"));
        mliMagicId_warn_version(&magic);

        /* capacity */
        chk_fread(&num_nodes, sizeof(uint64_t), 1u, f);
        chk_fread(&num_leafs, sizeof(uint64_t), 1u, f);
        chk_fread(&num_object_links, sizeof(uint64_t), 1u, f);

        chk_msg(mliOcTree_malloc(
                        octree, num_nodes, num_leafs, num_object_links),
                "Can not malloc octree from file.");

        chk_fread(octree->nodes, sizeof(struct mliNode), octree->num_nodes, f);
        chk_fread(
                octree->leafs.adresses,
                sizeof(struct mliLeafAddress),
                octree->leafs.num_leafs,
                f);
        chk_fread(
                octree->leafs.object_links,
                sizeof(uint32_t),
                octree->leafs.num_object_links,
                f);

        /* mliCube */
        chk_fread(&octree->cube.lower.x, sizeof(double), 1u, f);
        chk_fread(&octree->cube.lower.y, sizeof(double), 1u, f);
        chk_fread(&octree->cube.lower.z, sizeof(double), 1u, f);
        chk_fread(&octree->cube.edge_length, sizeof(double), 1u, f);

        /* root type */
        chk_fread(&octree->root_type, sizeof(uint8_t), 1u, f);

        return 1;
error:
        return 0;
}

int mliOcTree_write_to_path(const struct mliOcTree *octree, const char *path)
{
        FILE *f;
        f = fopen(path, "w");
        chk_msg(f != NULL, "Can not open octree-file for writing.");

        chk_msg(mliOcTree_fwrite(octree, f), "Can not write octree to file.");

        fclose(f);
        return 1;
error:
        if (f != NULL) {
                fclose(f);
        }
        return 0;
}

int mliOcTree_malloc_from_path(struct mliOcTree *octree, const char *path)
{
        FILE *f;
        f = fopen(path, "r");
        chk_msg(f != NULL, "Can not open octree-file for reading.");

        chk_msg(mliOcTree_malloc_fread(octree, f),
                "Can not read octree to file.");

        fclose(f);
        return 1;
error:
        if (f != NULL) {
                fclose(f);
        }
        return 0;
}
