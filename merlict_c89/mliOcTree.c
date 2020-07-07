/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "mliOcTree.h"

/* Copyright 2019 Sebastian Achim Mueller */

/*
 * mliOcTree
 * =========
 *
 * A cache-aware mliOcTree to accelerate the traversal.
 * mliOcTree is created from the dynamic, mliTmpOcTree.
 *
 * Structure of mliTmpOcTree
 * -------------------------
 *
 *                               mliTmpNode(0,-)
 *                                    |-objects[a,b,c,d,e,f,g,h]
 *                     _______________|________________
 *                    |                                |
 *                mliTmpNode(1,-)                  mliTmpNode(2,-)
 *                    |objects[a,b,c,d]                |objects[e,f,g,h]
 *             _______|_______                 ________|________
 *            |               |               |                 |
 *        mliTmpNode(3,0) mliTmpNode(4,1) mliTmpNode(5,2)   mliTmpNode(6,3)
 *            |objects[a,b,c] |objects[c,d]  |objects[e,f]     |objects[f,g,h]
 *
 *      mliTmpNode link to each other via pointer.
 *      Each mliTmpNode is allocated separately.
 *
 *      advantages:
 *              easy to grow and fill
 *      disadvantage:
 *              memory is scattered, difficult to serialize
 *
 * Structure of mliOcTree
 * ----------------------
 *
 *                object_links w.r.t. mliScenery ->
 *          leafs +-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+
 *            |---|  a  |  b  |  c  |  c  |  d  |  e  |  f  |  f  |  g  |  h  |
 *            |   +-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+
 * mliOcTree  |   0     1     2     3     4     5     6     7     8     9     10
 *  |         |   ^                 ^           ^           ^
 *  |         |
 *  |---------|   addresses
 *  |         |   +-----+-----+-----+-----+
 *  |         |---|  0  |  3  |  4  |  6  |
 *  |             +-----+-----+-----+-----+ first w.r.t object_links
 *  |             |  3  |  2  |  2  |  3  |
 *  |             +-----+-----+-----+-----+ size w.r.t object_links
 *  |             0     1     2     3
 *  |
 *  |         nodes ->
 *  |         +-----+-----+-----+
 *  |---------|node |leaf |leaf |
 *            +-----+-----+-----+ ^
 *            |node |leaf |leaf | |
 *            +-----+-----+-----+ types   type == node
 *            |  1  |  0  |  2  |                  w.r.t. mliOcTree.nodes
 *            +-----+-----+-----+ ^       type == leaf
 *            |  2  |  1  |  3  | |               w.r.t. leafs.addresses
 *            +-----+-----+-----+ children
 *            0     1     2
 *
 *      advantages:
 *              small memory footprint, contoinous memory blocks,
 *              easy to serialze
 *      disadvantage:
 *              can not grow
 */

struct mliLeafAddress mliLeafAddress_init(void)
{
        struct mliLeafAddress address;
        address.first_object_link = 0u;
        address.num_object_links = 0u;
        return address;
}

struct mliLeafArray mliLeafArray_init(void)
{
        struct mliLeafArray leafs;
        leafs.num_leafs = 0;
        leafs.adresses = NULL;
        leafs.num_object_links = 0u;
        leafs.object_links = NULL;
        return leafs;
}

void mliLeafArray_free(struct mliLeafArray *leafs)
{
        free(leafs->object_links);
        free(leafs->adresses);
        *leafs = mliLeafArray_init();
}

int mliLeafArray_malloc(
        struct mliLeafArray *leafs,
        const uint64_t num_leafs,
        const uint64_t num_object_links)
{
        uint64_t i;
        mliLeafArray_free(leafs);
        leafs->num_leafs = num_leafs;
        mli_malloc(leafs->adresses, struct mliLeafAddress, leafs->num_leafs);
        for (i = 0; i < leafs->num_leafs; i++) {
                leafs->adresses[i] = mliLeafAddress_init();
        }
        leafs->num_object_links = num_object_links;
        mli_malloc(leafs->object_links, uint32_t, leafs->num_object_links);
        for (i = 0; i < leafs->num_object_links; i++) {
                leafs->object_links[i] = 0u;
        }
        return 1;
error:
        return 0;
}

struct mliNode mliNode_init(void)
{
        uint64_t c = 0;
        struct mliNode node;
        for (c = 0; c < 8u; c++) {
                node.children[c] = 0u;
                node.types[c] = MLI_OCTREE_TYPE_NONE;
        }
        return node;
}

struct mliOcTree mliOcTree_init(void)
{
        struct mliOcTree tree;
        tree.cube.lower = mliVec_set(0., 0., 0.);
        tree.cube.edge_length = 0.;
        tree.num_nodes = 0u;
        tree.nodes = NULL;
        tree.leafs = mliLeafArray_init();
        tree.root_type = MLI_OCTREE_TYPE_NONE;
        return tree;
}

void mliOcTree_free(struct mliOcTree *tree)
{
        free(tree->nodes);
        mliLeafArray_free(&tree->leafs);
        *tree = mliOcTree_init();
}

int mliOcTree_malloc(
        struct mliOcTree *tree,
        const uint64_t num_nodes,
        const uint64_t num_leafs,
        const uint64_t num_object_links)
{
        uint64_t i;
        mliOcTree_free(tree);
        tree->num_nodes = num_nodes;
        mli_malloc(tree->nodes, struct mliNode, tree->num_nodes);
        for (i = 0; i < tree->num_nodes; i++)
                tree->nodes[i] = mliNode_init();
        mli_c(mliLeafArray_malloc(&tree->leafs, num_leafs, num_object_links));
        return 1;
error:
        return 0;
}

void _mliOcTree_set_node(
        struct mliOcTree *tree,
        const struct mliTmpNode *dynnode)
{
        uint64_t c;
        uint64_t i = dynnode->node_index;
        assert(i < tree->num_nodes);
        for (c = 0; c < 8u; c++) {
                if (dynnode->children[c] != NULL) {
                        if (dynnode->children[c]->node_index >= 0) {
                                tree->nodes[i].children[c] =
                                        dynnode->children[c]->node_index;
                                tree->nodes[i].types[c] = MLI_OCTREE_TYPE_NODE;
                        } else if (dynnode->children[c]->leaf_index >= 0) {
                                tree->nodes[i].children[c] =
                                        dynnode->children[c]->leaf_index;
                                tree->nodes[i].types[c] = MLI_OCTREE_TYPE_LEAF;
                        } else {
                                tree->nodes[i].children[c] = 0;
                                tree->nodes[i].types[c] = MLI_OCTREE_TYPE_NONE;
                        }
                } else {
                        tree->nodes[i].children[c] = 0;
                        tree->nodes[i].types[c] = MLI_OCTREE_TYPE_NONE;
                }
        }
}

void _mliOcTree_set_leaf(
        struct mliOcTree *tree,
        const struct mliTmpNode *dynnode,
        uint64_t *object_link_size)
{
        uint64_t o;
        uint64_t i = dynnode->leaf_index;
        assert(i < tree->leafs.num_leafs);
        tree->leafs.adresses[i].first_object_link = *object_link_size;
        tree->leafs.adresses[i].num_object_links = dynnode->num_objects;
        *object_link_size += dynnode->num_objects;
        for (o = 0; o < dynnode->num_objects; o++) {
                uint64_t l = tree->leafs.adresses[i].first_object_link + o;
                assert(l < tree->leafs.num_object_links);
                tree->leafs.object_links[l] = dynnode->objects[o];
        }
}

void _mliOcTree_set(
        struct mliOcTree *tree,
        const struct mliTmpNode *dynnode,
        uint64_t *object_link_size)
{
        uint64_t c;
        if (dynnode->node_index >= 0) {
                _mliOcTree_set_node(tree, dynnode);
        } else if (dynnode->leaf_index >= 0) {
                _mliOcTree_set_leaf(tree, dynnode, object_link_size);
        }

        for (c = 0; c < 8u; c++) {
                if (dynnode->children[c] != NULL) {
                        _mliOcTree_set(
                                tree, dynnode->children[c], object_link_size);
                }
        }
}

void mliOcTree_set(struct mliOcTree *tree, const struct mliTmpOcTree *dyntree)
{
        uint64_t object_link_size = 0u;
        tree->cube = dyntree->cube;
        if (mliTmpNode_num_children(&dyntree->root) > 0) {
                tree->root_type = MLI_OCTREE_TYPE_NODE;
        } else {
                tree->root_type = MLI_OCTREE_TYPE_LEAF;
        }

        _mliOcTree_set(tree, &dyntree->root, &object_link_size);
}

uint64_t mliOcTree_node_num_children(
        const struct mliOcTree *tree,
        const uint64_t node_idx)
{
        uint64_t num = 0u;
        uint64_t c;
        for (c = 0; c < 8u; c++) {
                if (tree->nodes[node_idx].types[c] > MLI_OCTREE_TYPE_NONE) {
                        num++;
                }
        }
        return num;
}

uint64_t mliOcTree_leaf_num_objects(
        const struct mliOcTree *tree,
        const uint64_t leaf)
{
        return tree->leafs.adresses[leaf].num_object_links;
}

uint32_t mliOcTree_leaf_object_link(
        const struct mliOcTree *tree,
        const uint64_t leaf,
        const uint64_t object_link)
{
        uint64_t i = tree->leafs.adresses[leaf].first_object_link + object_link;
        return tree->leafs.object_links[i];
}

int mliOcTree_malloc_from_scenery(
        struct mliOcTree *octree,
        const struct mliScenery *scenery)
{
        uint64_t num_nodes = 0;
        uint64_t num_leafs = 0;
        uint64_t num_object_links = 0;
        struct mliTmpOcTree tmp_octree = mliTmpOcTree_init();
        mli_check(
                mliTmpOcTree_malloc_from_scenery(&tmp_octree, scenery),
                "Failed to create dynamic, and temporary TmpOcTree "
                "from scenery");
        mliTmpNode_set_flat_index(&tmp_octree.root);
        mliTmpNode_num_nodes_leafs_objects(
                &tmp_octree.root, &num_nodes, &num_leafs, &num_object_links);

        mliOcTree_free(octree);
        mli_check(
                mliOcTree_malloc(
                        octree, num_nodes, num_leafs, num_object_links),
                "Failed to allocate cache-aware octree from dynamic octree.");
        mliOcTree_set(octree, &tmp_octree);
        mliTmpOcTree_free(&tmp_octree);

        return 1;
error:
        return 0;
}

int _mliOcTree_equal_payload(
        const struct mliOcTree *tree,
        const int32_t node_idx,
        const int32_t node_type,
        const struct mliTmpNode *tmp_node)
{
        if (node_type == MLI_OCTREE_TYPE_LEAF) {
                /* leaf */
                uint64_t leaf_idx;
                uint64_t obj;
                mli_check(
                        mliTmpNode_num_children(tmp_node) == 0,
                        "Expect tmp_node to have 0 children when "
                        "node_type == LEAF.");
                leaf_idx = node_idx;
                mli_check(
                        leaf_idx < tree->leafs.num_leafs,
                        "The leaf_idx is out of range.");
                mli_check(
                        tree->leafs.adresses[leaf_idx].num_object_links ==
                                tmp_node->num_objects,
                        "Expected leafs to have equal num_object_links.");
                for (obj = 0; obj < tmp_node->num_objects; obj++) {
                        uint64_t l = obj + tree->leafs.adresses[leaf_idx]
                                                   .first_object_link;
                        mli_check(
                                tree->leafs.object_links[l] ==
                                        tmp_node->objects[obj],
                                "Expected object_links in leaf to be equal.");
                }
                mli_check(
                        tree->leafs.adresses[leaf_idx].num_object_links ==
                                tmp_node->num_objects,
                        "Expected leafs to have equal num_object_links.");
        } else if (node_type == MLI_OCTREE_TYPE_NODE) {
                /* node */
                uint64_t c;
                mli_check(
                        node_idx >= 0,
                        "This node_idx is expected to point to a node.");
                for (c = 0; c < 8u; c++) {
                        if (tmp_node->children[c] == NULL) {
                                mli_check(
                                        tree->nodes[node_idx].children[c] == 0,
                                        "Expected node's child == "
                                        "0 when tmp_node's child == NULL");
                        } else {
                                if (tmp_node->children[c]->num_objects == 0) {
                                        mli_check(
                                                tree->nodes[node_idx]
                                                                .children[c] ==
                                                        0,
                                                "Expected node's child != "
                                                "0 when tmp_node's child != "
                                                "NULL");
                                }
                        }
                }

                for (c = 0; c < 8u; c++) {
                        if (tmp_node->children[c] != NULL) {
                                uint64_t child_node_idx =
                                        tree->nodes[node_idx].children[c];
                                int32_t child_node_type =
                                        tree->nodes[node_idx].types[c];
                                mli_check(
                                        _mliOcTree_equal_payload(
                                                tree,
                                                child_node_idx,
                                                child_node_type,
                                                tmp_node->children[c]),
                                        "Expected tree to be euqal further "
                                        "down");
                        }
                }
        } else if (node_type == MLI_OCTREE_TYPE_NONE) {

        } else {
                mli_sentinel("node_idx must be either node, leaf or none");
        }

        return 1;
error:
        return 0;
}

int mliOcTree_equal_payload(
        const struct mliOcTree *tree,
        const struct mliTmpOcTree *tmp_octree)
{
        int32_t root_node_idx = 0;
        int32_t root_node_type = MLI_OCTREE_TYPE_NODE;
        mli_check(
                mliCube_is_equal(tree->cube, tmp_octree->cube),
                "Cubes are not equal");
        mli_check(
                _mliOcTree_equal_payload(
                        tree, root_node_idx, root_node_type, &tmp_octree->root),
                "Tree is not equal");
        return 1;
error:
        return 0;
}

void _mliOcTree_print(
        const struct mliOcTree *tree,
        const int32_t node_idx,
        const uint8_t node_type,
        const uint32_t indent,
        const uint32_t child)
{
        uint32_t i;
        uint32_t c;
        for (i = 0u; i < indent; i++)
                printf(" ");
        if (node_type == MLI_OCTREE_TYPE_NONE) {
                printf("|-Leaf[%d, %d] %u: %u []", -1, -1, child, 0);
        } else if (node_type == MLI_OCTREE_TYPE_LEAF) {
                int32_t leaf_idx = node_idx;
                uint32_t j;
                assert(leaf_idx < (int32_t)tree->leafs.num_leafs);
                printf("|-Leaf[%d, %d] %u: %u [",
                       -1,
                       leaf_idx,
                       child,
                       tree->leafs.adresses[leaf_idx].num_object_links);
                for (j = 0; j < tree->leafs.adresses[leaf_idx].num_object_links;
                     j++) {
                        int32_t l = j + tree->leafs.adresses[leaf_idx]
                                                .first_object_link;
                        printf("%u, ", tree->leafs.object_links[l]);
                }
                printf("]");
        } else if (node_type == MLI_OCTREE_TYPE_NODE) {
                printf("Node[%d, %d]: %u", node_idx, -1, child);
        }
        printf("\n");
        for (c = 0u; c < 8u; c++) {
                if (node_type == MLI_OCTREE_TYPE_NODE) {
                        int32_t child_node_idx;
                        int32_t child_node_type;
                        assert(node_idx < (int32_t)tree->num_nodes);
                        child_node_idx = tree->nodes[node_idx].children[c];
                        child_node_type = tree->nodes[node_idx].types[c];
                        if (child_node_type != MLI_OCTREE_TYPE_NONE) {
                                _mliOcTree_print(
                                        tree,
                                        child_node_idx,
                                        child_node_type,
                                        indent + 2,
                                        c);
                        }
                }
        }
}

void mliOcTree_print(const struct mliOcTree *tree)
{
        if (tree->num_nodes > 0) {
                int32_t root_idx = 0;
                _mliOcTree_print(tree, root_idx, tree->root_type, 0u, 0u);
        }
}
