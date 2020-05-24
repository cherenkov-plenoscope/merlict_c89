/* Copyright 2019 Sebastian Achim Mueller */
#ifndef MERLICT_OCTREE_READ_H_
#define MERLICT_OCTREE_READ_H_

#include <math.h>
#include <stdint.h>
#include "mli_debug.h"
#include "mliOcTree.h"


int mliOcTree_read_and_malloc_from_file(struct mliOcTree *octree, FILE *f)
{
        char line[1024];
        uint64_t num_nodes;
        uint64_t num_leafs;
        uint64_t num_object_links;

        /* identifier */
        mli_check(
                fgets(line, 1024, f),
                "Can not read identifier 1st line.")
        mli_check(
                strcmp(line, "merlict_c89\n") == 0,
                "Expected starts with 'merlict_c89\\n'.");
        mli_check(
                fgets(line, 1024, f),
                "Can not read identifier 2nd line.")
        mli_check(
                strcmp(line, "octree\n") == 0,
                "Expected starts with 'octree\\n'.");

        /* capacity */
        mli_fread(&num_nodes, sizeof(uint64_t), 1u, f);
        mli_fread(&num_leafs, sizeof(uint64_t), 1u, f);
        mli_fread(&num_object_links, sizeof(uint64_t), 1u, f);

        mli_check(
                mliOcTree_malloc(
                        octree,
                        num_nodes,
                        num_leafs,
                        num_object_links),
                "Can not malloc octree from file.");

        mli_fread(
                octree->nodes,
                sizeof(struct mliNode),
                octree->num_nodes,
                f);
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

int mliOcTree_read_and_malloc_from_path(
        struct mliOcTree *octree,
        const char *path)
{
        FILE *f;
        f = fopen(path, "r");
        mli_check(f != NULL, "Can not open octree-file for reading.");

        mli_check(
                mliOcTree_read_and_malloc_from_file(octree, f),
                "Can not read octree to file.");

        fclose(f);
        return 1;
error:
        if (f != NULL) {
                fclose(f);
        }
        return 0;
}

#endif
