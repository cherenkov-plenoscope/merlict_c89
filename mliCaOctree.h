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








struct mliCa2Node {
        int32_t children[8];
};

struct mliCa2Node mliCa2Node_init()
{
        size_t c = 0;
        struct mliCa2Node node;
        for (c = 0; c < 8u; c++) {
                node.children[c] = 0;
        }
        return node;
}

struct mliCa2Octree {
        struct mliCube cube;

        size_t num_nodes;
        struct mliCa2Node *nodes;

        struct mliLeafArray leafs;
};

struct mliCa2Octree mliCa2Octree_init()
{
        struct mliCa2Octree tree;
        tree.cube.lower = mliVec_set(0., 0., 0.);
        tree.cube.edge_length = 0.;
        tree.num_nodes = 0u;
        tree.nodes = NULL;
        tree.leafs = mliLeafArray_init();
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
        const struct mliNode *dynnode)
{
        size_t c;
        size_t i = dynnode->node_index;
        assert(i < tree->num_nodes);
        for (c = 0; c < 8u; c++) {
                if (dynnode->children[c] != NULL) {
                        if (dynnode->children[c]->node_index >= 0) {
                                tree->nodes[i].children[c] =
                                        dynnode->children[c]->node_index;
                        } else if (dynnode->children[c]->leaf_index >= 0) {
                                tree->nodes[i].children[c] =
                                        -dynnode->children[c]->leaf_index;
                        } else {
                                tree->nodes[i].children[c] = 0;
                        }
                } else {
                        tree->nodes[i].children[c] = 0;
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

        fprintf(stderr, "%s, %d\n", __FILE__, __LINE__);
        _mliCa2Octree_set(tree, &dyntree->root, &object_link_size);
}

int _mliCa2Octree_equal_payload(
        const struct mliCa2Octree *tree,
        const int32_t node_idx,
        const struct mliNode *dynnode)
{
        fprintf(stderr, "%s, %d\n", __FILE__, __LINE__);
        if (mliNode_num_children(dynnode) == 0u) {
                fprintf(stderr, "%s, %d\n", __FILE__, __LINE__);
                /* leaf */
                size_t leaf_idx;
                size_t obj;
                mli_check(
                        node_idx < 0,
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
                        fprintf(stderr, "%u", dynnode->objects[obj]);
                }
                fprintf(stderr, "\n");
                mli_check(
                        tree->leafs.adresses[leaf_idx].num_object_links ==
                        dynnode->num_objects,
                        "Expected leafs to have equal num_object_links.");
        } else {
                fprintf(stderr, "%s, %d\n", __FILE__, __LINE__);
                /* node */
                size_t c;
                mli_check(
                        node_idx >= 0,
                        "This node_idx is expected to point to a node.");
                for (c = 0; c < 8u; c++) {
                        if (tree->nodes[node_idx].children[c] == 0) {
                                mli_check(
                                        dynnode->children[c] == NULL,
                                        "Expected dynnode's child to be "
                                        "NULL when node's child is 0");
                        }
                        if (dynnode->children[c] == NULL) {
                                mli_check(
                                        tree->nodes[node_idx].children[c] == 0,
                                        "Expected node's child to be "
                                        "0 when dynnode's child is NULL");
                        }
                }

                for (c = 0; c < 8u; c++) {
                        if (dynnode->children[c] == NULL) {
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
        fprintf(stderr, "%s, %d\n", __FILE__, __LINE__);
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














struct mliCaNode {
        int32_t children[8];
        int32_t objects;
        uint32_t num_objects;
};

MLIDYNARRAY_TEMPLATE(mli, CaNode, struct mliCaNode)


struct mliCaOctree {
        struct mliCube cube;
        struct mliDynUint32 objects;
        struct mliDynCaNode nodes;
};

struct mliCaOctree mliCaOctree_init()
{
        struct mliCaOctree caoctree;
        caoctree.cube.lower = mliVec_set(0., 0., 0.);
        caoctree.cube.edge_length = 0.;

        caoctree.nodes = mliDynCaNode_init();
        caoctree.objects = mliDynUint32_init();
        return caoctree;
}

void mliCaOctree_free(struct mliCaOctree* caoctree)
{
        mliDynCaNode_free(&caoctree->nodes);
        mliDynUint32_free(&caoctree->objects);
        *caoctree = mliCaOctree_init();
}

int mliCaOctree_malloc(
        struct mliCaOctree* caoctree)
{
        mliCaOctree_free(caoctree);
        mli_c(mliDynCaNode_malloc(&caoctree->nodes, 0u));
        mli_c(mliDynUint32_malloc(&caoctree->objects, 0u));
        return 1;
error:
        return 0;
}

int _mliCaOctree_feed(struct mliCaOctree* caoctree, const struct mliNode *node)
{
        struct mliCaNode canode;
        size_t c, num_children;
        num_children = 0;

        if (mliNode_num_children(node) == 0 && node->num_objects == 0)
                return 1;

        for (c = 0; c < 8u; c++) {
                if (node->children[c] == NULL) {
                        canode.children[c] = -1;
                } else {
                        if (node->children[c]->num_objects > 0) {
                                canode.children[c] = node->children[c]->flat_index;
                                num_children++;
                        } else {
                                canode.children[c] = -1;
                        }
                }
        }
        if (num_children == 0) {
                /* Leaf */
                size_t obj;
                canode.objects = caoctree->objects.dyn.size;
                canode.num_objects = node->num_objects;
                for (obj = 0; obj < node->num_objects; obj++) {
                        mli_c(mliDynUint32_push_back(&
                                caoctree->objects,
                                node->objects[obj]))
                }
        } else {
                /* Node */
                canode.objects = -1;
                canode.num_objects = 0u;
        }

        mli_c(mliDynCaNode_push_back(
                &caoctree->nodes,
                canode));

        for (c = 0; c < 8u; c++) {
                if (node->children[c] != NULL) {
                        mli_c(_mliCaOctree_feed(caoctree, node->children[c]));
                }
        }

        return 1;
error:
        return 0;
}

int mliCaOctree_set_with_dynamic_octree(
        struct mliCaOctree* caoctree,
        const struct mliOcTree* dynoctree)
{
        caoctree->cube = dynoctree->cube;


        mli_c(_mliCaOctree_feed(caoctree, &dynoctree->root));
        return 1;
error:
        return 0;
}

/*
         3
     1--{
    /    4
0--{
   \    5
    2--{
        6

                                        0
                        1                               2
                3              (4)              5               6
               7 8                             9 10           11 12

0, 1, 2, 3, 4, 5, 6

*/

void mliCaOctree_print(const struct mliCaOctree* caoctree)
{
        size_t n, c;
        for (n = 0; n < caoctree->nodes.dyn.size; n++) {
                printf("_node_[% 6ld]\n", n);
                for (c = 0; c < 8u; c++) {
                        printf("  c % 6d\n", caoctree->nodes.arr[n].children[c]);
                }
                printf("  num. obj %u\n", caoctree->nodes.arr[n].num_objects);
        }
}

#endif
