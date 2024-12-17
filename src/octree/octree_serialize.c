/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "octree_serialize.h"
#include "../magicid/magicid.h"
#include "../chk/chk.h"

int mli_OcTree_to_io(const struct mli_OcTree *octree, struct mli_IO *f)
{
        struct mli_MagicId magic;

        /* magic identifier */
        chk(mli_MagicId_set(&magic, "mli_OcTree"));
        chk_IO_write(&magic, sizeof(struct mli_MagicId), 1u, f);

        /* capacity */
        chk_IO_write(&octree->num_nodes, sizeof(uint64_t), 1u, f);
        chk_IO_write(&octree->leafs.num_leafs, sizeof(uint64_t), 1u, f);
        chk_IO_write(&octree->leafs.num_object_links, sizeof(uint64_t), 1u, f);

        /* nodes */
        chk_IO_write(
                octree->nodes,
                sizeof(struct mli_octree_Node),
                octree->num_nodes,
                f);

        /* leaf addresses */
        chk_IO_write(
                octree->leafs.adresses,
                sizeof(struct mli_octree_LeafAddress),
                octree->leafs.num_leafs,
                f);

        /* leaf links */
        chk_IO_write(
                octree->leafs.object_links,
                sizeof(uint32_t),
                octree->leafs.num_object_links,
                f);

        /* mli_Cube */
        chk_IO_write(&octree->cube.lower.x, sizeof(double), 1u, f);
        chk_IO_write(&octree->cube.lower.y, sizeof(double), 1u, f);
        chk_IO_write(&octree->cube.lower.z, sizeof(double), 1u, f);
        chk_IO_write(&octree->cube.edge_length, sizeof(double), 1u, f);

        /* root type */
        chk_IO_write(&octree->root_type, sizeof(uint8_t), 1u, f);

        return 1;
chk_error:
        return 0;
}

int mli_OcTree_from_io(struct mli_OcTree *octree, struct mli_IO *f)
{
        uint64_t num_nodes;
        uint64_t num_leafs;
        uint64_t num_object_links;
        struct mli_MagicId magic;

        /* magic identifier */
        chk_IO_read(&magic, sizeof(struct mli_MagicId), 1u, f);
        chk(mli_MagicId_has_word(&magic, "mli_OcTree"));
        mli_MagicId_warn_version(&magic);

        /* capacity */
        chk_IO_read(&num_nodes, sizeof(uint64_t), 1u, f);
        chk_IO_read(&num_leafs, sizeof(uint64_t), 1u, f);
        chk_IO_read(&num_object_links, sizeof(uint64_t), 1u, f);

        chk_msg(mli_OcTree_malloc(
                        octree, num_nodes, num_leafs, num_object_links),
                "Can not malloc octree from file.");

        chk_IO_read(
                octree->nodes,
                sizeof(struct mli_octree_Node),
                octree->num_nodes,
                f);
        chk_IO_read(
                octree->leafs.adresses,
                sizeof(struct mli_octree_LeafAddress),
                octree->leafs.num_leafs,
                f);
        chk_IO_read(
                octree->leafs.object_links,
                sizeof(uint32_t),
                octree->leafs.num_object_links,
                f);

        /* mli_Cube */
        chk_IO_read(&octree->cube.lower.x, sizeof(double), 1u, f);
        chk_IO_read(&octree->cube.lower.y, sizeof(double), 1u, f);
        chk_IO_read(&octree->cube.lower.z, sizeof(double), 1u, f);
        chk_IO_read(&octree->cube.edge_length, sizeof(double), 1u, f);

        /* root type */
        chk_IO_read(&octree->root_type, sizeof(uint8_t), 1u, f);

        return 1;
chk_error:
        return 0;
}
