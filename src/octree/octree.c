/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "octree.h"
#include <assert.h>
#include "../object/object_AABB.h"
#include "../geometry/geometry_aabb.h"
#include "../geometry/geometry_and_accelerator.h"
#include "../math/math.h"

/*
 * mli_OcTree
 * =========
 *
 * A cache-aware mli_OcTree to accelerate the traversal.
 * mli_OcTree is created from the dynamic, mli_octree_TmpOcTree.
 *
 * Structure of mli_octree_TmpOcTree
 * -------------------------
 *
 *                               mli_octree_TmpNode(0,-)
 *                                    |-objects[a,b,c,d,e,f,g,h]
 *                     _______________|________________
 *                    |                                |
 *                mli_octree_TmpNode(1,-) mli_octree_TmpNode(2,-)
 *                    |objects[a,b,c,d]                |objects[e,f,g,h]
 *             _______|_______                 ________|________
 *            |               |               |                 |
 *        mli_octree_TmpNode(3,0) mli_octree_TmpNode(4,1)
 * mli_octree_TmpNode(5,2)   mli_octree_TmpNode(6,3) |objects[a,b,c]
 * |objects[c,d]  |objects[e,f]     |objects[f,g,h]
 *
 *      mli_octree_TmpNode link to each other via pointer.
 *      Each mli_octree_TmpNode is allocated separately.
 *
 *      advantages:
 *              easy to grow and fill
 *      disadvantage:
 *              memory is scattered, difficult to serialize
 *
 * Structure of mli_OcTree
 * ----------------------
 *
 *                object_links w.r.t. mli_Geometry ->
 *          leafs +-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+
 *            |---|  a  |  b  |  c  |  c  |  d  |  e  |  f  |  f  |  g  |  h  |
 *            |   +-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+
 * mli_OcTree  |   0     1     2     3     4     5     6     7     8     9 10 |
 * |   ^                 ^           ^           ^ |         |
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
 *            |  1  |  0  |  2  |                  w.r.t. mli_OcTree.nodes
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

struct mli_octree_LeafAddress mli_octree_LeafAddress_init(void)
{
        struct mli_octree_LeafAddress address;
        address.first_object_link = 0u;
        address.num_object_links = 0u;
        return address;
}

struct mli_octree_LeafArray mli_octree_LeafArray_init(void)
{
        struct mli_octree_LeafArray leafs;
        leafs.num_leafs = 0;
        leafs.adresses = NULL;
        leafs.num_object_links = 0u;
        leafs.object_links = NULL;
        return leafs;
}

void mli_octree_LeafArray_free(struct mli_octree_LeafArray *leafs)
{
        free(leafs->object_links);
        free(leafs->adresses);
        *leafs = mli_octree_LeafArray_init();
}

chk_rc mli_octree_LeafArray_malloc(
        struct mli_octree_LeafArray *leafs,
        const uint64_t num_leafs,
        const uint64_t num_object_links)
{
        uint64_t i;
        mli_octree_LeafArray_free(leafs);
        leafs->num_leafs = num_leafs;
        chk_malloc(
                leafs->adresses,
                struct mli_octree_LeafAddress,
                leafs->num_leafs);
        for (i = 0; i < leafs->num_leafs; i++) {
                leafs->adresses[i] = mli_octree_LeafAddress_init();
        }
        leafs->num_object_links = num_object_links;
        chk_malloc(leafs->object_links, uint32_t, leafs->num_object_links);
        for (i = 0; i < leafs->num_object_links; i++) {
                leafs->object_links[i] = 0u;
        }
        return CHK_SUCCESS;
chk_error:
        return CHK_FAIL;
}

struct mli_octree_Node mli_octree_Node_init(void)
{
        uint64_t c = 0;
        struct mli_octree_Node node;
        for (c = 0; c < 8u; c++) {
                node.children[c] = 0u;
                node.types[c] = MLI_OCTREE_TYPE_NONE;
        }
        return node;
}

struct mli_OcTree mli_OcTree_init(void)
{
        struct mli_OcTree tree;
        tree.cube.lower = mli_Vec_set(0., 0., 0.);
        tree.cube.edge_length = 0.;
        tree.num_nodes = 0u;
        tree.nodes = NULL;
        tree.leafs = mli_octree_LeafArray_init();
        tree.root_type = MLI_OCTREE_TYPE_NONE;
        return tree;
}

void mli_OcTree_free(struct mli_OcTree *tree)
{
        free(tree->nodes);
        mli_octree_LeafArray_free(&tree->leafs);
        *tree = mli_OcTree_init();
}

chk_rc mli_OcTree_malloc(
        struct mli_OcTree *tree,
        const uint64_t num_nodes,
        const uint64_t num_leafs,
        const uint64_t num_object_links)
{
        uint64_t i;
        mli_OcTree_free(tree);
        tree->num_nodes = num_nodes;
        chk_malloc(tree->nodes, struct mli_octree_Node, tree->num_nodes);
        for (i = 0; i < tree->num_nodes; i++)
                tree->nodes[i] = mli_octree_Node_init();
        chk(mli_octree_LeafArray_malloc(
                &tree->leafs, num_leafs, num_object_links));
        return CHK_SUCCESS;
chk_error:
        return CHK_FAIL;
}

void mli_OcTree_set_node(
        struct mli_OcTree *tree,
        const struct mli_octree_TmpNode *dynnode)
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

void mli_OcTree_set_leaf(
        struct mli_OcTree *tree,
        const struct mli_octree_TmpNode *dynnode,
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

void mli_OcTree_set_walk(
        struct mli_OcTree *tree,
        const struct mli_octree_TmpNode *dynnode,
        uint64_t *object_link_size)
{
        uint64_t c;
        if (dynnode->node_index >= 0) {
                mli_OcTree_set_node(tree, dynnode);
        } else if (dynnode->leaf_index >= 0) {
                mli_OcTree_set_leaf(tree, dynnode, object_link_size);
        }

        for (c = 0; c < 8u; c++) {
                if (dynnode->children[c] != NULL) {
                        mli_OcTree_set_walk(
                                tree, dynnode->children[c], object_link_size);
                }
        }
}

void mli_OcTree_set(
        struct mli_OcTree *tree,
        const struct mli_octree_TmpOcTree *dyntree)
{
        uint64_t object_link_size = 0u;
        tree->cube = dyntree->cube;
        if (mli_octree_TmpNode_num_children(&dyntree->root) > 0) {
                tree->root_type = MLI_OCTREE_TYPE_NODE;
        } else {
                tree->root_type = MLI_OCTREE_TYPE_LEAF;
        }

        mli_OcTree_set_walk(tree, &dyntree->root, &object_link_size);
}

uint64_t mli_OcTree_node_num_children(
        const struct mli_OcTree *tree,
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

uint64_t mli_OcTree_leaf_num_objects(
        const struct mli_OcTree *tree,
        const uint64_t leaf)
{
        return tree->leafs.adresses[leaf].num_object_links;
}

uint32_t mli_OcTree_leaf_object_link(
        const struct mli_OcTree *tree,
        const uint64_t leaf,
        const uint64_t object_link)
{
        uint64_t i = tree->leafs.adresses[leaf].first_object_link + object_link;
        return tree->leafs.object_links[i];
}

mli_bool mli_OcTree_equal_payload_walk(
        const struct mli_OcTree *tree,
        const int32_t node_idx,
        const int32_t node_type,
        const struct mli_octree_TmpNode *tmp_node)
{
        if (node_type == MLI_OCTREE_TYPE_LEAF) {
                /* leaf */
                uint64_t leaf_idx;
                uint64_t obj;
                chk_msg(mli_octree_TmpNode_num_children(tmp_node) == 0,
                        "Expect tmp_node to have 0 children when "
                        "node_type == LEAF.");
                leaf_idx = node_idx;
                chk_msg(leaf_idx < tree->leafs.num_leafs,
                        "The leaf_idx is out of range.");
                chk_msg(tree->leafs.adresses[leaf_idx].num_object_links ==
                                tmp_node->num_objects,
                        "Expected leafs to have equal num_object_links.");
                for (obj = 0; obj < tmp_node->num_objects; obj++) {
                        uint64_t l = obj + tree->leafs.adresses[leaf_idx]
                                                   .first_object_link;
                        chk_msg(tree->leafs.object_links[l] ==
                                        tmp_node->objects[obj],
                                "Expected object_links in leaf to be equal.");
                }
                chk_msg(tree->leafs.adresses[leaf_idx].num_object_links ==
                                tmp_node->num_objects,
                        "Expected leafs to have equal num_object_links.");
        } else if (node_type == MLI_OCTREE_TYPE_NODE) {
                /* node */
                uint64_t c;
                chk_msg(node_idx >= 0,
                        "This node_idx is expected to point to a node.");
                for (c = 0; c < 8u; c++) {
                        if (tmp_node->children[c] == NULL) {
                                chk_msg(tree->nodes[node_idx].children[c] == 0,
                                        "Expected node's child == "
                                        "0 when tmp_node's child == NULL");
                        } else {
                                if (tmp_node->children[c]->num_objects == 0) {
                                        chk_msg(tree->nodes[node_idx]
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
                                chk_msg(mli_OcTree_equal_payload_walk(
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
                chk_bad("node_idx must be either node, leaf or none");
        }

        return CHK_SUCCESS;
chk_error:
        return CHK_FAIL;
}

mli_bool mli_OcTree_equal_payload(
        const struct mli_OcTree *tree,
        const struct mli_octree_TmpOcTree *tmp_octree)
{
        int32_t root_node_idx = 0;
        int32_t root_node_type = MLI_OCTREE_TYPE_NODE;
        chk_msg(mli_Cube_equal(tree->cube, tmp_octree->cube),
                "Cubes are not equal");
        chk_msg(mli_OcTree_equal_payload_walk(
                        tree, root_node_idx, root_node_type, &tmp_octree->root),
                "Tree is not equal");
        return CHK_SUCCESS;
chk_error:
        return CHK_FAIL;
}

void mli_OcTree_print_walk(
        const struct mli_OcTree *tree,
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
                                mli_OcTree_print_walk(
                                        tree,
                                        child_node_idx,
                                        child_node_type,
                                        indent + 2,
                                        c);
                        }
                }
        }
}

void mli_OcTree_print(const struct mli_OcTree *tree)
{
        printf("__ mli_OcTree __\n");
        printf("- num_nodes %u\n", (uint32_t)tree->num_nodes);
        printf("- num_leafs %u\n", (uint32_t)tree->leafs.num_leafs);
        printf("- root_type %d\n", tree->root_type);
        printf("- cube.lower %.1f, %.1f, %.1f\n",
               tree->cube.lower.x,
               tree->cube.lower.y,
               tree->cube.lower.z);
        printf("- cube.edge_length %.1f\n", tree->cube.edge_length);
        if (tree->num_nodes > 0) {
                int32_t root_idx = 0;
                mli_OcTree_print_walk(tree, root_idx, tree->root_type, 0u, 0u);
        }
}

chk_rc mli_OcTree_malloc_from_object_wavefront(
        struct mli_OcTree *octree,
        const struct mli_Object *object)
{
        uint64_t num_nodes = 0;
        uint64_t num_leafs = 0;
        uint64_t num_object_links = 0;
        struct mli_octree_TmpOcTree tmp_octree = mli_octree_TmpOcTree_init();
        chk_msg(mli_octree_TmpOcTree_malloc_from_bundle(
                        &tmp_octree,
                        (const void *)object,
                        object->num_faces,
                        mli_Object_face_in_local_frame_has_overlap_aabb_void,
                        mli_Object_aabb_in_local_frame(object)),
                "Failed to create dynamic, and temporary TmpOcTree "
                "from mli_Object");
        mli_octree_TmpNode_set_flat_index(&tmp_octree.root);
        mli_octree_TmpNode_num_nodes_leafs_objects(
                &tmp_octree.root, &num_nodes, &num_leafs, &num_object_links);

        chk_msg(mli_OcTree_malloc(
                        octree, num_nodes, num_leafs, num_object_links),
                "Failed to allocate cache-aware octree from dynamic octree.");
        mli_OcTree_set(octree, &tmp_octree);
        mli_octree_TmpOcTree_free(&tmp_octree);

        return CHK_SUCCESS;
chk_error:
        return CHK_FAIL;
}

chk_rc mli_OcTree_malloc_from_Geometry(
        struct mli_OcTree *octree,
        const struct mli_GeometryAndAccelerator *accgeo,
        const struct mli_AABB outermost_aabb)
{
        uint64_t num_nodes = 0;
        uint64_t num_leafs = 0;
        uint64_t num_object_links = 0;
        struct mli_octree_TmpOcTree tmp_octree = mli_octree_TmpOcTree_init();
        chk_msg(mli_octree_TmpOcTree_malloc_from_bundle(
                        &tmp_octree,
                        (const void *)accgeo,
                        accgeo->geometry->num_robjects,
                        mli_Geometry_robject_has_overlap_aabb_void,
                        outermost_aabb),
                "Failed to create dynamic, and temporary TmpOcTree "
                "from scenery(Geometry, Accelerator)");
        mli_octree_TmpNode_set_flat_index(&tmp_octree.root);
        mli_octree_TmpNode_num_nodes_leafs_objects(
                &tmp_octree.root, &num_nodes, &num_leafs, &num_object_links);

        chk_msg(mli_OcTree_malloc(
                        octree, num_nodes, num_leafs, num_object_links),
                "Failed to allocate cache-aware octree from dynamic octree.");
        mli_OcTree_set(octree, &tmp_octree);
        mli_octree_TmpOcTree_free(&tmp_octree);

        return CHK_SUCCESS;
chk_error:
        return CHK_FAIL;
}
