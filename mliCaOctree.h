/* Copyright 2019 Sebastian Achim Mueller */
#ifndef MERLICT_MLICAOCTTREE_H_
#define MERLICT_MLICAOCTTREE_H_

#include <stdint.h>
#include "mliOcTree.h"
#include "mliDynArray_template.h"

struct mliLeafAddress {
        uint32_t first_object_link;
        uint32_t num_object_links;
};

struct mliLeafAddress mliLeafAddress_init() {
        struct mliLeafAddress address;
        address.first_object_link = 0u;
        address.num_object_links = 0u;
        return address;
}

struct mliLeafArray {
        size_t num_leafs;
        struct mliLeafAddress *adresses;
        size_t num_object_links;
        uint32_t *object_links;
};

struct mliLeafArray mliLeafArray_init()
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
        const size_t num_leafs,
        const size_t num_object_links)
{
        size_t i;
        mliLeafArray_free(leafs);
        leafs->num_leafs = num_leafs;
        mli_malloc(
                leafs->adresses,
                struct mliLeafAddress,
                leafs->num_leafs);
        for (i = 0; i < leafs->num_leafs; i++) {
                leafs->adresses[i] = mliLeafAddress_init();
        }
        leafs->num_object_links = num_object_links;
        mli_malloc(
                leafs->object_links,
                uint32_t,
                leafs->num_object_links);
        for (i = 0; i < leafs->num_object_links; i++) {
                leafs->object_links[i] = 0u;
        }
        return 1;
error:
        return 0;
}

#define MLI_OCTREE_TYPE_NONE 0
#define MLI_OCTREE_TYPE_NODE 1
#define MLI_OCTREE_TYPE_LEAF 2

struct mliCa2Node {
        uint32_t children[8];
        uint8_t types[8];
};

struct mliCa2Node mliCa2Node_init()
{
        size_t c = 0;
        struct mliCa2Node node;
        for (c = 0; c < 8u; c++) {
                node.children[c] = 0u;
                node.types[c] = MLI_OCTREE_TYPE_NONE;
        }
        return node;
}

struct mliCa2Octree {
        struct mliCube cube;
        size_t num_nodes;
        struct mliCa2Node *nodes;
        struct mliLeafArray leafs;
        uint8_t root_type;
};

struct mliCa2Octree mliCa2Octree_init()
{
        struct mliCa2Octree tree;
        tree.cube.lower = mliVec_set(0., 0., 0.);
        tree.cube.edge_length = 0.;
        tree.num_nodes = 0u;
        tree.nodes = NULL;
        tree.leafs = mliLeafArray_init();
        tree.root_type = MLI_OCTREE_TYPE_NONE;
        return tree;
}

void mliCa2Octree_free(struct mliCa2Octree* tree)
{
        free(tree->nodes);
        mliLeafArray_free(&tree->leafs);
        *tree = mliCa2Octree_init();
}

int mliCa2Octree_malloc(
        struct mliCa2Octree* tree,
        const size_t num_nodes,
        const size_t num_leafs,
        const size_t num_object_links)
{
        size_t i;
        mliCa2Octree_free(tree);
        tree->num_nodes = num_nodes;
        mli_malloc(tree->nodes, struct mliCa2Node, tree->num_nodes);
        for (i = 0; i < tree->num_nodes; i++)
                tree->nodes[i] = mliCa2Node_init();
        mli_c(mliLeafArray_malloc(&tree->leafs, num_leafs, num_object_links));
        return 1;
error:
        return 0;
}

void _mliCa2Octree_set_node(
        struct mliCa2Octree* tree,
        const struct mliTmpNode *dynnode)
{
        size_t c;
        size_t i = dynnode->node_index;
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

void _mliCa2Octree_set_leaf(
        struct mliCa2Octree* tree,
        const struct mliTmpNode *dynnode,
        size_t *object_link_size)
{
        size_t o;
        size_t i = dynnode->leaf_index;
        assert(i < tree->leafs.num_leafs);
        tree->leafs.adresses[i].first_object_link = *object_link_size;
        tree->leafs.adresses[i].num_object_links = dynnode->num_objects;
        *object_link_size += dynnode->num_objects;
        for (o = 0; o < dynnode->num_objects; o++) {
                size_t l = tree->leafs.adresses[i].first_object_link + o;
                assert(l < tree->leafs.num_object_links);
                tree->leafs.object_links[l] = dynnode->objects[o];
        }
}

void _mliCa2Octree_set(
        struct mliCa2Octree* tree,
        const struct mliTmpNode *dynnode,
        size_t *object_link_size)
{
        size_t c;
        if (dynnode->node_index >= 0) {
                _mliCa2Octree_set_node(tree, dynnode);
        } else if (dynnode->leaf_index >= 0) {
                _mliCa2Octree_set_leaf(tree, dynnode, object_link_size);
        }

        for (c = 0; c < 8u; c++) {
                if (dynnode->children[c] != NULL) {
                        _mliCa2Octree_set(
                                tree,
                                dynnode->children[c],
                                object_link_size);
                }
        }
}

void mliCa2Octree_set(
        struct mliCa2Octree* tree,
        const struct mliTmpOcTree *dyntree)
{
        size_t object_link_size = 0u;
        tree->cube = dyntree->cube;
        if (mliTmpNode_num_children(&dyntree->root) > 0) {
                tree->root_type = MLI_OCTREE_TYPE_NODE;
        } else {
                tree->root_type = MLI_OCTREE_TYPE_LEAF;
        }

        _mliCa2Octree_set(tree, &dyntree->root, &object_link_size);
}

size_t mliCa2Octree_node_num_children(
        const struct mliCa2Octree* tree,
        const size_t node_idx)
{
        size_t num = 0u;
        size_t c;
        for (c = 0; c < 8u; c++) {
                if (tree->nodes[node_idx].types[c] > MLI_OCTREE_TYPE_NONE) {
                        num++;
                }
        }
        return num;
}

size_t mliCa2Octree_leaf_num_objects(
        const struct mliCa2Octree* tree,
        const size_t leaf)
{
        assert(leaf < tree->leafs.num_leafs);
        return tree->leafs.adresses[leaf].num_object_links;
}

uint32_t mliCa2Octree_leaf_object_link(
        const struct mliCa2Octree* tree,
        const size_t leaf,
        const size_t object_link)
{
        size_t i;
        assert(leaf < tree->leafs.num_leafs);
        assert(object_link < mliCa2Octree_leaf_num_objects(tree, leaf));
        i = tree->leafs.adresses[leaf].first_object_link + object_link;
        return tree->leafs.object_links[i];
}

int mliCa2Octree_malloc_from_scenery(
        struct mliCa2Octree *octree,
        const struct mliScenery *scenery)
{
        size_t num_nodes = 0;
        size_t num_leafs = 0;
        size_t num_object_links = 0;
        struct mliTmpOcTree tmp_octree = mliTmpOcTree_init();
        mli_check(mliTmpOcTree_malloc_from_scenery(&tmp_octree, scenery),
                "Failed to create dynamic, and temporary TmpOcTree "
                "from scenery");
        mliTmpNode_set_flat_index(&tmp_octree.root);
        mliTmpNode_num_nodes_leafs_objects(
                &tmp_octree.root,
                &num_nodes,
                &num_leafs,
                &num_object_links);

        mliCa2Octree_free(octree);
        mli_check(mliCa2Octree_malloc(
                octree,
                num_nodes,
                num_leafs,
                num_object_links),
                "Failed to allocate cache-aware octree from dynamic octree.");
        mliCa2Octree_set(octree, &tmp_octree);
        mliTmpOcTree_free(&tmp_octree);

        return 1;
error:
        return 0;
}

int _mliCa2Octree_equal_payload(
        const struct mliCa2Octree *tree,
        const int32_t node_idx,
        const int32_t node_type,
        const struct mliTmpNode *dynnode)
{
        if (node_type == MLI_OCTREE_TYPE_LEAF) {
                /* leaf */
                size_t leaf_idx;
                size_t obj;
                mli_check(
                        mliTmpNode_num_children(dynnode) == 0,
                        "Expect dynnode to have 0 children when "
                        "node_type == LEAF.");
                leaf_idx = node_idx;
                mli_check(
                        leaf_idx < tree->leafs.num_leafs,
                        "The leaf_idx is out of range.");
                mli_check(
                        tree->leafs.adresses[leaf_idx].num_object_links ==
                        dynnode->num_objects,
                        "Expected leafs to have equal num_object_links.");
                for (obj = 0; obj < dynnode->num_objects; obj++) {
                        size_t l = obj +
                                tree->leafs.adresses[leaf_idx].
                                        first_object_link;
                        mli_check(
                                tree->leafs.object_links[l] ==
                                dynnode->objects[obj],
                                "Expected object_links in leaf to be equal.");
                }
                mli_check(
                        tree->leafs.adresses[leaf_idx].num_object_links ==
                        dynnode->num_objects,
                        "Expected leafs to have equal num_object_links.");
        } else if (node_type == MLI_OCTREE_TYPE_NODE) {
                /* node */
                size_t c;
                mli_check(
                        node_idx >= 0,
                        "This node_idx is expected to point to a node.");
                for (c = 0; c < 8u; c++) {
                        if (dynnode->children[c] == NULL) {
                                mli_check(
                                        tree->nodes[node_idx].children[c] == 0,
                                        "Expected node's child == "
                                        "0 when dynnode's child == NULL");
                        } else {
                                if (dynnode->children[c]->num_objects == 0) {
                                        mli_check(
                                                tree->nodes[node_idx].
                                                        children[c] == 0,
                                                "Expected node's child != "
                                                "0 when dynnode's child != "
                                                "NULL");
                                }
                        }
                }

                for (c = 0; c < 8u; c++) {
                        if (dynnode->children[c] != NULL) {
                                size_t child_node_idx =
                                        tree->nodes[node_idx].children[c];
                                int32_t child_node_type =
                                        tree->nodes[node_idx].types[c];
                                mli_check(
                                        _mliCa2Octree_equal_payload(
                                                tree,
                                                child_node_idx,
                                                child_node_type,
                                                dynnode->children[c]),
                                        "Expected tree to be euqal further down"
                                        );
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

int mliCa2Octree_equal_payload(
        const struct mliCa2Octree *tree,
        const struct mliTmpOcTree *dyntree)
{
        int32_t root_node_idx = 0;
        int32_t root_node_type = MLI_OCTREE_TYPE_NODE;
        mli_check(mliCube_is_equal(tree->cube, dyntree->cube),
                "Cubes are not equal");
        mli_check(_mliCa2Octree_equal_payload(
                tree,
                root_node_idx,
                root_node_type,
                &dyntree->root),
                "Tree is not equal");
        return 1;
error:
        return 0;
}

void _mliCa2Octree_print(
        const struct mliCa2Octree *tree,
        const int32_t node_idx,
        const uint8_t node_type,
        const int32_t indent,
        const uint32_t child)
{
        uint32_t i;
        uint32_t c;
        for (i = 0u; i < indent; i++) printf(" ");
        if (node_type == MLI_OCTREE_TYPE_NONE) {
                printf(
                        "|-Leaf[%d, %d] %u: %u []",
                        -1,
                        -1,
                        child,
                        0);
        } else if (node_type == MLI_OCTREE_TYPE_LEAF) {
                int32_t leaf_idx = node_idx;
                uint32_t j;
                assert(leaf_idx < tree->leafs.num_leafs);
                printf(
                        "|-Leaf[%d, %d] %u: %u [",
                        -1,
                        leaf_idx,
                        child,
                        tree->leafs.adresses[leaf_idx].num_object_links);
                for(
                        j = 0;
                        j < tree->leafs.adresses[leaf_idx].num_object_links;
                        j++)
                {
                        int32_t l = j + tree->leafs.adresses[
                                                leaf_idx].first_object_link;
                        printf("%u, ", tree->leafs.object_links[l]);
                }
                printf("]");
        } else if (node_type == MLI_OCTREE_TYPE_NODE) {
                printf(
                        "Node[%d, %d]: %u",
                        node_idx,
                        -1,
                        child);
        }
        printf("\n");
        for (c = 0u; c < 8u; c++) {
                if (node_type == MLI_OCTREE_TYPE_NODE) {
                        int32_t child_node_idx;
                        int32_t child_node_type;
                        assert(node_idx < tree->num_nodes);
                        child_node_idx = tree->nodes[node_idx].children[c];
                        child_node_type = tree->nodes[node_idx].types[c];
                        if (child_node_type != MLI_OCTREE_TYPE_NONE) {
                                _mliCa2Octree_print(
                                        tree,
                                        child_node_idx,
                                        child_node_type,
                                        indent + 2,
                                        c);
                        }
                }
        }
}

void mliCa2Octree_print(const struct mliCa2Octree *tree)
{
        if (tree->num_nodes > 0) {
                int32_t root_idx = 0;
                _mliCa2Octree_print(
                        tree,
                        root_idx,
                        tree->root_type,
                        0u,
                        0u);
        }
}

#endif
