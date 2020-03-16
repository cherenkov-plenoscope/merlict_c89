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

struct mliCaNodeChildren {
        int32_t children[8];
};

struct mliCaNodeChildren mliCaNodeChildren_init()
{
        size_t c = 0;
        struct mliCaNodeChildren ch;
        for (c = 0; c < 8u; c++) {
                ch.children[c] = -1;
        }
        return ch;
}

struct mliNodeArray {
        size_t num_nodes;
        struct mliCaNodeChildren *node_children;
};

struct mliNodeArray mliNodeArray_init()
{
        struct mliNodeArray node_array;
        node_array.num_nodes = 0u;
        node_array.node_children = NULL;
        return node_array;
}

void mliNodeArray_free(struct mliNodeArray *node_array)
{
        free(node_array->node_children);
        *node_array = mliNodeArray_init();
}

int mliNodeArray_malloc(
        struct mliNodeArray *node_array,
        const size_t num_nodes)
{
        mliNodeArray_free(node_array);
        node_array->num_nodes = num_nodes;
        mli_malloc(
                node_array->node_children,
                struct mliCaNodeChildren,
                node_array->num_nodes);
        return 1;
error:
        return 0;
}

#define MLI_OCTREE_TYPE_NONE 0
#define MLI_OCTREE_TYPE_NODE 1
#define MLI_OCTREE_TYPE_LEAF 2

struct mliCaNode {
        uint32_t idx;
        uint8_t type;
};

struct mliCaNode mliCaNode_init()
{
        struct mliCaNode node;
        node.idx = 0u;
        node.type = MLI_OCTREE_TYPE_NONE;
        return node;
}

struct mliCa2Octree {
        struct mliCube cube;

        size_t num_nodes;
        struct mliCaNode *nodes;

        struct mliLeafArray leafs;
        struct mliNodeArray node_children;
};

struct mliCa2Octree mliCa2Octree_init()
{
        struct mliCa2Octree tree;
        tree.cube.lower = mliVec_set(0., 0., 0.);
        tree.cube.edge_length = 0.;
        tree.num_nodes = 0u;
        tree.nodes = NULL;
        tree.leafs = mliLeafArray_init();
        tree.node_children = mliNodeArray_init();
        return tree;
}

void mliCa2Octree_free(struct mliCa2Octree* tree)
{
        free(tree->nodes);
        mliLeafArray_free(&tree->leafs);
        mliNodeArray_free(&tree->node_children);
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
        mli_malloc(tree->nodes, struct mliCaNode, tree->num_nodes);
        for (i = 0; i < tree->num_nodes; i++)
                tree->nodes[i] = mliCaNode_init();
        mli_c(mliLeafArray_malloc(&tree->leafs, num_leafs, num_object_links));
        mli_c(mliNodeArray_malloc(&tree->node_children, num_nodes));
        return 1;
error:
        return 0;
}

void _mliCa2Octree_set_node(
        struct mliCa2Octree* tree,
        const struct mliNode *dynnode)
{
        size_t c;
        size_t i = dynnode->node_index;
        assert(i < tree->num_nodes);

        tree->nodes[i].type = MLI_OCTREE_TYPE_NODE;
        tree->nodes[i].idx = dynnode->children[c]->node_index;
        tree->node_children[]
                


        for (c = 0; c < 8u; c++) {
                if (dynnode->children[c] != NULL) {
                        if (dynnode->children[c]->node_index >= 0) {

                                tree->nodes[i].types[c] = 
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
        const struct mliNode *dynnode,
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
        const struct mliNode *dynnode,
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
        const struct mliOcTree *dyntree)
{
        size_t object_link_size = 0u;
        tree->cube = dyntree->cube;
        _mliCa2Octree_set(tree, &dyntree->root, &object_link_size);
}





int _mli_node_idx_is_none(const int32_t node_idx)
{
        return node_idx == 0;
}

int _mli_node_idx_is_leaf(const int32_t node_idx)
{
        return node_idx < 0;
}

int _mli_node_idx_is_node(const int32_t node_idx)
{
        return node_idx >= 0;
}

int _mliCa2Octree_equal_payload(
        const struct mliCa2Octree *tree,
        const int32_t node_idx,
        const struct mliNode *dynnode)
{
        if (_mli_node_idx_is_leaf(node_idx)) {
                /* leaf */
                size_t leaf_idx;
                size_t obj;
                mli_check(
                        mliNode_num_children(dynnode) == 0,
                        "This node_idx is expected to point to a leaf.");
                leaf_idx = -node_idx;
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
                        fprintf(stderr, "%u, ", dynnode->objects[obj]);
                }
                fprintf(stderr, "\n");
                mli_check(
                        tree->leafs.adresses[leaf_idx].num_object_links ==
                        dynnode->num_objects,
                        "Expected leafs to have equal num_object_links.");
        } else if (_mli_node_idx_is_node(node_idx)) {
                /* node */
                size_t c;
                fprintf(stderr, "%s, %d\n", __FILE__, __LINE__);
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
                                                tree->nodes[node_idx].children[c] == 0,
                                                "Expected node's child != "
                                                "0 when dynnode's child != NULL");
                                }
                        }
                }

                for (c = 0; c < 8u; c++) {
                        if (dynnode->children[c] != NULL) {
                                size_t child_node_idx =
                                        tree->nodes[node_idx].children[c];
                                mli_check(
                                        _mliCa2Octree_equal_payload(
                                                tree,
                                                child_node_idx,
                                                dynnode->children[c]),
                                        "Expected tree to be euqal further down"
                                        );
                        }
                }
        } else if (_mli_node_idx_is_none(node_idx)) {

        } else {
                mli_sentinel("node_idx must be either node, leaf or none");
        }

        return 1;
error:
        return 0;
}

int mliCa2Octree_equal_payload(
        const struct mliCa2Octree *tree,
        const struct mliOcTree *dyntree)
{
        int32_t root_node_idx = 0;
        mli_check(mliCube_is_equal(tree->cube, dyntree->cube),
                "Cubes are not equal");
        mli_check(_mliCa2Octree_equal_payload(
                tree,
                root_node_idx,
                &dyntree->root),
                "Tree is not equal");
        fprintf(stderr, "%s, %d\n", __FILE__, __LINE__);
        return 1;
error:
        return 0;
}

/*


void _mliCa2Octree_print(
        const struct mliCa2Octree *tree,
        const struct mliCa2Node node,
        const uint32_t indent,
        const uint32_t child)
{
        uint32_t i;
        uint32_t c;
        for (i = 0u; i < indent; i++) printf(" ");
        if (node_idx == 0) {
                printf(
                        "|-Leaf[%d, %d] %u: %u []",
                        -1,
                        -1,
                        child,
                        0);
        } else if (node_idx < 0) {
                int32_t leaf_idx = -node_idx;
                uint32_t j;
                assert(leaf_idx < tree->leafs.num_leafs);
                printf(
                        "|-Leaf[%d, %d] %u: %u [",
                        -1,
                        leaf_idx,
                        child,
                        tree->leafs.adresses[leaf_idx].num_object_links);
                for(j = 0; j < tree->leafs.adresses[leaf_idx].num_object_links; j++) {
                        int32_t l = j + tree->leafs.adresses[leaf_idx].first_object_link;
                        printf("%u, ", tree->leafs.object_links[l]);
                }
                printf("]");
        } else {
                printf(
                        "Node[%d, %d]: %u",
                        node_idx,
                        -1,
                        child);
        }
        printf("\n");
        for (c = 0u; c < 8u; c++) {
                if (node_idx >= 0) {
                        int32_t child_node_idx;
                        assert(node_idx < tree->num_nodes);
                        child_node_idx = tree->nodes[node_idx].children[c];
                        if (child_node_idx != 0) {
                                _mliCa2Octree_print(tree, child_node_idx, indent + 2, c);
                        }
                }
        }
}


void mliCa2Octree_print(const struct mliCa2Octree *tree)
{
        int32_t root_node_idx = 0;
        if (tree->num_nodes > 0) {
                _mliCa2Octree_print(
                        tree,
                        root_node_idx,
                        tree->nodes[0],
                        0u);
        }
}


*/
#endif
