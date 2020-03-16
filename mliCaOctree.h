/* Copyright 2019 Sebastian Achim Mueller */
#ifndef MERLICT_MLICAOCTTREE_H_
#define MERLICT_MLICAOCTTREE_H_

#include <stdint.h>
#include "mliOcTree.h"
#include "mliDynArray_template.h"


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

/*

num_children 0 - J
object_link idx
j = 0 idx
j = 1 idx
.
.
.
j = J idx


*/

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
