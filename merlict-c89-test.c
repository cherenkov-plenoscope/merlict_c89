/* Copyright 2019 Sebastian Achim Mueller                                     */
/* Compile with:                                                              */
/* gcc merlict-c89-test.c -o merlict-c89-test -std=c89 -lm -Wall -pedantic    */
#include <math.h>
#include <assert.h>
#include "merlict_c89.h"


int main(int argc, char *argv[]) {
    /* mliFrame */
    /* A scenery-tree */
    {
        mliFrame f = mliFrame_init();
        CHECK(f.mother == NULL);
        CHECK(f.children.size == 0u);
        CHECK(f.type == MLI_FRAME);
    }

    /* mliFrame */
    /* malloc frame */
    {
        mliFrame f = mliFrame_init();
        CHECK(mliFrame_malloc(&f, MLI_FRAME));
        mliFrame_free(&f);
        CHECK(f.mother == NULL);
        CHECK(f.children.size == 0u);
        CHECK(f.type == MLI_FRAME);
    }

    /* mliFrame */
    /* add two childs */
    {
        mliFrame* child1 = NULL;
        mliFrame* child2 = NULL;
        mliFrame mother = mliFrame_init();
        CHECK(mliFrame_malloc(&mother, MLI_FRAME));
        mother.id = 1337;

        child1 = mliFrame_add(&mother, MLI_FRAME);
        CHECK(child1);
        child1->id = 41;

        child2 = mliFrame_add(&mother, MLI_FRAME);
        CHECK(child2);
        child2->id = 42;

        CHECK(mother.children.size == 2);
        CHECK(&mother == (mliFrame*)child1->mother);
        CHECK(&mother == (mliFrame*)child2->mother);
        mliFrame_free(&mother);
    }

    /* mliFrame */
    /* add grand childs */
    {
        /*
                            |--child_00
                |--child_0--|
                |           |--child_01
        mother--|
                |           |--child_10
                |--child_1--|
                            |--child_11
        */
        mliFrame* child_0 = NULL;
        mliFrame* child_1 = NULL;
        mliFrame* child_00 = NULL;
        mliFrame* child_01 = NULL;
        mliFrame* child_10 = NULL;
        mliFrame* child_11 = NULL;
        mliFrame mother = mliFrame_init();
        CHECK(mliFrame_malloc(&mother, MLI_FRAME));
        mother.id = 1337;
        child_0 = mliFrame_add(&mother, MLI_FRAME);
        CHECK(child_0);
        child_0->id = 10;
        child_00 = mliFrame_add(child_0, MLI_FRAME);
        CHECK(child_00);
        child_00->id = 100;
        child_01 = mliFrame_add(child_0, MLI_FRAME);
        CHECK(child_01);
        child_01->id = 101;

        child_1 = mliFrame_add(&mother, MLI_FRAME);
        CHECK(child_1);
        child_1->id = 20;
        child_10 = mliFrame_add(child_1, MLI_FRAME);
        CHECK(child_10);
        child_10->id = 200;
        child_11 = mliFrame_add(child_1, MLI_FRAME);
        CHECK(child_11);
        child_11->id = 201;

        CHECK(mother.children.size == 2);
        CHECK(&mother == (mliFrame*)child_0->mother);
        CHECK(&mother == (mliFrame*)child_0->mother);

        CHECK(child_0->children.size == 2);
        CHECK(child_0 == (mliFrame*)child_00->mother);
        CHECK(child_0 == (mliFrame*)child_01->mother);

        CHECK(child_1->children.size == 2);
        CHECK(child_1 == (mliFrame*)child_10->mother);
        CHECK(child_1 == (mliFrame*)child_11->mother);
        /* mliFrame_print(&mother); */

        mliFrame_free(&mother);
    }

    /* mliFrame */
    /* basic mesh allocation and initialization */
    {
        mliFrame* child = NULL;
        mliFrame mother = mliFrame_init();
        CHECK(mliFrame_malloc(&mother, MLI_FRAME));
        mother.id = 1337;

        child = mliFrame_add(&mother, MLI_MESH);
        CHECK(child);
        CHECK(child->type == MLI_MESH);
        child->id = 42;
        child->primitive.mesh->num_vertices = 3;
        child->primitive.mesh->num_faces = 1;
        mliMesh_malloc(child->primitive.mesh);
        child->primitive.mesh->vertices[0] = mliVec_set(1., 0., 0.);
        child->primitive.mesh->vertices[0] = mliVec_set(0., 1., 0.);
        child->primitive.mesh->vertices[0] = mliVec_set(0., 0., 1.);
        child->primitive.mesh->faces[0] = mliFace_set(0, 1, 2);
        mliFrame_free(&mother);
    }

    /* mliFrame */
    /* basic sphere */
    {
        mliFrame* child = NULL;
        mliFrame mother = mliFrame_init();
        CHECK(mliFrame_malloc(&mother, MLI_FRAME));
        mother.id = 1337;

        child = mliFrame_add(&mother, MLI_SPHERE);
        CHECK(child);
        CHECK(child->type == MLI_SPHERE);
        child->id = 33;
        child->primitive.sphere->radius = 1.;
        mliFrame_free(&mother);
    }

    /* mli_frame_to_scenery */
    /* estimate capacity */
    {
        mliFrame root = mliFrame_init();
        mliFrame* sphere = NULL;
        mliScenery scenery = mliScenery_init();
        CHECK(mliFrame_malloc(&root, MLI_FRAME));
        sphere = mliFrame_add(&root, MLI_SPHERE);
        CHECK(sphere);
        CHECK(__mliScenery_set_primitive_capacity(&scenery, &root));

        CHECK(scenery.num_spheres == 1u);

        mliScenery_free(&scenery);
        mliFrame_free(&root);
    }

    /* mliFrame type: uint64 <-> string */
    {
        uint64_t i;
        uint64_t types[8] = {
            MLI_FRAME,
            MLI_MESH,
            MLI_SPHERICAL_CAP_HEX,
            MLI_SPHERE,
            MLI_CYLINDER,
            MLI_HEXAGON,
            MLI_BICIRCLEPLANE,
            MLI_DISC};
        uint64_t type;
        char type_string[1024];
        CHECK(!mli_string_to_type("Wtf?", &type));
        for (i = 0; i < 8; i++) {
            mli_type_to_string(types[i], type_string);
            CHECK(mli_string_to_type(type_string, &type));
            CHECK(type == types[i]);
        }
    }

    /* mliFrame */
    /* mliFrame_set_frame2root */
    /* test only translation z-component */
    {
        mliFrame* c1;
        mliFrame* c2;
        mliFrame* c1_c1;
        mliFrame root = mliFrame_init();
        CHECK(mliFrame_malloc(&root, MLI_FRAME));
        root.id = 1337;
        c1 = mliFrame_add(&root, MLI_FRAME);
        CHECK(c1);
        c1->id = 1;
        c1->frame2mother.trans = mliVec_set(0., 0., 1.);
        c2 = mliFrame_add(&root, MLI_FRAME);
        CHECK(c2);
        c2->id = 2;
        c2->frame2mother.trans = mliVec_set(0., 0., -1.);
        c1_c1 = mliFrame_add(c1, MLI_FRAME);
        CHECK(c1_c1);
        c1_c1->id = 11;
        c1_c1->frame2mother.trans = mliVec_set(0., 0., 1.);

        mliFrame_set_frame2root(&root);
        CHECK_MARGIN(root.frame2root.trans.z, 0., 1e-9);
        CHECK_MARGIN(c1->frame2root.trans.z, 1., 1e-9);
        CHECK_MARGIN(c2->frame2root.trans.z, -1., 1e-9);
        CHECK_MARGIN(c1_c1->frame2root.trans.z, 2., 1e-9);
    }

    /* mliUserScenery */
    {
        mliUserScenery uscn = mliUserScenery_init();
        CHECK(mliUserScenery_malloc(&uscn));
        mliUserScenery_free(&uscn);
    }

    /* mli_frame_to_scenery */
    {
        mliUserScenery uscn = mliUserScenery_init();
        mliScenery scenery = mliScenery_init();
        CHECK(mliUserScenery_malloc(&uscn));

        CHECK(mliScenery_malloc_from_mliUserScenery(&scenery, &uscn));

        mliScenery_free(&scenery);
        mliUserScenery_free(&uscn);
    }

    /* upper bound */
    {
        double points[3] = {0., 1., 2.};
        uint64_t num_points = 3;
        CHECK(mli_upper_compare_double(points, num_points, -1.) == 0);
        CHECK(mli_upper_compare_double(points, num_points, 3.) == 3);
    }

    {
        double *points = NULL;
        uint64_t num_points = 0;
        CHECK(mli_upper_compare_double(points, num_points, 4.5) == 0);
    }

    {
        double points[8] = {1., 2., 3., 4., 5., 6., 7., 8.};
        uint64_t num_points = 8;
        CHECK(mli_upper_compare_double(points, num_points, 4.5) == 4);
    }

    {
        double points[8] = {1., 2., 3.};
        uint64_t num_points = 3;
        CHECK(mli_upper_compare_double(points, num_points, 9.) == 3);
    }

    {
        double points[8] = {1., 2., 3., 4., 5., 6., 7., 8.};
        uint64_t num_points = 8;
        CHECK(mli_upper_compare_double(points, num_points, 0.) == 0);
    }

    {
        double points[8] = {1., 2., 3., 4., 5., 6., 7., 8.};
        uint64_t num_points = 8;
        CHECK(mli_upper_compare_double(points, num_points, 2.) == 2);
    }

    {
        double points[3] = {1., 2., 3.};
        uint64_t num_points = 3;
        CHECK(mli_upper_compare_double(points, num_points, 2.5) == 2);
    }

    /* histogram 1D */
    {
        const double bin_edges[3] = {1., 2., 3.};
        const uint64_t num_bin_edges = 3;
        uint64_t bins[2] = {0u, 0u};
        uint64_t overflow_bin, underflow_bin;
        overflow_bin = 0u;
        underflow_bin = 0u;
        mli_histogram(
            bin_edges, num_bin_edges, &underflow_bin, bins, &overflow_bin, 2.5);
        CHECK(underflow_bin == 0u);
        CHECK(bins[0] == 0u);
        CHECK(bins[1] == 1u);
        CHECK(overflow_bin == 0u);

        mli_histogram(
            bin_edges, num_bin_edges, &underflow_bin, bins, &overflow_bin, 0.5);
        CHECK(underflow_bin == 1u);
        CHECK(bins[0] == 0u);
        CHECK(bins[1] == 1u);
        CHECK(overflow_bin == 0u);

        mli_histogram(
            bin_edges, num_bin_edges, &underflow_bin, bins, &overflow_bin, 3.5);
        CHECK(underflow_bin == 1u);
        CHECK(bins[0] == 0u);
        CHECK(bins[1] == 1u);
        CHECK(overflow_bin == 1u);

        mli_histogram(
            bin_edges, num_bin_edges, &underflow_bin, bins, &overflow_bin, 1.0);
        CHECK(underflow_bin == 1u);
        CHECK(bins[0] == 1u);
        CHECK(bins[1] == 1u);
        CHECK(overflow_bin == 1u);

        mli_histogram(
            bin_edges, num_bin_edges, &underflow_bin, bins, &overflow_bin, 1.9);
        CHECK(underflow_bin == 1u);
        CHECK(bins[0] == 2u);
        CHECK(bins[1] == 1u);
        CHECK(overflow_bin == 1u);

        mli_histogram(
            bin_edges, num_bin_edges, &underflow_bin, bins, &overflow_bin, 2.0);
        CHECK(underflow_bin == 1u);
        CHECK(bins[0] == 2u);
        CHECK(bins[1] == 2u);
        CHECK(overflow_bin == 1u);

        mli_histogram(
            bin_edges, num_bin_edges, &underflow_bin, bins, &overflow_bin, 3.0);
        CHECK(underflow_bin == 1u);
        CHECK(bins[0] == 2u);
        CHECK(bins[1] == 2u);
        CHECK(overflow_bin == 2u);
    }
    /* mli_bin_edges_linspace */
    {
        double bin_edges[3];
        const uint64_t num_bin_edges = 3;
        mli_linspace(0., 1., bin_edges, num_bin_edges);
        CHECK_MARGIN(bin_edges[0], 0., 1e-6);
        CHECK_MARGIN(bin_edges[1], 0.5, 1e-6);
        CHECK_MARGIN(bin_edges[2], 1., 1e-6);
    }

    /* pseudo random number generator */
    {
        double sum = 0;
        uint64_t i;
        mliMT19937 prng;
        mliMT19937_init(&prng, 0u);
        for (i = 0; i < 1000000; i++) {
            sum += mliMT19937_uniform(&prng);
        }
        CHECK_MARGIN(sum, 5e5, 2e2);
    }

    {
        uint64_t i;
        mliMT19937 prng;
        double bin_edges[100];
        const uint64_t num_bin_edges = 100;
        const uint64_t num_bins = num_bin_edges - 1u;
        uint64_t bins[99];
        uint64_t overflow_bin, underflow_bin;
        overflow_bin = 0u;
        underflow_bin = 0u;
        mli_zeros_uint64_t(bins, num_bins);
        mli_linspace(0., 1., bin_edges, num_bin_edges);
        mliMT19937_init(&prng, 0u);
        for (i = 0; i < 100000u; i++) {
            mli_histogram(
                bin_edges,
                num_bin_edges,
                &underflow_bin,
                bins,
                &overflow_bin,
                mliMT19937_uniform(&prng));
        }
        CHECK(underflow_bin == 0u);
        CHECK(overflow_bin == 0u);
        for (i = 0; i < num_bins; i++) {
            CHECK_MARGIN((double)bins[i], 1000., 100.);
        }
    }

    /* throw Pi test */
    {
        uint64_t num_throws = 1000000u;
        uint64_t i;
        uint64_t num_in_circle = 0u;
        double pi_estimate;
        mliMT19937 prng;
        mliMT19937_init(&prng, 0u);
        for (i = 0; i < num_throws; i++) {
            const double x = mliMT19937_uniform(&prng);
            const double y = mliMT19937_uniform(&prng);
            const double r_sq = x*x + y*y;
            if (r_sq <= 1.) {
                num_in_circle += 1u;
            }
        }
        pi_estimate = 4*(double)num_in_circle/(double)num_throws;
        CHECK_MARGIN(pi_estimate, MLI_PI, 1e-3);
    }

    /* from_outside_to_inside */
    {
        mliVec normal = {0., 0., 1.};
        mliVec direction = {0., 0., -1.};
        CHECK(mli_ray_runs_from_outside_to_inside(direction, normal));
    }

    {
        mliVec normal = {0., 0., 1.};
        mliVec direction = {0., 0., 1.};
        CHECK(!mli_ray_runs_from_outside_to_inside(direction, normal));
    }

    /* sphere */
    {
        double c;
        for (c = -2.5; c < 2.5; c += .25) {
            mliOBB b;
            double radius = 1.;
            b = mliSphere_obb(radius, mliVec_set(c, c*2., c*3.));
            CHECK_MARGIN(b.lower.x, (-1. + c), 1e-6);
            CHECK_MARGIN(b.lower.y, (-1. + c*2), 1e-6);
            CHECK_MARGIN(b.lower.z, (-1. + c*3), 1e-6);
            CHECK_MARGIN(b.upper.x, ( 1. + c), 1e-6);
            CHECK_MARGIN(b.upper.y, ( 1. + c*2), 1e-6);
            CHECK_MARGIN(b.upper.z, ( 1. + c*3), 1e-6);
        }
    }

    {
        double radius = 1.;
        mliOBB obb;
        obb.lower = mliVec_set(-1., -1., -1.);
        obb.upper = mliVec_set(1., 1., 1.);
        CHECK(mliSphere_has_overlap_obb(radius, mliVec_set(0., 0., 0.), obb));
        CHECK(mliSphere_has_overlap_obb(radius, mliVec_set(0., 0., .5), obb));
        CHECK(mliSphere_has_overlap_obb(radius, mliVec_set(.5, .5, .5), obb));
        CHECK(!mliSphere_has_overlap_obb(radius, mliVec_set(.0, .0, 2.5), obb));
        CHECK(!mliSphere_has_overlap_obb(radius, mliVec_set(.0, 2.5, 0.), obb));
        CHECK(!mliSphere_has_overlap_obb(radius, mliVec_set(2.5, 0., 0.), obb));
        CHECK(!mliSphere_has_overlap_obb(radius, mliVec_set(-2.5, 0., 0.), obb));
        CHECK(mliSphere_has_overlap_obb(radius, mliVec_set(1.99, 0., 0.), obb));
        CHECK(!mliSphere_has_overlap_obb(radius, mliVec_set(2.01, 0., 0.), obb));
    }

    {
        double radius = 1.;
        double plus_solution, minus_solution;
        mliRay ray;
        ray.support = mliVec_set(0., 0., -10);
        ray.direction = mliVec_set(0., 0., 1.);
        CHECK(
            mli_sphere_intersection_equation(
                radius,
                ray,
                &plus_solution,
                &minus_solution));
        CHECK_MARGIN(plus_solution, 11., 1e-6);
        CHECK_MARGIN(minus_solution, 9., 1e-6);
    }

    {
        double radius = 1.;
        double plus_solution, minus_solution;
        mliRay ray;
        ray.support = mliVec_set(0., 0., 10);
        ray.direction = mliVec_set(0., 0., 1.);
        CHECK(
            mli_sphere_intersection_equation(
                radius,
                ray,
                &plus_solution,
                &minus_solution));
    }

    {
        double radius = 1.;
        double plus_solution, minus_solution;
        mliRay ray;
        ray.support = mliVec_set(0., 2., -10);
        ray.direction = mliVec_set(0., 0., 1.);
        CHECK(
            !mli_sphere_intersection_equation(
                radius,
                ray,
                &plus_solution,
                &minus_solution));
    }
    /* Orientated-Bounding-Box */
    {
        mliCube node;
        mliCube child;
        node.lower = mliVec_set(.0, .0, .0);
        node.edge_length = 2.;
        child = mliCube_octree_child(node, 0, 0, 0);
        CHECK_MARGIN(child.lower.x, 0., 1e-7);
        CHECK_MARGIN(child.lower.y, 0., 1e-7);
        CHECK_MARGIN(child.lower.z, 0., 1e-7);
        CHECK_MARGIN(child.edge_length, 1., 1e-7);

        child = mliCube_octree_child(node, 0, 0, 1);
        CHECK_MARGIN(child.lower.x, 0., 1e-7);
        CHECK_MARGIN(child.lower.y, 0., 1e-7);
        CHECK_MARGIN(child.lower.z, 1., 1e-7);
        CHECK_MARGIN(child.edge_length, 1., 1e-7);

        child = mliCube_octree_child(node, 0, 1, 0);
        CHECK_MARGIN(child.lower.x, 0., 1e-7);
        CHECK_MARGIN(child.lower.y, 1., 1e-7);
        CHECK_MARGIN(child.lower.z, 0., 1e-7);
        CHECK_MARGIN(child.edge_length, 1., 1e-7);

        child = mliCube_octree_child(node, 1, 0, 0);
        CHECK_MARGIN(child.lower.x, 1., 1e-7);
        CHECK_MARGIN(child.lower.y, 0., 1e-7);
        CHECK_MARGIN(child.lower.z, 0., 1e-7);
        CHECK_MARGIN(child.edge_length, 1., 1e-7);
    }

    {
        mliOBB a;
        mliCube cube;
        cube.lower = mliVec_set(.0, .0, .0);
        cube.edge_length = 1.;
        a = mliCube_to_obb(cube);
        CHECK(
            mliVec_equal_margin(
                a.lower,
                cube.lower,
                1e-6));
        CHECK(
            mliVec_equal_margin(
                a.upper,
                mliVec_set(1., 1., 1.),
                1e-6));
    }

    {
        mliOBB a;
        mliCube cube;
        a.lower = mliVec_set(.0, .0, .0);
        a.upper = mliVec_set(1., 2., 3.);
        cube = mliCube_outermost_cube(a);

        CHECK_MARGIN(cube.lower.x, 0.5 - 1.5, 1e-7);
        CHECK_MARGIN(cube.lower.y, 1. - 1.5, 1e-7);
        CHECK_MARGIN(cube.lower.z, 1.5 - 1.5, 1e-7);
        CHECK_MARGIN(cube.edge_length, 3., 1e-7);
    }

    {
        mliVec center;
        mliOBB obb;
        obb.lower = mliVec_set(.5, .5, .5);
        obb.upper = mliVec_set(1.5, 1.5, 1.5);
        center = mliOBB_center(obb);
        CHECK_MARGIN(center.x, 1., 1e-7);
        CHECK_MARGIN(center.y, 1., 1e-7);
        CHECK_MARGIN(center.z, 1., 1e-7);
    }

    {
        mliOBB obb = mliTriangle_obb(
            mliVec_set(-5., 2., -.8),
            mliVec_set(20., -3., 19.),
            mliVec_set(10., 6., 2.5));
        CHECK_MARGIN(obb.lower.x, -5., 1e-7);
        CHECK_MARGIN(obb.lower.y, -3., 1e-7);
        CHECK_MARGIN(obb.lower.z, -.8, 1e-7);
        CHECK_MARGIN(obb.upper.x, 20., 1e-7);
        CHECK_MARGIN(obb.upper.y, 6., 1e-7);
        CHECK_MARGIN(obb.upper.z, 19., 1e-7);
    }

    /* OBB mliVec */
    {
        mliVec point_inside = {1., 1., 1.};
        mliVec point_outside = {-1., -2., -3.};
        mliVec low = {0. ,0., 0.};
        mliVec upp = {2., 2., 2.};
        CHECK(mliVec_overlap_obb(point_inside, low, upp));
        CHECK(!mliVec_overlap_obb(point_outside, low, upp));
    }

    {
        mliVec low = {0. ,0., 0.};
        mliVec upp = {2., 2., 2.};
        CHECK(mliVec_overlap_obb(mliVec_set(0., 0., 0.), low, upp));
        CHECK(mliVec_overlap_obb(mliVec_set(0., 0., 1.), low, upp));
        CHECK(mliVec_overlap_obb(mliVec_set(0., 1., 1.), low, upp));
        CHECK(mliVec_overlap_obb(mliVec_set(2., 2., 2.), low, upp));
        CHECK(mliVec_overlap_obb(mliVec_set(1., 2., 2.), low, upp));
        CHECK(mliVec_overlap_obb(mliVec_set(1., 1., 2.), low, upp));
    }
    /* OBB mliTriangle */
    {
        mliOBB obb;
        obb.lower = mliVec_set(0. ,0., 0.);
        obb.upper = mliVec_set(2., 2., 2.);
        CHECK(mliTriangle_has_overlap_obb(
            mliVec_set(0., 0., 0.),
            mliVec_set(2., 2., 2.),
            mliVec_set(1., 1., 1.),
            obb));

        CHECK(!mliTriangle_has_overlap_obb(
            mliVec_set(0., 0., 5.),
            mliVec_set(0., 1., 5.),
            mliVec_set(1., 1., 5.),
            obb));

        CHECK(mliTriangle_has_overlap_obb(
            mliVec_set(0., 0., 2.),
            mliVec_set(0., 1., 2.),
            mliVec_set(1., 1., 2.),
            obb));

        CHECK(mliTriangle_has_overlap_obb(
            mliVec_set(2., 2., 1.),
            mliVec_set(2., 3., 1.),
            mliVec_set(3., 3., 1.),
            obb));

        CHECK(!mliTriangle_has_overlap_obb(
            mliVec_set(2.1, 2., 1.),
            mliVec_set(2.1, 3., 1.),
            mliVec_set(3., 3., 1.),
            obb));

        CHECK(mliTriangle_has_overlap_obb(
            mliVec_set(-50, -50, 1.),
            mliVec_set(50, -50, 1.),
            mliVec_set(0., 50, 1.),
            obb));

    }

    /* assert */
    {
        assert(1);
        /* printf("%u\n", UINT32_MAX); */
    }
    /* mliScenery */
    {
        uint64_t i;
        mliScenery scenery = mliScenery_init();
        mliScenery scenery_back = mliScenery_init();
        mliMesh diff_cube_sphere = mliMesh_init();
        mliColor red = {255., 0., 0.};
        mliColor blue = {0., 0., 255.};
        mliColor green = {0., 255., 0.};
        mliColor violet = {255., 0., 255.};
        scenery.num_functions = 1u;
        scenery.num_colors = 4u;
        scenery.num_surfaces = 4u;
        scenery.num_spherical_cap_hex = 1u;
        scenery.num_spheres = 1u;
        scenery.num_cylinders = 1u;
        scenery.num_hexagons = 1u;
        scenery.num_bicircleplanes = 1u;
        scenery.num_discs = 1u;

        mliMesh_malloc_from_object_file(
            "diff_cube_sphere.off",
            &diff_cube_sphere);

        scenery.num_vertices = diff_cube_sphere.num_vertices;
        scenery.num_triangles = diff_cube_sphere.num_faces;

        CHECK(mliScenery_malloc(&scenery));

        scenery.functions[0].num_points = 2u;
        CHECK(mliFunc_malloc(&scenery.functions[0]));
        scenery.functions[0].x[0] = 200.e-9;
        scenery.functions[0].x[1] = 1200.e-9;
        scenery.functions[0].y[0] = 0.;
        scenery.functions[0].y[1] = 0.;

        scenery.colors[0] = red;
        scenery.colors[1] = blue;
        scenery.colors[2] = green;
        scenery.colors[3] = violet;

        scenery.surfaces[0].color = 0u;
        scenery.surfaces[0].reflection = 0u;
        scenery.surfaces[0].refraction = 0u;
        scenery.surfaces[0].absorbtion = 0u;

        scenery.surfaces[1].color = 1u;
        scenery.surfaces[1].reflection = 0u;
        scenery.surfaces[1].refraction = 0u;
        scenery.surfaces[1].absorbtion = 0u;

        scenery.surfaces[2].color = 2u;
        scenery.surfaces[2].reflection = 0u;
        scenery.surfaces[2].refraction = 0u;
        scenery.surfaces[2].absorbtion = 0u;

        scenery.surfaces[3].color = 3u;
        scenery.surfaces[3].reflection = 0u;
        scenery.surfaces[3].refraction = 0u;
        scenery.surfaces[3].absorbtion = 0u;

        mliVec_ncpy(
            diff_cube_sphere.vertices,
            scenery.vertices,
            diff_cube_sphere.num_vertices);

        mliFace_ncpy(
            diff_cube_sphere.faces,
            scenery.triangles,
            diff_cube_sphere.num_faces);

        for (i = 0; i < scenery.num_triangles; i++) {
            scenery.triangles_surfaces[i].outer = 0u;
            scenery.triangles_surfaces[i].inner = 0u;}

        /* spherical_cap_hex */
        scenery.spherical_cap_hex[0].curvature_radius = 10*2.;
        scenery.spherical_cap_hex[0].inner_hex_radius = 3.3;
        scenery.spherical_cap_hex_T[0].trans = mliVec_set(0., -2, 0);
        scenery.spherical_cap_hex_T[0].rot =
            mliQuaternion_set_rotaxis_and_angle(
                mliVec_set(0., 0., 0.),
                0.);
        scenery.spherical_cap_hex_surfaces[0].outer = 2u;
        scenery.spherical_cap_hex_surfaces[0].inner = 2u;

        /* spheres */
        scenery.spheres[0].radius = 2.5;
        scenery.spheres_surfaces[0].outer = 1u;
        scenery.spheres_surfaces[0].inner = 1u;
        scenery.spheres_T[0].trans = mliVec_set(0., 0., 0.);
        scenery.spheres_T[0].rot =
            mliQuaternion_set_rotaxis_and_angle(
                mliVec_set(0., 0., 0.),
                0.);

        /* cylinders */
        scenery.cylinders[0].radius = 1.;
        scenery.cylinders[0].length = 7.;
        scenery.cylinders_surfaces[0].outer = 3u;
        scenery.cylinders_surfaces[0].inner = 3u;
        scenery.cylinders_T[0].trans = mliVec_set(0., 3., 0.);
        scenery.cylinders_T[0].rot =
            mliQuaternion_set_rotaxis_and_angle(
                mliVec_set(0., 1., 0.),
                mli_deg2rad(30));

        /* hexagons */
        scenery.hexagons[0].inner_radius = 1.;
        scenery.hexagons_surfaces[0].outer = 0u;
        scenery.hexagons_surfaces[0].inner = 0u;
        scenery.hexagons_T[0].trans = mliVec_set(2., 3., 0.);
        scenery.hexagons_T[0].rot =
            mliQuaternion_set_rotaxis_and_angle(
                mliVec_set(0., 1., 0.),
                mli_deg2rad(-30));

        /* bicircleplanes */
        scenery.bicircleplanes[0].y_width = 2.;
        scenery.bicircleplanes[0].x_height = 1.;
        scenery.bicircleplanes_surfaces[0].outer = 0u;
        scenery.bicircleplanes_surfaces[0].inner = 0u;
        scenery.bicircleplanes_T[0].trans = mliVec_set(0., 2., 5.);
        scenery.bicircleplanes_T[0].rot =
            mliQuaternion_set_rotaxis_and_angle(
                mliVec_set(0., 1., 0.),
                mli_deg2rad(0.));

        /* discs */
        scenery.discs[0].radius = 2.;
        scenery.discs_surfaces[0].outer = 1u;
        scenery.discs_surfaces[0].inner = 1u;
        scenery.discs_T[0].trans = mliVec_set(0., -2., -5.);
        scenery.discs_T[0].rot =
            mliQuaternion_set_rotaxis_and_angle(
                mliVec_set(0., 1., 0.),
                mli_deg2rad(0.));

        mliScenery_write_to_path(&scenery, "my_scenery.mli.tmp");
        mliScenery_read_from_path(&scenery_back, "my_scenery.mli.tmp");

        CHECK(mliScenery_is_equal(&scenery, &scenery_back));

        mliScenery_free(&scenery);
        mliScenery_free(&scenery_back);
    }

    /* render image */
    {
        mliScenery scenery = mliScenery_init();
        mliOcTree octree;
        mliCamera camera;
        mliImage img = mliImage_init();
        mliScenery_read_from_path(&scenery, "my_scenery.mli.tmp");
        octree = mliOcTree_from_scenery(&scenery);
        /* mliNode_print(&octree.root, 0); */

        camera.position.x = 0.;
        camera.position.y = 0.;
        camera.position.z = -20.;
        camera.rotation.x = 0.;
        camera.rotation.y = 0.;
        camera.rotation.z = 0.;
        camera.field_of_view = mli_deg2rad(80.);

        CHECK(mliImage_malloc(&img, 640u, 480u));
        mliCamera_render_image(&camera, &scenery, &octree, &img);
        mliImage_write_to_ppm(&img, "my_image.ppm.tmp");

        mliImage_free(&img);
        mliOcTree_free(&octree);
        mliScenery_free(&scenery);
    }

    /* scenery indexes are valid */
    {
        mliScenery scenery = mliScenery_init();
        mliScenery_read_from_path(&scenery, "my_scenery.mli.tmp");
        CHECK(mliScenery_valid(&scenery));     /* <--- to be tested */
        mliScenery_free(&scenery);
    }

    /* scenery indexes are valid */
    {
        mliOBB obb;
        uint64_t i;
        uint64_t num_surface_entities;
        mliScenery scenery = mliScenery_init();
        mliScenery_read_from_path(&scenery, "my_scenery.mli.tmp");
        num_surface_entities = scenery.num_triangles;
        num_surface_entities += scenery.num_spherical_cap_hex;
        num_surface_entities += scenery.num_spheres;
        num_surface_entities += scenery.num_cylinders;
        num_surface_entities += scenery.num_hexagons;
        num_surface_entities += scenery.num_bicircleplanes;
        num_surface_entities += scenery.num_discs;

        CHECK(mliScenery_num_objects(&scenery) == num_surface_entities);

        obb.lower = mliVec_set(-1e2, -1e2, -1e2);
        obb.upper = mliVec_set(1e2, 1e2, 1e2);
        for (i = 0u; i <= mliScenery_num_objects(&scenery); i++) {
            CHECK(mliScenery_overlap_obb(
                &scenery,
                i,
                obb));
        }

        mliScenery_free(&scenery);
    }

    {
        mliScenery scenery = mliScenery_init();
        mliOBB obb;
        mliScenery_read_from_path(&scenery, "my_scenery.mli.tmp");
        CHECK(mliScenery_valid(&scenery));

        obb = mliScenery_outermost_obb(&scenery);
        CHECK_MARGIN(obb.lower.x, -7.5, 1e-7);
        CHECK_MARGIN(obb.lower.y, -7.5, 1e-7);
        CHECK_MARGIN(obb.lower.z, -7.5, 1e-7);
        CHECK_MARGIN(obb.upper.x, +7.5, 1e-7);
        CHECK_MARGIN(obb.upper.y, +7.5, 1e-7);
        CHECK_MARGIN(obb.upper.z, +7.5, 1e-7);

        mliScenery_free(&scenery);
    }

    /* read non existing file */
    {
        mliScenery scenery = mliScenery_init();
        CHECK(!mliScenery_read_from_path(&scenery, "does_not_exist.tmp"));
    }


    /* scenery intersection interface -> sphere */
    {
        mliScenery scenery = mliScenery_init();
        mliIntersection isec;
        int hit = 0;
        uint64_t sphere_idx;
        double sphere_radius = 2.5;
        mliScenery_read_from_path(&scenery, "my_scenery.mli.tmp");

        sphere_idx = scenery.num_triangles +
            scenery.num_spherical_cap_hex;

        hit = mliScenery_intersection(
            &scenery,
            mliRay_set(
                mliVec_set(0., 0., -10.),
                mliVec_set(0. ,0., 1.)),
            sphere_idx,
            &isec);

        CHECK(hit);
        CHECK(isec.object_idx == sphere_idx);
        CHECK(
            mliVec_equal_margin(
                isec.position,
                mliVec_set(0., 0., -sphere_radius),
                1e-6));
        CHECK(
            mliVec_equal_margin(
                isec.surface_normal,
                mliVec_set(0., 0., -1),
                1e-6));
        CHECK(isec.from_outside_to_inside);
        CHECK_MARGIN(isec.distance_of_ray, 10 - sphere_radius, 1e-6);

        mliScenery_free(&scenery);
    }

    /* OctTree*/
    {
        mliScenery scenery = mliScenery_init();
        mliOcTree octree;
        mliIntersection isec;
        mliScenery_read_from_path(&scenery, "my_scenery.mli.tmp");
        octree = mliOcTree_from_scenery(&scenery);

        /*mliNode_print(&tree, 0u);*/
        /*
        fprintf(stderr ,"num_nodes: %u\n", mliNode_num_nodes(&tree));
        fprintf(
            stderr,
            "capacity_objects: %u\n",
            mliNode_capacity_objects(&tree));*/

        mli_ray_octree_traversal(
            &scenery,
            &octree,
            mliRay_set(
                mliVec_set(0.1 ,2.5, 10.),
                mliVec_set(0. ,0., -1.)),
            &isec);

        mliOcTree_free(&octree);
        mliScenery_free(&scenery);
    }

    /* mliScenery asymetric */
    {
        mliScenery scenery = mliScenery_init();
        mliVec offset;
        uint64_t i;
        scenery.num_functions = 1u;
        scenery.num_colors = 1u;
        scenery.num_surfaces = 1u;
        scenery.num_spherical_cap_hex = 0u;
        scenery.num_spheres = 50u;
        scenery.num_vertices = 0u;
        scenery.num_triangles = 0u;

        CHECK(mliScenery_malloc(&scenery));

        scenery.functions[0].num_points = 2u;
        CHECK(mliFunc_malloc(&scenery.functions[0]));
        scenery.functions[0].x[0] = 200.e-9;
        scenery.functions[0].x[1] = 1200.e-9;
        scenery.functions[0].y[0] = 0.;
        scenery.functions[0].y[1] = 0.;

        scenery.colors[0] = mliColor_set(255., 0., 0.);

        scenery.surfaces[0].color = 0u;
        scenery.surfaces[0].reflection = 0u;
        scenery.surfaces[0].refraction = 0u;
        scenery.surfaces[0].absorbtion = 0u;
        offset = mliVec_set(0, 0, -8);
        for (i = 0; i < scenery.num_spheres; i ++) {
            const double phi = 2.*MLI_PI*(double)(i)/scenery.num_spheres;
            const double ii = (double)(i)/scenery.num_spheres;
            const double zoff = 0.5 - ii;
            scenery.spheres[i].radius = 0.1;
            scenery.spheres_surfaces[i].outer = 0u;
            scenery.spheres_surfaces[i].inner = 0u;
            scenery.spheres_T[i].trans = mliVec_set(
                5*cos(phi) + offset.x,
                -5*sin(phi) + offset.y,
                5*zoff + offset.z);
            scenery.spheres_T[i].rot =
                mliQuaternion_set_rotaxis_and_angle(
                    mliVec_set(0., 0., 0.),
                    0.);
        }
        mliScenery_write_to_path(&scenery, "asymetric_scenery.mli.tmp");
        mliScenery_free(&scenery);
    }

    /* render image asymetric scenery */
    {
        mliScenery scenery = mliScenery_init();
        mliOcTree octree;
        mliCamera camera;
        mliImage img = mliImage_init();
        mliScenery_read_from_path(&scenery, "asymetric_scenery.mli.tmp");
        octree = mliOcTree_from_scenery(&scenery);
        CHECK(mliNode_num_nodes(&octree.root) == 9);
        /* mliNode_print(&octree.root, 0); */
        camera.position.x = 0.;
        camera.position.y = 0.;
        camera.position.z = -20.;
        camera.rotation.x = 0.;
        camera.rotation.y = 0.;
        camera.rotation.z = 0.;
        camera.field_of_view = mli_deg2rad(80.);

        CHECK(mliImage_malloc(&img, 640u, 480u));
        mliCamera_render_image(&camera, &scenery, &octree, &img);
        mliImage_write_to_ppm(&img, "asymetric_image.ppm.tmp");

        mliImage_free(&img);
        mliOcTree_free(&octree);
        mliScenery_free(&scenery);
    }

    /* mliVector */
    {
        uint64_t i;
        mliVector vec = mliVector_init();
        CHECK(mliVector_malloc(&vec, 0u, sizeof(mliColor)));
        CHECK(vec.size == 0u);

        for (i = 0; i < 100; i++) {
            mliColor color = mliColor_set(i*1., i*2., i*3.);
            CHECK(vec.size == i);
            CHECK(mliVector_push_back(&vec, &color));
            CHECK(vec.size == i + 1);}

        for (i = 0; i < vec.size; i++) {
            mliColor color = *(mliColor*)mliVector_at(&vec, i);
            CHECK_MARGIN(color.r, i*1., 1e-9);
            CHECK_MARGIN(color.g, i*2., 1e-9);
            CHECK_MARGIN(color.b, i*3., 1e-9);
            CHECK(vec.size == 100);}

        mliVector_free(&vec);
        CHECK(vec.size == 0);
        CHECK(vec.capacity == 0);
    }

    /* mliVector with pointers */
    {
        uint64_t i;
        mliVector vec = mliVector_init();
        CHECK(mliVector_malloc(&vec, 0u, sizeof(mliColor*)));
        CHECK(vec.size == 0u);

        for (i = 0; i < 10; i++) {
            mliColor* ptr_color_in = (mliColor*)(i*i);
            CHECK(vec.size == i);
            CHECK(mliVector_push_back(&vec, &ptr_color_in));
            CHECK(vec.size == i + 1);}

        for (i = 0; i < vec.size; i++) {
            mliColor* ptr_color_out = *((mliColor**)mliVector_at(&vec, i));
            CHECK(ptr_color_out == (mliColor*)(i*i));
            CHECK(vec.size == 10);}

        mliVector_free(&vec);
        CHECK(vec.size == 0);
        CHECK(vec.capacity == 0);
    }

    /* mliQuaternion */
    {
        mliQuaternion p = mliQuaternion_set(1., 2., 3., 4.);
        CHECK(p.w = 1.);
        CHECK(p.x = 2.);
        CHECK(p.y = 3.);
        CHECK(p.z = 4.);
        CHECK_MARGIN(mliQuaternion_norm(p), sqrt(1.+4.+9.+16.), 1e-6);
    }

    /* mliQuaternion */
    /* rotation is pure i.e. norm is 1. */
    {
        mliQuaternion q = mliQuaternion_set_rotaxis_and_angle(
            mliVec_set(1., 0., 0.),
            mli_deg2rad(23.));
        CHECK_MARGIN(mliQuaternion_norm(q), 1., 1e-6);
    }

    /* mliQuaternion */
    /* unity quaternion and matrix */
    {
        mliQuaternion q_eye = mliQuaternion_set_rotaxis_and_angle(
            mliVec_set(1., 0., 0.),
            mli_deg2rad(0.));
        mliRotMat r_eye = mliQuaternion_to_matrix(q_eye);
        CHECK_MARGIN(r_eye.r00, 1., 1e-6);
        CHECK_MARGIN(r_eye.r01, 0., 1e-6);
        CHECK_MARGIN(r_eye.r02, 0., 1e-6);

        CHECK_MARGIN(r_eye.r10, 0., 1e-6);
        CHECK_MARGIN(r_eye.r11, 1., 1e-6);
        CHECK_MARGIN(r_eye.r12, 0., 1e-6);

        CHECK_MARGIN(r_eye.r20, 0., 1e-6);
        CHECK_MARGIN(r_eye.r21, 0., 1e-6);
        CHECK_MARGIN(r_eye.r22, 1., 1e-6);
    }

    /* mliQuaternion */
    /* rotation matrix z-axis +90deg */
    {
        mliQuaternion z90 = mliQuaternion_set_rotaxis_and_angle(
            mliVec_set(0., 0., 1.),
            mli_deg2rad(90.));
        mliRotMat r_z90 = mliQuaternion_to_matrix(z90);

        CHECK_MARGIN(r_z90.r00, 0., 1e-6);
        CHECK_MARGIN(r_z90.r01, -1., 1e-6);
        CHECK_MARGIN(r_z90.r02, 0., 1e-6);

        CHECK_MARGIN(r_z90.r10, 1., 1e-6);
        CHECK_MARGIN(r_z90.r11, 0., 1e-6);
        CHECK_MARGIN(r_z90.r12, 0., 1e-6);

        CHECK_MARGIN(r_z90.r20, 0., 1e-6);
        CHECK_MARGIN(r_z90.r21, 0., 1e-6);
        CHECK_MARGIN(r_z90.r22, 1., 1e-6);
    }

    /* mliQuaternion */
    /* sequence of rotations */
    {
        /*
        __initial__

                    z
                    |
                    |
                    |_____ y
                   /
                  /
                 x

        __axis x, +90deg__

                    y
                    |
                    |
            z ______|
                   /
                  /
                 x

        __axis z, +90deg

                    x
                    |  y
                    | /
            z ______|/

        */
        mliQuaternion x90 = mliQuaternion_set_rotaxis_and_angle(
            mliVec_set(1., 0., 0.),
            mli_deg2rad(90.));
        mliQuaternion z90 = mliQuaternion_set_rotaxis_and_angle(
            mliVec_set(0., 0., 1.),
            mli_deg2rad(90.));
        mliQuaternion z90_x90;
        mliRotMat r_z90_x90;
        CHECK_MARGIN(mliQuaternion_norm(x90), 1., 1e-6);
        CHECK_MARGIN(mliQuaternion_norm(z90), 1., 1e-6);
        z90_x90 = mliQuaternion_product(x90, z90);
        CHECK_MARGIN(mliQuaternion_norm(z90_x90), 1., 1e-6);
        r_z90_x90 = mliQuaternion_to_matrix(z90_x90);

        CHECK_MARGIN(r_z90_x90.r00, 0., 1e-6);
        CHECK_MARGIN(r_z90_x90.r10, 0., 1e-6);
        CHECK_MARGIN(r_z90_x90.r20, 1., 1e-6);

        CHECK_MARGIN(r_z90_x90.r01, -1., 1e-6);
        CHECK_MARGIN(r_z90_x90.r11, 0., 1e-6);
        CHECK_MARGIN(r_z90_x90.r21, 0., 1e-6);

        CHECK_MARGIN(r_z90_x90.r02, 0., 1e-6);
        CHECK_MARGIN(r_z90_x90.r12, -1., 1e-6);
        CHECK_MARGIN(r_z90_x90.r22, 0., 1e-6);
    }

    /* mliQuaternion */
    /* Tait-Bryan-angles */
    {
        mliQuaternion q = mliQuaternion_set_tait_bryan(0., 0.,0.);
        CHECK_MARGIN(mliQuaternion_norm(q), 1., 1e-6);
    }

    {
        mliQuaternion q = mliQuaternion_set_tait_bryan(0., 0., mli_deg2rad(90));
        mliRotMat rot = mliRotMat_init_tait_bryan(0., 0., mli_deg2rad(90));
        mliRotMat rot_from_q;
        CHECK_MARGIN(mliQuaternion_norm(q), 1., 1e-6);
        rot_from_q = mliQuaternion_to_matrix(q);
        CHECK(mliRotMat_equal_margin(rot, rot_from_q, 1e-9));
    }

    /* Tait-Bryan-angles */
    /* multiple combinations */
    {
        double rx, ry, rz;
        for (rx = -400; rx < 400; rx += 25) {
            for (ry = -400; ry < 400; ry += 25) {
                for (rz = -400; rz < 400; rz += 25) {
                    double xrad = mli_deg2rad(rx);
                    double yrad = mli_deg2rad(ry);
                    double zrad = mli_deg2rad(rz);
                    mliQuaternion q = mliQuaternion_set_tait_bryan(
                        xrad, yrad, zrad);
                    mliRotMat rot = mliRotMat_init_tait_bryan(
                        xrad, yrad, zrad);
                    mliRotMat rot_from_q = mliQuaternion_to_matrix(q);
                    CHECK(mliRotMat_equal_margin(rot, rot_from_q, 1e-9));
                }
            }
        }
    }

    /* mliHomTraComp */
    /* sequence, unity and unity shall be unity*/
    {
        mliHomTraComp a;
        mliHomTraComp b;
        mliHomTraComp a_b;

        a.trans = mliVec_set(0., 0., 0.);
        a.rot = mliQuaternion_set_tait_bryan(0., 0., 0.);

        b.trans = mliVec_set(0., 0., 0.);
        b.rot = mliQuaternion_set_tait_bryan(0., 0., 0.);

        a_b = mliHomTraComp_sequence(a, b);
        CHECK_MARGIN(a_b.trans.x, 0., 1e-9);
        CHECK_MARGIN(a_b.trans.y, 0., 1e-9);
        CHECK_MARGIN(a_b.trans.z, 0., 1e-9);

        CHECK_MARGIN(a_b.rot.w, 1., 1e-9);
    }

    {
        mliHomTraComp a; /* only translation */
        mliHomTraComp b; /* only rotation */
        mliHomTraComp a_b;
        mliHomTra a_b_;

        a.trans = mliVec_set(0., 0., 1.);
        a.rot = mliQuaternion_set_tait_bryan(0., 0., 0.);

        b.trans = mliVec_set(0., 0., 0.);
        b.rot = mliQuaternion_set_tait_bryan(0., 0., mli_deg2rad(90.));

        a_b = mliHomTraComp_sequence(a, b);
        a_b_ = mliHomTra_from_compact(a_b);
        /* mliHomTra_print(a_b_); */
        CHECK_MARGIN(a_b_.trans.x, 0., 1e-9);
        CHECK_MARGIN(a_b_.trans.y, 0., 1e-9);
        CHECK_MARGIN(a_b_.trans.z, 1., 1e-9);

        CHECK_MARGIN(a_b_.rot.r00,  0., 1e-9);
        CHECK_MARGIN(a_b_.rot.r10, -1., 1e-9);
        CHECK_MARGIN(a_b_.rot.r20,  0., 1e-9);

        CHECK_MARGIN(a_b_.rot.r01,  1., 1e-9);
        CHECK_MARGIN(a_b_.rot.r11,  0., 1e-9);
        CHECK_MARGIN(a_b_.rot.r21,  0., 1e-9);

        CHECK_MARGIN(a_b_.rot.r02,  0., 1e-9);
        CHECK_MARGIN(a_b_.rot.r12,  0., 1e-9);
        CHECK_MARGIN(a_b_.rot.r22,  1., 1e-9);
    }

    /* cancelation */
    {
        mliHomTraComp a;
        mliHomTraComp a_inverse; /* inverse ov a */
        mliHomTraComp a_a_inverse;
        mliHomTra a_a_inverse_;

        a.trans = mliVec_set(0., 0., 1.);
        a.rot = mliQuaternion_set_tait_bryan(0., 0., -mli_deg2rad(90.));

        a_inverse.trans = mliVec_set(0., 0., -1.);
        a_inverse.rot = mliQuaternion_set_tait_bryan(0., 0., mli_deg2rad(90.));

        a_a_inverse = mliHomTraComp_sequence(a, a_inverse);
        a_a_inverse_ = mliHomTra_from_compact(a_a_inverse);
        /* mliHomTra_print(a_a_inverse_); */
        CHECK_MARGIN(a_a_inverse_.trans.x, 0., 1e-9);
        CHECK_MARGIN(a_a_inverse_.trans.y, 0., 1e-9);
        CHECK_MARGIN(a_a_inverse_.trans.z, 0., 1e-9);

        CHECK_MARGIN(a_a_inverse_.rot.r00,  1., 1e-9);
        CHECK_MARGIN(a_a_inverse_.rot.r10,  0., 1e-9);
        CHECK_MARGIN(a_a_inverse_.rot.r20,  0., 1e-9);

        CHECK_MARGIN(a_a_inverse_.rot.r01,  0., 1e-9);
        CHECK_MARGIN(a_a_inverse_.rot.r11,  1., 1e-9);
        CHECK_MARGIN(a_a_inverse_.rot.r21,  0., 1e-9);

        CHECK_MARGIN(a_a_inverse_.rot.r02,  0., 1e-9);
        CHECK_MARGIN(a_a_inverse_.rot.r12,  0., 1e-9);
        CHECK_MARGIN(a_a_inverse_.rot.r22,  1., 1e-9);
    }

    /* simple sequence */
    {
        mliHomTraComp a;
        mliHomTraComp b;
        mliHomTraComp a_b;
        mliHomTra a_b_;

        a.trans = mliVec_set(0., 0., 1.);
        a.rot = mliQuaternion_set_tait_bryan(0., 0., mli_deg2rad(90.));

        b.trans = mliVec_set(1., 0., 0.);
        b.rot = mliQuaternion_set_tait_bryan(mli_deg2rad(90.), 0., 0.);

        a_b = mliHomTraComp_sequence(a, b);
        a_b_ = mliHomTra_from_compact(a_b);
        /* mliHomTra_print(a_b_); */
        CHECK_MARGIN(a_b_.trans.x, 1., 1e-9);
        CHECK_MARGIN(a_b_.trans.y, 0., 1e-9);
        CHECK_MARGIN(a_b_.trans.z, 1., 1e-9);

        CHECK_MARGIN(a_b_.rot.r00,  0., 1e-9);
        CHECK_MARGIN(a_b_.rot.r10, -1., 1e-9);
        CHECK_MARGIN(a_b_.rot.r20,  0., 1e-9);

        CHECK_MARGIN(a_b_.rot.r01,  0., 1e-9);
        CHECK_MARGIN(a_b_.rot.r11,  0., 1e-9);
        CHECK_MARGIN(a_b_.rot.r21, -1., 1e-9);

        CHECK_MARGIN(a_b_.rot.r02,  1., 1e-9);
        CHECK_MARGIN(a_b_.rot.r12,  0., 1e-9);
        CHECK_MARGIN(a_b_.rot.r22,  0., 1e-9);
    }

    /* mliHomTra */
    {
        mliRotMat rot = mliRotMat_init_tait_bryan(0., 0., 0.);
        CHECK_MARGIN(rot.r00, 1., 1e-9);
        CHECK_MARGIN(rot.r01, 0., 1e-9);
        CHECK_MARGIN(rot.r02, 0., 1e-9);

        CHECK_MARGIN(rot.r10, 0., 1e-9);
        CHECK_MARGIN(rot.r11, 1., 1e-9);
        CHECK_MARGIN(rot.r12, 0., 1e-9);

        CHECK_MARGIN(rot.r20, 0., 1e-9);
        CHECK_MARGIN(rot.r21, 0., 1e-9);
        CHECK_MARGIN(rot.r22, 1., 1e-9);

        rot = mliRotMat_init_tait_bryan(0., 0., mli_deg2rad(90));
        CHECK_MARGIN(rot.r00, 0., 1e-9);
        CHECK_MARGIN(rot.r01, 1., 1e-9);
        CHECK_MARGIN(rot.r02, 0., 1e-9);

        CHECK_MARGIN(rot.r10, -1., 1e-9);
        CHECK_MARGIN(rot.r11, 0., 1e-9);
        CHECK_MARGIN(rot.r12, 0., 1e-9);

        CHECK_MARGIN(rot.r20, 0., 1e-9);
        CHECK_MARGIN(rot.r21, 0., 1e-9);
        CHECK_MARGIN(rot.r22, 1., 1e-9);
        /*
            |y       rot z 90deg        |x
            |           --->            |
            |                           |
            --------x            y-------
        */
    }

    {
        mliRotMat rot;
        mliVec axis = {0., 0., 1.};
        rot = mliRotMat_init_axis(axis, 0.);
        CHECK_MARGIN(rot.r00, 1., 1e-9);
        CHECK_MARGIN(rot.r01, 0., 1e-9);
        CHECK_MARGIN(rot.r02, 0., 1e-9);

        CHECK_MARGIN(rot.r10, 0., 1e-9);
        CHECK_MARGIN(rot.r11, 1., 1e-9);
        CHECK_MARGIN(rot.r12, 0., 1e-9);

        CHECK_MARGIN(rot.r20, 0., 1e-9);
        CHECK_MARGIN(rot.r21, 0., 1e-9);
        CHECK_MARGIN(rot.r22, 1., 1e-9);

        rot = mliRotMat_init_axis(axis, mli_deg2rad(90.));
        CHECK_MARGIN(rot.r00, 0., 1e-9);
        CHECK_MARGIN(rot.r01, 1., 1e-9);
        CHECK_MARGIN(rot.r02, 0., 1e-9);

        CHECK_MARGIN(rot.r10, -1., 1e-9);
        CHECK_MARGIN(rot.r11, 0., 1e-9);
        CHECK_MARGIN(rot.r12, 0., 1e-9);

        CHECK_MARGIN(rot.r20, 0., 1e-9);
        CHECK_MARGIN(rot.r21, 0., 1e-9);
        CHECK_MARGIN(rot.r22, 1., 1e-9);
    }

    {
        mliRotMat rot;
        mliVec a = {0., 0., 1.};
        mliVec a_rot;
        rot = mliRotMat_init_tait_bryan(0., 0., mli_deg2rad(45.));
        a_rot = mli_transform_orientation(&rot, a);
        CHECK_MARGIN(a_rot.x, 0., 1e-9);
        CHECK_MARGIN(a_rot.y, 0., 1e-9);
        CHECK_MARGIN(a_rot.z, 1., 1e-9);
    }

    {
        mliRotMat rot;
        mliVec x = {1., 0., 0.};
        mliVec x_rot;
        rot = mliRotMat_init_tait_bryan(0., 0., mli_deg2rad(45.));
        x_rot = mli_transform_orientation(&rot, x);
        CHECK_MARGIN(x_rot.x, 1./sqrt(2.), 1e-6);
        CHECK_MARGIN(x_rot.y, -1./sqrt(2.), 1e-6);
        CHECK_MARGIN(x_rot.z, 0., 1e-9);

        x_rot = mli_transform_orientation_inverse(&rot, x);
        CHECK_MARGIN(x_rot.x, 1./sqrt(2.), 1e-6);
        CHECK_MARGIN(x_rot.y, 1./sqrt(2.), 1e-6);
        CHECK_MARGIN(x_rot.z, 0., 1e-9);
    }

    {
        mliRotMat rot;
        mliVec trans = {0., 0., 3.};
        mliVec x = {1., 0., 0.};
        mliVec x_t;
        rot = mliRotMat_init_tait_bryan(0., 0., 0.);
        x_t = mli_transform_position(&rot, trans, x);
        CHECK_MARGIN(x_t.x, 1., 1e-6);
        CHECK_MARGIN(x_t.y, 0., 1e-6);
        CHECK_MARGIN(x_t.z, 3., 1e-9);

        x_t = mli_transform_position_inverse(&rot, trans, x);
        CHECK_MARGIN(x_t.x, 1., 1e-6);
        CHECK_MARGIN(x_t.y, 0., 1e-6);
        CHECK_MARGIN(x_t.z, -3., 1e-9);
    }

    /* mliMath */
    {
        int i;
        for (i = -721; i < 721; i++) {
            double angle_in_deg = (double)i;
            CHECK_MARGIN(
                angle_in_deg,
                mli_rad2deg(
                    mli_deg2rad(angle_in_deg)
                ),
                1e-9);
        }
    }

    {
        CHECK_MARGIN(mli_deg2rad(0.), 0., 1e-9);
        CHECK_MARGIN(mli_deg2rad(90.), MLI_PI/2, 1e-9);
        CHECK_MARGIN(mli_deg2rad(180.), MLI_PI, 1e-9);
        CHECK_MARGIN(mli_deg2rad(-90.), -MLI_PI/2.0, 1e-9);
        CHECK_MARGIN(mli_deg2rad(-180.), -MLI_PI, 1e-9);
        CHECK_MARGIN(mli_deg2rad(360.), 2.*MLI_PI, 1e-9);
    }

    {
        CHECK_MARGIN(mli_rad2deg(0.*MLI_PI), 0., 1e-9);
        CHECK_MARGIN(mli_rad2deg(.25*MLI_PI), 45., 1e-9);
        CHECK_MARGIN(mli_rad2deg(.5*MLI_PI), 90., 1e-9);
        CHECK_MARGIN(mli_rad2deg(1.*MLI_PI), 180., 1e-9);
        CHECK_MARGIN(mli_rad2deg(1.e3*MLI_PI), 180.e3, 1e-9);
    }

    {
        CHECK(MLI_MAX2(3, 4) == 4);
        CHECK(MLI_MAX2(4, 3) == 4);
    }

    {
        CHECK(MLI_MIN3(4, 5, 6) == 4);
        CHECK(MLI_MIN3(5, 4, 6) == 4);
        CHECK(MLI_MIN3(5, 6, 4) == 4);

        CHECK(MLI_MAX3(4, 5, 6) == 6);
        CHECK(MLI_MAX3(5, 4, 6) == 6);
        CHECK(MLI_MAX3(5, 6, 4) == 6);

        CHECK(MLI_MAX3(4., 5., 6.) == 6.);
        CHECK(MLI_MAX3(5., 4., 6.) == 6.);
        CHECK(MLI_MAX3(5., 6., 4.) == 6.);
    }

    /* mliQuadraticEquation */
    {
        /* ax**2 + bx + c = 0 */
        const double b_over_a = 1.;
        const double c_over_a = 1.;
        double plus_solution;
        double minus_solution;
        int valid_solutions = mli_quadratiq_equation(
            b_over_a,
            c_over_a,
            &plus_solution,
            &minus_solution);

        CHECK(!valid_solutions);
    }

    {
        /* ax**2 + bx + c = 0 */
        const double b_over_a = 0.;
        const double c_over_a = 0.;
        double plus_solution;
        double minus_solution;
        CHECK(mli_quadratiq_equation(
            b_over_a,
            c_over_a,
            &plus_solution,
            &minus_solution));

        CHECK(minus_solution == 0.);
        CHECK(plus_solution == 0.);
    }

    {
        /* ax**2 + bx + c = 0 */
        const double b_over_a = 0.;
        const double c_over_a = -1.;
        double plus_solution;
        double minus_solution;
        CHECK(mli_quadratiq_equation(
            b_over_a,
            c_over_a,
            &plus_solution,
            &minus_solution));

        CHECK(minus_solution == -1.);
        CHECK(plus_solution == +1.);
    }

    {
        /* ax**2 + bx + c = 0 */
        double a, b, c;
        for (a = -5.0; a < 5.0; a = a+.25) {
            for (b = -5.0; b < 5.0; b = b+.25) {
                for (c = -5.0; c < 5.0; c = c+.25) {
                    if (a != 0.0) {
                        double xp;
                        double xm;
                        if (mli_quadratiq_equation(b/a, c/a, &xp, &xm)) {
                            CHECK_MARGIN(a*xp*xp + b*xp + c, 0., 1e-12);
                            CHECK_MARGIN(a*xm*xm + b*xm + c, 0., 1e-12);
                        }
                    }
                }
            }
        }
    }

    /* mliColor */
    {
        mliColor red = {255., 0., 0.};
        mliColor blue = {0., 0., 255.};
        mliColor mix = mliColor_mix(red, blue, 0.2);
        CHECK_MARGIN(mix.r, 255.*0.8, 1e-6);
        CHECK_MARGIN(mix.g, 0., 1e-6);
        CHECK_MARGIN(mix.b, 255.*0.2, 1e-6);
    }

    {
        mliColor colors[3];
        mliColor one = {10., 20., 30.};
        mliColor two = {1., 2., 3.};
        mliColor three = {50., 60., 70.};
        mliColor mean;
        colors[0] = one;
        colors[1] = two;
        colors[2] = three;
        mean = mliColor_mean(colors, 3);
        CHECK_MARGIN(mean.r, (10. + 1. + 50.)/3., 1e-6);
        CHECK_MARGIN(mean.g, (20. + 2. + 60.)/3., 1e-6);
        CHECK_MARGIN(mean.b, (30. + 3. + 70.)/3., 1e-5);
    }

    /* mliImage */
    {
        mliImage img = mliImage_init();
        CHECK(mliImage_malloc(&img, 3, 2));
        CHECK(img.num_cols == 3u);
        CHECK(img.num_rows == 2u);
        mliImage_free(&img);
        CHECK(img.num_cols == 0u);
        CHECK(img.num_rows == 0u);
    }

    /* mliImage */
    /* scaling */
    {
        mliImage src = mliImage_init();
        mliImage dst = mliImage_init();
        CHECK(mliImage_malloc(&src, 4, 4));
        CHECK(mliImage_malloc(&dst, 2, 2));
        /*
            src:
                     0    1    2    3  cols
                 _____________________
            0   |   10   20   30   40
            1   |   50   60   70   80
            2   |   55   34   31   17
            3   |   98   26   11   23
           rows
        */
        mliImage_set(&src, 0, 0, mliColor_set(0, 0, 10));
        mliImage_set(&src, 0, 1, mliColor_set(0, 0, 20));
        mliImage_set(&src, 0, 2, mliColor_set(0, 0, 30));
        mliImage_set(&src, 0, 3, mliColor_set(0, 0, 40));
        mliImage_set(&src, 1, 0, mliColor_set(0, 0, 50));
        mliImage_set(&src, 1, 1, mliColor_set(0, 0, 60));
        mliImage_set(&src, 1, 2, mliColor_set(0, 0, 70));
        mliImage_set(&src, 1, 3, mliColor_set(0, 0, 80));
        mliImage_set(&src, 2, 0, mliColor_set(0, 0, 55));
        mliImage_set(&src, 2, 1, mliColor_set(0, 0, 34));
        mliImage_set(&src, 2, 2, mliColor_set(0, 0, 31));
        mliImage_set(&src, 2, 3, mliColor_set(0, 0, 17));
        mliImage_set(&src, 3, 0, mliColor_set(0, 0, 98));
        mliImage_set(&src, 3, 1, mliColor_set(0, 0, 26));
        mliImage_set(&src, 3, 2, mliColor_set(0, 0, 11));
        mliImage_set(&src, 3, 3, mliColor_set(0, 0, 23));

        mliImage_scale_down_twice(&src, &dst);

        CHECK_MARGIN(mliImage_at(&dst, 0, 0).b, (10.+20.+50.+60.)/4., 1e-6);
        CHECK_MARGIN(mliImage_at(&dst, 0, 1).b, (30.+40.+70.+80.)/4., 1e-6);
        CHECK_MARGIN(mliImage_at(&dst, 1, 0).b, (55.+34.+98.+26.)/4., 1e-6);
        CHECK_MARGIN(mliImage_at(&dst, 1, 1).b, (31.+17.+11.+23.)/4., 1e-6);
        mliImage_free(&src);
        mliImage_free(&dst);
    }

    {
        mliImage img = mliImage_init();
        mliImage back = mliImage_init();
        uint32_t col;
        uint32_t row;
        double tone;
        CHECK(mliImage_malloc(&img, 3, 2));
        CHECK(img.num_cols == 3u);
        CHECK(img.num_rows == 2u);
        tone = 0.;
        for (col = 0; col < img.num_cols; col++) {
            for (row = 0; row < img.num_rows; row++) {
                mliColor color;
                tone = (double)col * (double)row;
                color.r = tone;
                color.g = tone + 1.;
                color.b = tone + 2.;
                mliImage_set(&img, col, row, color);
            }
        }
        mliImage_write_to_ppm(&img, "img.ppm.tmp");

        CHECK(mliImage_malloc_from_ppm(&back, "img.ppm.tmp"));
        CHECK(back.num_cols == 3u);
        CHECK(back.num_rows == 2u);

        for (col = 0; col < back.num_cols; col++) {
            for (row = 0; row < back.num_rows; row++) {
                mliColor c_in = mliImage_at(&img, col, row);
                mliColor c_back = mliImage_at(&back, col, row);
                CHECK_MARGIN(c_in.r, c_back.r, 1.);
                CHECK_MARGIN(c_in.g, c_back.g, 1.);
                CHECK_MARGIN(c_in.b, c_back.b, 1.);
            }
        }

        mliImage_free(&img);
        mliImage_free(&back);
    }

    /* mliVec */
    {
        mliVec a = {1., 2., 3.};
        CHECK_MARGIN(a.x, 1.0, 1e-6);
        CHECK_MARGIN(a.y, 2.0, 1e-6);
        CHECK_MARGIN(a.z, 3.0, 1e-6);
    }

    {
        mliVec a = {1., 2., 3.};
        mliVec b = {4., 5., 6.};
        mliVec out = mliVec_add(a, b);
        CHECK_MARGIN(out.x, 5.0, 1e-6);
        CHECK_MARGIN(out.y, 7.0, 1e-6);
        CHECK_MARGIN(out.z, 9.0, 1e-6);
    }

    {
        mliVec a = {1., 2., 3.};
        mliVec b = {4., 6., 8.};
        mliVec out = mliVec_substract(a, b);
        CHECK_MARGIN(out.x, -3.0, 1e-6);
        CHECK_MARGIN(out.y, -4.0, 1e-6);
        CHECK_MARGIN(out.z, -5.0, 1e-6);
    }

    {
        mliVec a = {1., 2., 3.};
        mliVec b = {4., 5., 6.};
        CHECK_MARGIN(mliVec_dot(a, b), 32.0, 1e-6);
    }

    {
        mliVec a = {0., 0., 1.};
        mliVec b = {0., 1., 0.};
        CHECK_MARGIN(mliVec_dot(a, b), 0., 1e-6);
    }

    {
        mliVec a = {0., 0., 1.};
        mliVec b = {1., 0., 0.};
        CHECK_MARGIN(mliVec_dot(a, b), 0., 1e-6);
    }

    {
        mliVec a = {0., 0., 1.};
        mliVec b = {0., 0., 1.};
        CHECK_MARGIN(mliVec_dot(a, b), 1., 1e-6);
    }

    {
        mliVec a = {1., 0., 0.};
        mliVec b = {0., 1., 0.};
        mliVec out = mliVec_cross(a, b);
        CHECK_MARGIN(out.x, 0., 1e-6);
        CHECK_MARGIN(out.y, 0., 1e-6);
        CHECK_MARGIN(out.z, 1., 1e-6);
    }

    {
        mliVec a = {1., 0., 0.};
        mliVec b = {0., 0., 1.};
        mliVec out = mliVec_cross(a, b);
        CHECK_MARGIN(out.x, 0., 1e-6);
        CHECK_MARGIN(out.y, -1., 1e-6);
        CHECK_MARGIN(out.z, 0., 1e-6);
    }

    {
        mliVec a = {1., 0., 0.};
        CHECK_MARGIN(mliVec_norm(a), 1., 1e-6);
    }

    {
        mliVec a = {3., 4., 0.};
        CHECK_MARGIN(mliVec_norm(a), 5., 1e-6);
    }

    {
        mliVec a = {0., 0., -1.};
        CHECK_MARGIN(mliVec_norm(a), 1., 1e-6);
    }

    {
        mliVec in = {0., 0., -1.};
        mliVec surface_normal = {0., 0., 1.};
        mliVec out = mliVec_mirror(in, surface_normal);
        CHECK_MARGIN(out.x, 0., 1e-6);
        CHECK_MARGIN(out.y, 0., 1e-6);
        CHECK_MARGIN(out.z, 1., 1e-6);
    }

    {
        mliVec in = {1., 0., -1.};
        mliVec surface_normal = {0., 0., 1.};
        mliVec out = mliVec_mirror(in, surface_normal);
        CHECK_MARGIN(out.x, 1., 1e-6);
        CHECK_MARGIN(out.y, 0., 1e-6);
        CHECK_MARGIN(out.z, 1., 1e-6);
    }

    {
        mliVec a = {0., 0., 0.};
        mliVec b = {0., 0., 0.};
        CHECK(mliVec_equal_margin(a, b, 1e-6));
    }

    {
        mliVec a = {1., 0., 0.};
        mliVec b = {1. + 1e-5, 0., 0.};
        CHECK(!mliVec_equal_margin(a, b, 1e-6));
    }

    {
        /*
            x y z octant
            - - -   0
            - - +   1
            - + -   2
            - + +   3
            + - -   4
            + - +   5
            + + -   6
            + + +   7
        */
        mliVec a;

        const double p = +1.;
        const double n = -1.;

        a.x = n;
        a.y = n;
        a.z = n;
        CHECK(mliVec_octant(a) == 0u);

        a.x = n;
        a.y = n;
        a.z = p;
        CHECK(mliVec_octant(a) == 1u);

        a.x = n;
        a.y = p;
        a.z = n;
        CHECK(mliVec_octant(a) == 2u);

        a.x = n;
        a.y = p;
        a.z = p;
        CHECK(mliVec_octant(a) == 3u);

        a.x = p;
        a.y = n;
        a.z = n;
        CHECK(mliVec_octant(a) == 4u);

        a.x = p;
        a.y = n;
        a.z = p;
        CHECK(mliVec_octant(a) == 5u);

        a.x = p;
        a.y = p;
        a.z = n;
        CHECK(mliVec_octant(a) == 6u);

        a.x = p;
        a.y = p;
        a.z = p;
        CHECK(mliVec_octant(a) == 7u);

        a.x = 0.; a.y = 0.; a.z = p;
        CHECK(mliVec_octant(a) == 7u);

        a.x = 0.; a.y = p; a.z = 0.;
        CHECK(mliVec_octant(a) == 7u);

        a.x = p; a.y = 0.; a.z = 0.;
        CHECK(mliVec_octant(a) == 7u);

        a.x = 0.; a.y = p; a.z = p;
        CHECK(mliVec_octant(a) == 7u);

        a.x = p; a.y = p; a.z = 0.;
        CHECK(mliVec_octant(a) == 7u);

        a.x = p; a.y = 0.; a.z = p;
        CHECK(mliVec_octant(a) == 7u);
    }

    {
        int i;
        mliVec a[3];
        mliVec b[3];
        a[0] = mliVec_set(0., 1., 2.);
        a[1] = mliVec_set(3., 4., 5.);
        a[2] = mliVec_set(6., 7., 8.);

        mliVec_ncpy(a, b, 3);  /* <--- to be tested */

        for (i = 0; i < 3; i++) {
            CHECK(a[i].x == b[i].x);
            CHECK(a[i].y == b[i].y);
            CHECK(a[i].z == b[i].z);}
    }

    {
        mliFunc func = mliFunc_init();
        func.num_points = 0u;
        CHECK(mliFunc_malloc(&func));
        CHECK(func.num_points == 0u);
        CHECK(mliFunc_x_is_causal(&func));
        mliFunc_free(&func);
    }

    {
        mliFunc func = mliFunc_init();
        func.num_points = 3u;
        CHECK(mliFunc_malloc(&func));
        CHECK(func.num_points == 3u);
        func.x[0] = 0.;
        func.x[1] = 1.;
        func.x[2] = 2.;
        CHECK(mliFunc_x_is_causal(&func));
        mliFunc_free(&func);
    }

    {
        double y;
        mliFunc func = mliFunc_init();
        func.num_points = 5u;
        CHECK(mliFunc_malloc(&func));
        CHECK(func.num_points == 5u);

        func.x[0] = 0.;
        func.x[1] = 1.;
        func.x[2] = 2.;
        func.x[3] = 3.;
        func.x[4] = 4.;

        func.y[0] = 4.;
        func.y[1] = 3.;
        func.y[2] = 2.;
        func.y[3] = 1.;
        func.y[4] = 0.;

        CHECK(mliFunc_x_is_causal(&func));
        CHECK(mli_upper_compare_double(func.x, func.num_points, 1.5) == 2);
        CHECK(mliFunc_evaluate(&func, 1.5, &y));
        CHECK(y == 2.);
        mliFunc_free(&func);
    }

    {
        mliMesh m = mliMesh_init();
        mliVec support = {0 ,0 ,0};
        mliVec direction = {0 ,0 ,1};
        uint64_t i;
        mliMesh_malloc_from_object_file("text_on_cube.off", &m);
        for (i = 0; i < m.num_faces; i++) {
            double ray_parameter;
            if(
                mliRay_intersects_triangle(
                    support,
                    direction,
                    m.vertices[m.faces[i].a],
                    m.vertices[m.faces[i].b],
                    m.vertices[m.faces[i].c],
                    &ray_parameter)
            ) {
                CHECK(i == 1782);
                /* printf( "hit in idx %d\n", (int)i); */
            }
        }
        mliMesh_free(&m);
    }

    {
        mliMesh m = mliMesh_init();
        mliMesh_malloc_from_object_file("diff_cube_sphere.off", &m);
        CHECK(m.num_vertices == 432);
        CHECK(m.num_faces == 880);
        mliMesh_free(&m);
    }

    /* mliHexagonalPrismZ */
    {
        /*
                      /\ y
                  ____|____
                 /    |    \
                /     |     \
            __ /______|______\___\ x
               \      |      /   /
                \     |     /
                 \____|____/
                      |
        */
        mliVec c;
        double inner_radius = 0.5;
        double outer_radius = inner_radius/cos(MLI_PI/6.0);
        c.z = 0.;
        for (c.x = -1.; c.x < 1.; c.x = c.x + 0.01) {
            for (c.y = -1.; c.y < 1.; c.y = c.y + 0.01) {
                if (mliVec_norm(c) <= inner_radius)
                    CHECK(mli_inside_hexagonal_prism_z(c, inner_radius));
                else if(mliVec_norm(c) > outer_radius)
                    CHECK(!mli_inside_hexagonal_prism_z(c, inner_radius));
                else
                    continue;
            }
        }

        /*
                      /\ y
                  ____|____
                 /    |    \
                /     |     \
            __ /______|_____X\___\ x
               \      |      /   /
                \     |     /
                 \____|____/
                      |
        */
        CHECK(
            mli_inside_hexagonal_prism_z(
                mliVec_set(inner_radius + 1e-6, 0., 0.),
                inner_radius));

        /*
                      /\ y
                  ____X____
                 /    |    \
                /     |     \
            __ /______|______\___\ x
               \      |      /   /
                \     |     /
                 \____|____/
                      |
        */
        CHECK(
            !mli_inside_hexagonal_prism_z(
                mliVec_set(0., inner_radius + 1e-6, 0.),
                inner_radius));
    }

    /* hexagon corners */
    {
        /*
                      /\ y
                2 ____|____ 1
                 /    |    \
                /     |     \
            __ /______|______\_0_\ x
             3 \      |      /   /
                \     |     /
                 \____|____/
                4     |    5
        */
        uint64_t i;
        for (i = 0; i < 20; i++) {
            mliVec corner =  mli_hexagon_corner(i);
            CHECK_MARGIN(mliVec_norm(corner), 1., 1e-6);
        }
        CHECK(mli_hexagon_corner(0).x == 1.);
        CHECK(mli_hexagon_corner(0).y == 0.);
        CHECK(mli_hexagon_corner(0).z == 0.);

        CHECK(mli_hexagon_corner(1).x > 0.);
        CHECK(mli_hexagon_corner(1).y > 0.);
        CHECK(mli_hexagon_corner(1).z == 0.);

        CHECK(mli_hexagon_corner(2).x < 0.);
        CHECK(mli_hexagon_corner(2).y > 0.);
        CHECK(mli_hexagon_corner(2).z == 0.);

        CHECK(mli_hexagon_corner(3).x == -1.);
        CHECK(mli_hexagon_corner(3).y == 0.);
        CHECK(mli_hexagon_corner(3).z == 0.);

        CHECK(mli_hexagon_corner(4).x < 0.);
        CHECK(mli_hexagon_corner(4).y < 0.);
        CHECK(mli_hexagon_corner(4).z == 0.);

        CHECK(mli_hexagon_corner(5).x > 0.);
        CHECK(mli_hexagon_corner(5).y < 0.);
        CHECK(mli_hexagon_corner(5).z == 0.);
    }

    /* mliSphericalCap */
    {
        const double radius = 0.3;
        const mliVec support = {0., 0., 1.};
        const mliVec direction = {0., 0., -1.};
        const mliRay ray = mliRay_set(support, direction);
        double plus_solution, minus_solution;
        mliVec intersection_point;
        mliVec surface_normal;
        CHECK(mli_spherical_cap_equation(
                ray,
                radius,
                &plus_solution,
                &minus_solution));
        CHECK_MARGIN(plus_solution, 1.0, 1e-9);
        CHECK_MARGIN(minus_solution, (1.0 - 2.*radius), 1e-9);

        intersection_point = mliRay_at(&ray, plus_solution);
        surface_normal = mli_spherical_cap_surface_normal(
            intersection_point,
            radius);
        CHECK_MARGIN(surface_normal.x, 0., 1e-9);
        CHECK_MARGIN(surface_normal.y, 0., 1e-9);
        CHECK_MARGIN(surface_normal.z, 1., 1e-9);
    }

    {
        mliSphere sphere;
        mliIntersection intersection;
        mliHomTraComp local2root_comp;
        sphere.radius = 1.;
        local2root_comp.trans = mliVec_set(0., 0., 0.);
        local2root_comp.rot = mliQuaternion_set_rotaxis_and_angle(
            mliVec_set(0., 0., 0.),
            0.);

        CHECK(!mliSphere_intersection(
            sphere,
            local2root_comp,
            mliRay_set(mliVec_set(0., 0., 5.), mliVec_set(0., 0., 1.)),
            &intersection));

        CHECK(mliSphere_intersection(
            sphere,
            local2root_comp,
            mliRay_set(mliVec_set(0., 0., -5.), mliVec_set(0., 0., 1.)),
            &intersection));
        CHECK_MARGIN(intersection.distance_of_ray, 4., 1e-6);
    }

    /* mliCylinder */
    {
        mliCylinder a, b;
        a.radius = 1.;
        a.length = 2.;
        b.radius = 1.;
        b.length = 2.;
        CHECK(mliCylinder_is_equal(a, b));
        b.length = .5;
        CHECK(!mliCylinder_is_equal(a, b));
    }

    {
        mliCylinder a;
        mliVec start_local;
        mliVec end_local;
        a.radius = .1;
        a.length = .5;
        start_local = mliCylinder_start_local(a);
        CHECK_MARGIN(start_local.x, 0., 1e-6);
        CHECK_MARGIN(start_local.y, 0., 1e-6);
        CHECK_MARGIN(start_local.z, -a.length/2, 1e-6);
        end_local = mliCylinder_end_local(a);
        CHECK_MARGIN(end_local.x, 0., 1e-6);
        CHECK_MARGIN(end_local.y, 0., 1e-6);
        CHECK_MARGIN(end_local.z, a.length/2, 1e-6);
    }

    /* slim cylinder obb */
    {
        mliCylinder a;
        mliOBB obb;
        mliHomTraComp trafo;
        a.radius = .5;
        a.length = 12.;
        trafo.trans = mliVec_set(0., 0., 0.);
        trafo.rot = mliQuaternion_set_rotaxis_and_angle(
            mliVec_set(0., 0., 1.),
            0.);
        obb = mliCylinder_obb(a, trafo);
        CHECK_MARGIN(obb.lower.x, -.5, 1e-6);
        CHECK_MARGIN(obb.lower.y, -.5, 1e-6);
        CHECK_MARGIN(obb.lower.z, -6.5, 1e-6);
        CHECK_MARGIN(obb.upper.x, .5, 1e-6);
        CHECK_MARGIN(obb.upper.y, .5, 1e-6);
        CHECK_MARGIN(obb.upper.z, 6.5, 1e-6);
    }

    /* fat cylinder obb */
    {
        mliCylinder a;
        mliOBB obb;
        mliHomTraComp trafo;
        a.radius = 5;
        a.length = 1.;
        trafo.trans = mliVec_set(0., 0., 0.);
        trafo.rot = mliQuaternion_set_rotaxis_and_angle(
            mliVec_set(0., 0., 1.),
            0.);
        obb = mliCylinder_obb(a, trafo);
        CHECK_MARGIN(obb.lower.x, -5., 1e-6);
        CHECK_MARGIN(obb.lower.y, -5., 1e-6);
        CHECK_MARGIN(obb.lower.z, -5.5, 1e-6);
        CHECK_MARGIN(obb.upper.x, 5., 1e-6);
        CHECK_MARGIN(obb.upper.y, 5., 1e-6);
        CHECK_MARGIN(obb.upper.z, 5.5, 1e-6);
    }

    /* slim cylinder obb transformed */
    {
        mliCylinder a;
        mliOBB obb;
        mliHomTraComp trafo;
        a.radius = .5;
        a.length = 12.;
        trafo.trans = mliVec_set(0., 0., 1.);
        trafo.rot = mliQuaternion_set_rotaxis_and_angle(
            mliVec_set(1., 0., 0.),
            mli_deg2rad(90));
        obb = mliCylinder_obb(a, trafo);
        CHECK_MARGIN(obb.lower.x, -.5, 1e-6);
        CHECK_MARGIN(obb.lower.y, -6.5, 1e-6);
        CHECK_MARGIN(obb.lower.z, 0.5, 1e-6);
        CHECK_MARGIN(obb.upper.x, .5, 1e-6);
        CHECK_MARGIN(obb.upper.y, 6.5, 1e-6);
        CHECK_MARGIN(obb.upper.z, 1.5, 1e-6);
    }

    /* cylinder overlap with its own obb */
    {
        mliCylinder a;
        mliOBB obb;
        mliHomTraComp local2root_comp;
        a.radius = .5;
        a.length = 12.;
        local2root_comp.trans = mliVec_set(0., 0., 1.);
        local2root_comp.rot = mliQuaternion_set_rotaxis_and_angle(
            mliVec_set(1., 0., 0.),
            mli_deg2rad(90));
        obb = mliCylinder_obb(a, local2root_comp);
        CHECK(
            mliCylinder_has_overlap_obb(
                a,
                local2root_comp,
                obb));
    }

    /* cylinder overlap with its own obb */
    {
        mliCylinder a;
        mliOBB obb;
        mliHomTraComp local2root_comp;
        a.radius = .5;
        a.length = 12.;
        local2root_comp.trans = mliVec_set(0., 0., 1.);
        local2root_comp.rot = mliQuaternion_set_rotaxis_and_angle(
            mliVec_set(1., 0., 0.),
            mli_deg2rad(90));

        /* lower x */
        obb.lower = mliVec_set(-10, -10, -10);
        obb.upper = mliVec_set(-.51, 10, 10);
        CHECK(
            !mliCylinder_has_overlap_obb(
                a,
                local2root_comp,
                obb));
        obb.upper = mliVec_set(-0.49, 10, 10);
        CHECK(
            mliCylinder_has_overlap_obb(
                a,
                local2root_comp,
                obb));

        /* upper x */
        obb.lower = mliVec_set(.51, -10, -10);
        obb.upper = mliVec_set(10, 10, 10);
        CHECK(
            !mliCylinder_has_overlap_obb(
                a,
                local2root_comp,
                obb));
        obb.lower = mliVec_set(.49, -10, -10);
        CHECK(
            mliCylinder_has_overlap_obb(
                a,
                local2root_comp,
                obb));

        /* lower y */
        obb.lower = mliVec_set(-10, -10, -10);
        obb.upper = mliVec_set(10, -6.51, 10);
        CHECK(
            !mliCylinder_has_overlap_obb(
                a,
                local2root_comp,
                obb));
        obb.upper = mliVec_set(10, -6.49, 10);
        CHECK(
            mliCylinder_has_overlap_obb(
                a,
                local2root_comp,
                obb));

        /* upper y */
        obb.lower = mliVec_set(-10, 6.51, -10);
        obb.upper = mliVec_set(10, 10, 10);
        CHECK(
            !mliCylinder_has_overlap_obb(
                a,
                local2root_comp,
                obb));
        obb.lower = mliVec_set(-10, 6.49, -10);
        CHECK(
            mliCylinder_has_overlap_obb(
                a,
                local2root_comp,
                obb));

        /* lower z */
        obb.lower = mliVec_set(-10, -10, -10);
        obb.upper = mliVec_set(10, 10, .49);
        CHECK(
            !mliCylinder_has_overlap_obb(
                a,
                local2root_comp,
                obb));
        obb.upper = mliVec_set(10, 10, .51);
        CHECK(
            mliCylinder_has_overlap_obb(
                a,
                local2root_comp,
                obb));

        /* upper z */
        obb.lower = mliVec_set(-10, -10, 1.51);
        obb.upper = mliVec_set(10, 10, 10);
        CHECK(
            !mliCylinder_has_overlap_obb(
                a,
                local2root_comp,
                obb));
        obb.lower = mliVec_set(-10, -10, 1.49);
        CHECK(
            mliCylinder_has_overlap_obb(
                a,
                local2root_comp,
                obb));
    }

    /* cylinder intersection test */
    {
        double plus_solution, minus_solution;
        /* ray starts infron of cylinder */
        CHECK(
            mli_cylinder_equation(
                1.,
                mliRay_set(
                    mliVec_set(-5.,0., 0.),
                    mliVec_set(1., 0., 0.)),
                &plus_solution,
                &minus_solution));
        CHECK_MARGIN(minus_solution, 4., 1e-6);
        CHECK_MARGIN(plus_solution, 6., 1e-6);

        /* ray starts inside cylinder */
        CHECK(
            mli_cylinder_equation(
                1.,
                mliRay_set(
                    mliVec_set(0.,0., 0.),
                    mliVec_set(1., 0., 0.)),
                &plus_solution,
                &minus_solution));
        CHECK_MARGIN(minus_solution, -1., 1e-6);
        CHECK_MARGIN(plus_solution, +1., 1e-6);

        /* ray starts behind cylinder */
        CHECK(
            mli_cylinder_equation(
                1.,
                mliRay_set(
                    mliVec_set(5.,0., 0.),
                    mliVec_set(1., 0., 0.)),
                &plus_solution,
                &minus_solution));
        CHECK_MARGIN(minus_solution, -6., 1e-6);
        CHECK_MARGIN(plus_solution, -4., 1e-6);

        /* ray runs beside cylinder */
        CHECK(
            !mli_cylinder_equation(
                1.,
                mliRay_set(
                    mliVec_set(-5., 5., 0.),
                    mliVec_set(1., 0., 0.)),
                &plus_solution,
                &minus_solution));


        /* ray runs inside and parallel to cylinder */
        CHECK(
            !mli_cylinder_equation(
                1.,
                mliRay_set(
                    mliVec_set(0., 0., 0.),
                    mliVec_set(0., 0., 1.)),
                &plus_solution,
                &minus_solution));

    }

    {
        mliCylinder cylinder;
        mliHomTraComp local2root_comp;
        mliIntersection intersection;
        local2root_comp.trans = mliVec_set(0., 0., 0.);
        local2root_comp.rot = mliQuaternion_set_rotaxis_and_angle(
            mliVec_set(0., 0., 1.),
            mli_deg2rad(0.));

        cylinder.radius = 1.;
        cylinder.length = 2.;
        CHECK(
            mliCylinder_intersection(
                cylinder,
                local2root_comp,
                mliRay_set(
                    mliVec_set(0., 0., 0.),
                    mliVec_set(1., 0., 0.)),
                &intersection));
        CHECK_MARGIN(intersection.distance_of_ray, 1., 1e-6);

        CHECK(
            !mliCylinder_intersection(
                cylinder,
                local2root_comp,
                mliRay_set(
                    mliVec_set(1., 0., 0.),
                    mliVec_set(1., 0., 0.)),
                &intersection));

        CHECK(
            mliCylinder_intersection(
                cylinder,
                local2root_comp,
                mliRay_set(
                    mliVec_set(1., 0., 0.),
                    mliVec_set(-1., 0., 0.)),
                &intersection));
        CHECK_MARGIN(intersection.distance_of_ray, 2., 1e-6);
    }

    /* mliHexagon */
    {
        mliHexagon a;
        mliHexagon b;
        mliHexagon c;
        a.inner_radius = 1.;
        b.inner_radius = 1.;
        c.inner_radius = 0.5;
        CHECK(mliHexagon_is_equal(a, b));
        CHECK(!mliHexagon_is_equal(a, c));
        CHECK(!mliHexagon_is_equal(b, c));
    }

    /* mliHexagon_intersection */
    {
        mliHexagon hex;
        mliHomTraComp local2root_comp;
        mliIntersection intersection;
        hex.inner_radius = 1.;
        local2root_comp.trans = mliVec_set(0., 0., 0.);
        local2root_comp.rot = mliQuaternion_set_rotaxis_and_angle(
            mliVec_set(0., 0., 1.),
            mli_deg2rad(0.));

        CHECK(
            mliHexagon_intersection(
                hex,
                local2root_comp,
                mliRay_set(
                    mliVec_set(0., 0., 1.),
                    mliVec_set(0., 0., -1.)),
                &intersection));
        CHECK_MARGIN(intersection.distance_of_ray, 1., 1e-6);

        /*
                      /\ y
                  ____|____
                 /    |    \
                /     |     \   <- P is inside
            __ /______|_____P\___\ x
               \      |      /   /
                \     |     /
                 \____|____/
                      |
        */
        CHECK(
            mliHexagon_intersection(
                hex,
                local2root_comp,
                mliRay_set(
                    mliVec_set(1.001, 0., 1.),
                    mliVec_set(0., 0., -1.)),
                &intersection));
        CHECK_MARGIN(intersection.distance_of_ray, 1., 1e-6);

        /*
                      /\ y
                  ____P____ <- P is outside
                 /    |    \
                /     |     \
            __ /______|______\___\ x
               \      |      /   /
                \     |     /
                 \____|____/
                      |
        */
        CHECK(
            !mliHexagon_intersection(
                hex,
                local2root_comp,
                mliRay_set(
                    mliVec_set(0., 1.001, 1.),
                    mliVec_set(0., 0., -1.)),
                &intersection));
    }

    {
        mliHexagon hex;
        mliHomTraComp local2root_comp;
        mliOBB obb;
        hex.inner_radius = 1.;

        local2root_comp.trans = mliVec_set(0., 0., 0.);
        local2root_comp.rot = mliQuaternion_set_rotaxis_and_angle(
            mliVec_set(0., 0., 1.),
            mli_deg2rad(0.));
        obb = mliHexagon_obb(hex, local2root_comp);
        CHECK_MARGIN(obb.lower.x, -MLI_2_OVER_SQRT3, 1e-6);
        CHECK_MARGIN(obb.upper.x, +MLI_2_OVER_SQRT3, 1e-6);
        CHECK_MARGIN(obb.lower.y, -1., 1e-6);
        CHECK_MARGIN(obb.upper.y, +1., 1e-6);
        CHECK_MARGIN(obb.lower.z, 0., 1e-6);
        CHECK_MARGIN(obb.upper.z, 0., 1e-6);

        /* translation */
        local2root_comp.trans = mliVec_set(1., 2., 3.);
        local2root_comp.rot = mliQuaternion_set_rotaxis_and_angle(
            mliVec_set(0., 0., 1.),
            mli_deg2rad(0.));
        obb = mliHexagon_obb(hex, local2root_comp);
        CHECK_MARGIN(obb.lower.x, -MLI_2_OVER_SQRT3 + 1., 1e-6);
        CHECK_MARGIN(obb.upper.x, +MLI_2_OVER_SQRT3 + 1., 1e-6);
        CHECK_MARGIN(obb.lower.y, -1. + 2., 1e-6);
        CHECK_MARGIN(obb.upper.y, +1. + 2., 1e-6);
        CHECK_MARGIN(obb.lower.z, 0. + 3., 1e-6);
        CHECK_MARGIN(obb.upper.z, 0. + 3., 1e-6);

        /* rotation y-axis 90deg*/
        local2root_comp.trans = mliVec_set(0., 0., 0.);
        local2root_comp.rot = mliQuaternion_set_rotaxis_and_angle(
            mliVec_set(0., 1., 0.),
            mli_deg2rad(90.));
        obb = mliHexagon_obb(hex, local2root_comp);
        CHECK_MARGIN(obb.lower.x, 0., 1e-6);
        CHECK_MARGIN(obb.upper.x, 0., 1e-6);
        CHECK_MARGIN(obb.lower.y, -1., 1e-6);
        CHECK_MARGIN(obb.upper.y, +1., 1e-6);
        CHECK_MARGIN(obb.lower.z, -MLI_2_OVER_SQRT3, 1e-6);
        CHECK_MARGIN(obb.upper.z, +MLI_2_OVER_SQRT3, 1e-6);

        /* rotation y-axis 45deg */
        local2root_comp.trans = mliVec_set(0., 0., 0.);
        local2root_comp.rot = mliQuaternion_set_rotaxis_and_angle(
            mliVec_set(0., 1., 0.),
            mli_deg2rad(45.));
        obb = mliHexagon_obb(hex, local2root_comp);
        CHECK_MARGIN(obb.lower.x, -MLI_2_OVER_SQRT3/sqrt(2), 1e-6);
        CHECK_MARGIN(obb.upper.x, +MLI_2_OVER_SQRT3/sqrt(2), 1e-6);
        CHECK_MARGIN(obb.lower.y, -1., 1e-6);
        CHECK_MARGIN(obb.upper.y, +1., 1e-6);
        CHECK_MARGIN(obb.lower.z, -MLI_2_OVER_SQRT3/sqrt(2), 1e-6);
        CHECK_MARGIN(obb.upper.z, +MLI_2_OVER_SQRT3/sqrt(2), 1e-6);
    }

    {
        mliHexagon hex;
        mliHomTraComp local2root_comp;
        mliOBB obb;
        hex.inner_radius = 1.;
        local2root_comp.trans = mliVec_set(0., 0., 0.);
        local2root_comp.rot = mliQuaternion_set_rotaxis_and_angle(
            mliVec_set(0., 0., 1.),
            mli_deg2rad(0.));

        /*
            _obb____________________  + 2
           |                        |
           |                        |
           |       _________ + 1    |
           |      /         \       |
           |     /           \      |
           |    /             \     |
           |    \             /     |
           |     \           /      |
           |      \_________/ - 1   |
           |                        |
           |                        |
           |________________________| - 2

            y
            |__x
        */
        obb.lower = mliVec_set(-2., -2., -2.);
        obb.upper = mliVec_set(2., 2., 2.);
        CHECK(mliHexagon_has_overlap_obb(hex, local2root_comp, obb));

        /*
               obb   |                                                         *
                     |                                                         *
                   __|______ + 1                                               *
            _____ /__|      \                                                  *
                 /           \                                                 *
                /             \                                                *
                \             /                                                *
                 \           /                                                 *
                  \_________/ - 1                                              *
                                                                               *
                                                                               *
            y
            |__x
        */
        obb.lower = mliVec_set(-3., -3., -1.);
        obb.upper = mliVec_set(-0.5, 0.5, 1.);
        CHECK(mliHexagon_has_overlap_obb(hex, local2root_comp, obb));

        /*
                                                                               *
                                                                               *
                   _________ + 1                                               *
                  /  _____  \                                                  *
                 /  | obb |  \                                                 *
                /   |     |   \                                                *
                \   |_____|   /                                                *
                 \           /                                                 *
                  \_________/ - 1                                              *
                                                                               *
                                                                               *
            y
            |__x
        */
        obb.lower = mliVec_set(-.5, -.5, -1.);
        obb.upper = mliVec_set(.5, .5, 1.);
        CHECK(mliHexagon_has_overlap_obb(hex, local2root_comp, obb));

        /*
                                                                               *
                                                                               *
                   _________ + 1                                               *
                  /         \         _____                                    *
                 /           \       | obb |                                   *
                /             \      |     |                                   *
                \             /      |_____|                                   *
                 \           /                                                 *
                  \_________/ - 1                                              *
                                                                               *
                                                                               *
            y
            |__x
        */
        obb.lower = mliVec_set(6, -.5, -1.);
        obb.upper = mliVec_set(7, .5, 1.);
        CHECK(!mliHexagon_has_overlap_obb(hex, local2root_comp, obb));

        /*
                                                                               *
                                                                               *
                                   _________ + 1                               *
                                  /  _____  \                                  *
                                 /  | obb |  \                                 *
                                /   |     |   \                                *
                                \   |_____|   /                                *
                                 \           /                                 *
                                  \_________/ - 1                              *
                                                                               *
                                                                               *
            y
            |__x
        */
        local2root_comp.trans = mliVec_set(6.5, 0., 0.);
        obb.lower = mliVec_set(6, -.5, -1.);
        obb.upper = mliVec_set(7, .5, 1.);
        CHECK(mliHexagon_has_overlap_obb(hex, local2root_comp, obb));
    }

    /* mliBiCirclePlane OBB */
    {
        mliOBB obb;
        mliBiCirclePlane plane;
        mliHomTraComp local2root_comp;
        plane.x_height = 1.;
        plane.y_width = 2.;

        local2root_comp.trans = mliVec_set(0., 0., 0.);
        local2root_comp.rot = mliQuaternion_set_rotaxis_and_angle(
            mliVec_set(0., 0., 1.),
            mli_deg2rad(0.));
        obb = mliBiCirclePlane_obb(plane, local2root_comp);
        CHECK_MARGIN(obb.lower.x, -.5, 1e-6);
        CHECK_MARGIN(obb.upper.x, +.5, 1e-6);
        CHECK_MARGIN(obb.lower.y, -1., 1e-6);
        CHECK_MARGIN(obb.upper.y, 1., 1e-6);
        CHECK_MARGIN(obb.lower.z, 0., 1e-6);
        CHECK_MARGIN(obb.upper.z, 0., 1e-6);

        local2root_comp.trans = mliVec_set(0., 0., 0.);
        local2root_comp.rot = mliQuaternion_set_rotaxis_and_angle(
            mliVec_set(0., 0., 1.),
            mli_deg2rad(90.));          /* <-- rotation */
        obb = mliBiCirclePlane_obb(plane, local2root_comp);
        CHECK_MARGIN(obb.lower.x, -1., 1e-6);
        CHECK_MARGIN(obb.upper.x, 1., 1e-6);
        CHECK_MARGIN(obb.lower.y, -.5, 1e-6);
        CHECK_MARGIN(obb.upper.y, .5, 1e-6);
        CHECK_MARGIN(obb.lower.z, 0., 1e-6);
        CHECK_MARGIN(obb.upper.z, 0., 1e-6);

        local2root_comp.trans = mliVec_set(0., 0., 0.);
        local2root_comp.rot = mliQuaternion_set_rotaxis_and_angle(
            mliVec_set(1., 0., 0.),
            mli_deg2rad(45.));
        obb = mliBiCirclePlane_obb(plane, local2root_comp);
        CHECK_MARGIN(obb.lower.x, -.5, 1e-6);
        CHECK_MARGIN(obb.upper.x, .5, 1e-6);
        CHECK_MARGIN(obb.lower.y, -sqrt(2.)/2, 1e-6);
        CHECK_MARGIN(obb.upper.y, sqrt(2.)/2, 1e-6);
        CHECK_MARGIN(obb.lower.z, -sqrt(2.)/2, 1e-6);
        CHECK_MARGIN(obb.upper.z, sqrt(2.)/2, 1e-6);

        local2root_comp.trans = mliVec_set(0., 0., 0.);
        local2root_comp.rot = mliQuaternion_set_rotaxis_and_angle(
            mliVec_set(0., 1., 0.),
            mli_deg2rad(45.));
        obb = mliBiCirclePlane_obb(plane, local2root_comp);
        CHECK_MARGIN(obb.lower.x, -sqrt(2.)/4, 1e-6);
        CHECK_MARGIN(obb.upper.x, sqrt(2.)/4, 1e-6);
        CHECK_MARGIN(obb.lower.y, -1., 1e-6);
        CHECK_MARGIN(obb.upper.y, 1., 1e-6);
        CHECK_MARGIN(obb.lower.z, -sqrt(2.)/4, 1e-6);
        CHECK_MARGIN(obb.upper.z, sqrt(2.)/4, 1e-6);
    }

    {
        mliOBB obb;
        mliBiCirclePlane plane;
        mliHomTraComp local2root_comp;
        plane.x_height = 1.;
        plane.y_width = 2.;
        local2root_comp.trans = mliVec_set(0., 0., 0.);
        local2root_comp.rot = mliQuaternion_set_rotaxis_and_angle(
            mliVec_set(0., 0., 1.),
            mli_deg2rad(0.));

        obb.lower = mliVec_set(-1., -1., -1.);
        obb.upper = mliVec_set(1., 1., 1.);
        CHECK(mliBiCirclePlane_has_overlap_obb(plane, local2root_comp, obb));

        obb.lower = mliVec_set(-1., -1., -1.);
        obb.upper = mliVec_set(-0.51, 1., 1.);
        CHECK(!mliBiCirclePlane_has_overlap_obb(plane, local2root_comp, obb));
        obb.lower = mliVec_set(-1., -1., -1.);
        obb.upper = mliVec_set(-0.49, 1., 1.);
        CHECK(mliBiCirclePlane_has_overlap_obb(plane, local2root_comp, obb));

        obb.lower = mliVec_set(-1., -2., -1.);
        obb.upper = mliVec_set(1., -1.1, 1.);
        CHECK(!mliBiCirclePlane_has_overlap_obb(plane, local2root_comp, obb));
        obb.lower = mliVec_set(-1., -2., -1.);
        obb.upper = mliVec_set(1., -0.9, 1.);
        CHECK(mliBiCirclePlane_has_overlap_obb(plane, local2root_comp, obb));

        obb.lower = mliVec_set(-1., -1., -1.);
        obb.upper = mliVec_set(1., 1., -.01);
        CHECK(!mliBiCirclePlane_has_overlap_obb(plane, local2root_comp, obb));
        obb.lower = mliVec_set(-1., -1., -1.);
        obb.upper = mliVec_set(1., 1., .01);
        CHECK(mliBiCirclePlane_has_overlap_obb(plane, local2root_comp, obb));
    }

    {
        mliBiCirclePlane plane;
        mliHomTraComp local2root_comp;
        mliIntersection intersection;
        plane.x_height = 1.;
        plane.y_width = 2.;
        local2root_comp.trans = mliVec_set(0., 0., 0.);
        local2root_comp.rot = mliQuaternion_set_rotaxis_and_angle(
            mliVec_set(0., 0., 1.),
            mli_deg2rad(0.));

        CHECK(mliBiCirclePlane_intersection(
            plane,
            local2root_comp,
            mliRay_set(
                mliVec_set(0., 0., -1.),
                mliVec_set(0., 0., 1.)),
            &intersection));
        CHECK_MARGIN(intersection.distance_of_ray, 1., 1e-6);

        CHECK(!mliBiCirclePlane_intersection(
            plane,
            local2root_comp,
            mliRay_set(
                mliVec_set(0., 3., -1.),
                mliVec_set(0., 0., 1.)),
            &intersection));
    }

    /* mliDeisc */
    {
        mliDisc disc;
        mliHomTraComp local2root_comp;
        mliOBB obb;
        disc.radius = 1.;
        local2root_comp.trans = mliVec_set(0., 0., 0.);
        local2root_comp.rot = mliQuaternion_set_rotaxis_and_angle(
            mliVec_set(0., 0., 0.),
            0.);

        obb.lower = mliVec_set(-1, -1, -1.);
        obb.upper = mliVec_set(1., 1., 1.);
        CHECK(mliDisc_has_overlap_obb(disc, local2root_comp, obb));

        obb.lower = mliVec_set(-1, -1, .1);
        obb.upper = mliVec_set(1., 1., 2.1);
        CHECK(!mliDisc_has_overlap_obb(disc, local2root_comp, obb));
    }
    /* mliRay OBB */
    {
        mliOBB obb;
        /*         (-1, -2,  0)                         (-1, -1,  0)
         *                     O----------------------O
         *                    /.                     /|
         *                   / .                    / |
         *                  /  .                   /  |
         *                 /   .                  /   |
         *   ( 1, -2,  0) O----------------------O ( 1, -1,  0)
         *                |    .                 |    |
         *      Z         |    .                 |    |
         *      | (-1, -2,|-3) O- - - - - - - - -| - -O (-1, -1, -3)
         *      |         |   .                  |   /
         *      |         |  .                   |  /
         *      /-----Y   | .                    | /
         *     /          |.                     |/
         *    X           O----------------------O
         *     ( 1, -2, -3)                       ( 1, -1, -3)
         */

        double ray_parameter;
        obb.lower = mliVec_set(-1., -2, -3);
        obb.upper = mliVec_set(1., -1, 0);

        /* ray starts below the box */
        CHECK(
            mliRay_has_overlap_obb(
                mliRay_set(
                    mliVec_set(0., -1.5, -4.),
                    mliVec_set(0., 0., 1.)),
                obb,
                &ray_parameter));
        CHECK_MARGIN(ray_parameter, 1., 1e-6);

        /* ray starts above the box */
        CHECK(
            !mliRay_has_overlap_obb(
                mliRay_set(
                    mliVec_set(0., -1.5, +4.),
                    mliVec_set(0., 0., 1.)),
                obb,
                &ray_parameter));

        /* ray starts inside the box */
        CHECK(
            mliRay_has_overlap_obb(
                mliRay_set(
                    mliVec_set(0., -1.5, -1.5),
                    mliVec_set(0., 0., 1.)),
                obb,
                &ray_parameter));
        CHECK_MARGIN(ray_parameter, -1.5, 1e-6);

        /* ray starts beside the box */
        CHECK(
            !mliRay_has_overlap_obb(
                mliRay_set(
                    mliVec_set(10, 10, -5),
                    mliVec_set(0., 0., 1.)),
                obb,
                &ray_parameter));
    }

    {
        double causal_solution;
        int rc;
        mliBoundSurfaceChecklist cl;

        /*       _/_ps
         *      //  \
         *     |/ ms |
         *     /\___/
         *    /
         *   X support
         */
        cl.plus_solution = 2.0;
        cl.minus_solution = 1.0;
        cl.plus_is_inside = 1;
        cl.minus_is_inside = 1;
        rc = mli_outer_bound_surface_causal_intersection(
            cl,
            &causal_solution);
        CHECK(causal_solution == cl.minus_solution);
        CHECK(rc);

        /*       _/_ps
         *      //  \
         *     |X support
         *  ms /\___/
         *    /
         *
         */
        cl.plus_solution = 1.0;
        cl.minus_solution = -1.0;
        cl.plus_is_inside = 1;
        cl.minus_is_inside = 1;
        rc = mli_outer_bound_surface_causal_intersection(
            cl,
            &causal_solution);
        CHECK(causal_solution == cl.plus_solution);
        CHECK(rc);

        /*          /
         *         X support
         *       _/_ps
         *      //  \
         *     |/    |
         *  ms /\___/
         *    /
         *
         */
        cl.plus_solution = -1.0;
        cl.minus_solution = -2.0;
        cl.plus_is_inside = 1;
        cl.minus_is_inside = 1;
        rc = mli_outer_bound_surface_causal_intersection(
            cl,
            &causal_solution);
        CHECK(!rc);
    }

    {
        mliSphericalCapHex cap;
        double bounding_radius;
        cap.curvature_radius = 1.0;
        cap.inner_hex_radius = .1;
        bounding_radius = mliSphericalCapHex_bounding_radius(cap);
        CHECK(bounding_radius > cap.inner_hex_radius*MLI_2_OVER_SQRT3);
        CHECK(
            bounding_radius < 1.1*cap.inner_hex_radius*MLI_2_OVER_SQRT3);
    }

    {
        mliHomTraComp Tcomp;
        mliHomTra T;
        mliVec v1, v2, v3;
        Tcomp.trans = mliVec_set(1., 0., 0.);
        Tcomp.rot = mliQuaternion_set_rotaxis_and_angle(
            mliVec_set(0., 0., 1.),
            mli_deg2rad(90));
        T = mliHomTra_from_compact(Tcomp);
        v1 = mliVec_set(1., 0., 0.);
        v2 = mliHomTra_dir(&T, v1);
        CHECK_MARGIN(v2.x, 0., 1e-6);
        CHECK_MARGIN(v2.y, 1., 1e-6);
        CHECK_MARGIN(v2.z, 0., 1e-6);

        v3 = mliHomTra_dir_inverse(&T, v2);
        CHECK_MARGIN(v3.x, v1.x, 1e-6);
        CHECK_MARGIN(v3.y, v1.y, 1e-6);
        CHECK_MARGIN(v3.z, v1.z, 1e-6);
    }

    /* unity transformation must not change ray */
    {
        mliRay ray = mliRay_set(
            mliVec_set(0., 0., 1.),
            mliVec_set(0., 0., 1.));
        mliRay ray2;
        mliHomTraComp Tcomp;
        mliHomTra T;
        Tcomp.trans = mliVec_set(0., 0., 0.);
        Tcomp.rot = mliQuaternion_set_rotaxis_and_angle(
            mliVec_set(0., 0., 0.),
            0.);
        T = mliHomTra_from_compact(Tcomp);
        ray2 = mliHomTra_ray(&T, ray);
        CHECK(mliVec_equal_margin(ray2.support, ray.support, 1e-6));
        CHECK(mliVec_equal_margin(ray2.direction, ray.direction, 1e-6));
    }

    /* translation */
    {
        mliRay ray = mliRay_set(
            mliVec_set(0., 0., 1.),
            mliVec_set(0., 0., 1.));
        mliRay ray2;
        mliHomTraComp Tcomp;
        mliHomTra T;
        Tcomp.trans = mliVec_set(1., 0., 0.);
        Tcomp.rot = mliQuaternion_set_rotaxis_and_angle(
            mliVec_set(0., 0., 1.),
            0.);
        T = mliHomTra_from_compact(Tcomp);
        ray2 = mliHomTra_ray(&T, ray);
        CHECK(ray2.support.x == 1.);
        CHECK(ray2.support.y == 0.);
        CHECK(ray2.support.z == 1.);
        CHECK(mliVec_equal_margin(ray2.direction, ray.direction, 1e-6));
    }

    /* rotation */
    {
        mliRay ray = mliRay_set(
            mliVec_set(0., 0., 1.),
            mliVec_set(1., 0., 0.));
        mliRay ray2;
        mliHomTraComp Tcomp;
        mliHomTra T;
        Tcomp.trans = mliVec_set(0., 0., 0.);
        Tcomp.rot = mliQuaternion_set_rotaxis_and_angle(
            mliVec_set(0., 0., 1.),
            mli_deg2rad(90));
        T = mliHomTra_from_compact(Tcomp);
        ray2 = mliHomTra_ray(&T, ray);
        CHECK(mliVec_equal_margin(ray2.support, ray.support, 1e-6));
        CHECK_MARGIN(ray2.direction.x, 0., 1e-6);
        CHECK_MARGIN(ray2.direction.y, 1., 1e-6);
        CHECK_MARGIN(ray2.direction.z, 0., 1e-6);
    }

    /* translation and rotation */
    {
        mliRay ray = mliRay_set(
            mliVec_set(0., 0., 1.),
            mliVec_set(1., 0., 0.));
        mliRay ray2;
        mliRay ray3;
        mliHomTraComp Tcomp;
        mliHomTra T;
        Tcomp.trans = mliVec_set(0., 1., 0.);
        Tcomp.rot = mliQuaternion_set_rotaxis_and_angle(
            mliVec_set(0., 0., 1.),
            mli_deg2rad(90));
        T = mliHomTra_from_compact(Tcomp);
        ray2 = mliHomTra_ray(&T, ray);
        CHECK_MARGIN(ray2.support.x, 0., 1e-6);
        CHECK_MARGIN(ray2.support.y, 1., 1e-6);
        CHECK_MARGIN(ray2.support.z, 1., 1e-6);
        CHECK_MARGIN(ray2.direction.x, 0., 1e-6);
        CHECK_MARGIN(ray2.direction.y, 1., 1e-6);
        CHECK_MARGIN(ray2.direction.z, 0., 1e-6);

        /* and back again */
        ray3.support = mliHomTra_pos_inverse(&T, ray2.support);
        ray3.direction = mliHomTra_dir_inverse(&T, ray2.direction);
        CHECK_MARGIN(ray3.support.x,   ray.support.x, 1e-6);
        CHECK_MARGIN(ray3.support.y,   ray.support.y, 1e-6);
        CHECK_MARGIN(ray3.support.z,   ray.support.z, 1e-6);
        CHECK_MARGIN(ray3.direction.x, ray.direction.x, 1e-6);
        CHECK_MARGIN(ray3.direction.y, ray.direction.y, 1e-6);
        CHECK_MARGIN(ray3.direction.z, ray.direction.z, 1e-6);
    }

    {
        double solution;
        CHECK(
            mli_xyplane_equation(
                mliRay_set(
                    mliVec_set(0., 0., 1.),
                    mliVec_set(0., 0., -1.)),
                &solution));
        CHECK_MARGIN(solution, 1., 1e-6);

        CHECK(
            mli_xyplane_equation(
                mliRay_set(
                    mliVec_set(0., 0., 1.),
                    mliVec_set(0., 1., -1.)),
                &solution));
        CHECK_MARGIN(solution, sqrt(2.), 1e-6);

        CHECK(
            !mli_xyplane_equation(
                mliRay_set(
                    mliVec_set(0., 0., 1.),
                    mliVec_set(0., 1., 0.)),
                &solution));
    }

    /* dual spherical prism */
    {
        CHECK(mli_is_inside_dual_circle_prism(0., 0., 1., 2.));

        CHECK(mli_is_inside_dual_circle_prism(0., 0.9, 1., 2.));
        CHECK(!mli_is_inside_dual_circle_prism(0., 1.1, 1., 2.));

        CHECK(mli_is_inside_dual_circle_prism(0., -0.9, 1., 2.));
        CHECK(!mli_is_inside_dual_circle_prism(0., -1.1, 1., 2.));

        CHECK(!mli_is_inside_dual_circle_prism(-.51, 0., 1., 2.));
        CHECK(mli_is_inside_dual_circle_prism(-.49, 0., 1., 2.));

        CHECK(!mli_is_inside_dual_circle_prism(.51, 0., 1., 2.));
        CHECK(mli_is_inside_dual_circle_prism(.49, 0., 1., 2.));

        /* special case -> circle, height == width */
        CHECK(mli_is_inside_dual_circle_prism(0., 0.49, 1., 1.));
        CHECK(!mli_is_inside_dual_circle_prism(0., 0.51, 1., 1.));

        CHECK(mli_is_inside_dual_circle_prism(0., -0.49, 1., 1.));
        CHECK(!mli_is_inside_dual_circle_prism(0., -.51, 1., 1.));

        CHECK(!mli_is_inside_dual_circle_prism(-.51, 0., 1., 1.));
        CHECK(mli_is_inside_dual_circle_prism(-.49, 0., 1., 1.));

        CHECK(!mli_is_inside_dual_circle_prism(.51, 0., 1., 1.));
        CHECK(mli_is_inside_dual_circle_prism(.49, 0., 1., 1.));
    }

    /* string to int */
    {
        int64_t i;
        /* Lazy to calculate this size properly. */
        char s[256];

        /* Simple case. */
        sprintf(s, "11");
        CHECK(mli_string_to_int(&i, s, 10));
        CHECK(i == 11);

        /* Negative number . */
        sprintf(s, "-11");
        CHECK(mli_string_to_int(&i, s, 10));
        CHECK(i == -11);

        /* Different base. */
        sprintf(s, "11");
        CHECK(mli_string_to_int(&i, s, 16));
        CHECK(i == 17);

        /* 0 */
        sprintf(s, "0");
        CHECK(mli_string_to_int(&i, s, 10));
        CHECK(i == 0);

        /* INT_MAX. */
        sprintf(s, "%d", INT_MAX);
        CHECK(mli_string_to_int(&i, s, 10));
        CHECK(i == INT_MAX);

        /* INT_MIN. */
        sprintf(s, "%d", INT_MIN);
        CHECK(mli_string_to_int(&i, s, 10));
        CHECK(i == INT_MIN);

        /* Leading and trailing space. */
        sprintf(s, " 1");
        CHECK(!mli_string_to_int(&i, s, 10));
        sprintf(s, "1 ");
        CHECK(!mli_string_to_int(&i, s, 10));

        /* Trash characters. */
        sprintf(s, "a10 ");
        CHECK(!mli_string_to_int(&i, s, 10));
        sprintf(s, "10a ");
        CHECK(!mli_string_to_int(&i, s, 10));

        /* long overflow */
        sprintf(s, "%ld0", LONG_MAX);
        CHECK(!mli_string_to_int(&i, s, 10));

        /* long underflow */
        sprintf(s, "%ld0", LONG_MIN);
        CHECK(!mli_string_to_int(&i, s, 10));

    }
    /* string to float */
    {
        double i;
        /* Lazy to calculate this size properly. */
        char s[256];

        /* Simple case. */
        sprintf(s, "11.");
        CHECK(mli_string_to_float(&i, s));
        CHECK(i == 11.);

        /* Negative number . */
        sprintf(s, "-11.");
        CHECK(mli_string_to_float(&i, s));
        CHECK(i == -11);

        sprintf(s, ".11");
        CHECK(mli_string_to_float(&i, s));
        CHECK(i == .11);

        /* 0 */
        sprintf(s, "0");
        CHECK(mli_string_to_float(&i, s));
        CHECK(i == 0);
        sprintf(s, "0.");
        CHECK(mli_string_to_float(&i, s));
        CHECK(i == 0);
        sprintf(s, ".0");
        CHECK(mli_string_to_float(&i, s));
        CHECK(i == 0);

        /* Leading and trailing space. */
        sprintf(s, " 1");
        CHECK(!mli_string_to_float(&i, s));
        sprintf(s, "1 ");
        CHECK(!mli_string_to_float(&i, s));

        /* Trash characters. */
        sprintf(s, "a10");
        CHECK(!mli_string_to_float(&i, s));
        sprintf(s, "10a");
        CHECK(!mli_string_to_float(&i, s));

        /* long overflow */
        sprintf(s, "9.9e9999");
        CHECK(!mli_string_to_float(&i, s));

        /* long underflow */
        sprintf(s, "-9.9e9999");
        CHECK(!mli_string_to_float(&i, s));

    }

    /* mli_string_ends_with */
    {
        CHECK(mli_string_ends_with("123", ""));
        CHECK(mli_string_ends_with("", ""));
        CHECK(!mli_string_ends_with("", "123"));
        CHECK(mli_string_ends_with("123", "123"));
        CHECK(mli_string_ends_with("my_file.json", ".json"));
        CHECK(!mli_string_ends_with("my_file.json.stuff", ".json"));
    }

    /* json parsing */
    {
        char s[] = "{\"hans\": 84, \"cosmo\": [1, 3, 7]}\0";
        char *end;
        int64_t num_tokens;
        jsmn_parser p;
        jsmntok_t t[128]; /* We expect no more than 128 JSON tokens */

        jsmn_init(&p);
        num_tokens = jsmn_parse(&p, s, strlen(s), t, 128);

        CHECK(num_tokens == 8);
        CHECK(t[0].type == JSMN_OBJECT);
        CHECK(t[0].size == 2);

        CHECK(t[1].type == JSMN_STRING);
        CHECK(t[1].size == 1);

        CHECK(t[2].type == JSMN_PRIMITIVE);
        CHECK(t[2].size == 0);

        CHECK(t[3].type == JSMN_STRING);
        CHECK(t[3].size == 1);

        CHECK(t[4].type == JSMN_ARRAY);
        CHECK(t[4].size == 3);

        CHECK(t[5].type == JSMN_PRIMITIVE);
        CHECK(t[5].size == 0);
        CHECK(strtol(s + t[5].start, &end, 10) == 1);
        CHECK(end == s + t[5].end);

        CHECK(t[6].type == JSMN_PRIMITIVE);
        CHECK(t[6].size == 0);
        CHECK(strtol(s + t[6].start, &end, 10) == 3);
        CHECK(end == s + t[6].end);

        CHECK(t[7].type == JSMN_PRIMITIVE);
        CHECK(t[7].size == 0);
        CHECK(strtol(s + t[7].start, &end, 10) == 7);
        CHECK(end == s + t[7].end);
    }

    /* mliJson */
    {
        mliJson json = mliJson_init();
        CHECK(json.num_chars == 0u);
        CHECK(json.chars == NULL);
        CHECK(json.num_tokens == 0u);
        CHECK(json.tokens == NULL);
    }

    {
        mliJson json = mliJson_init();
        json.num_chars = 10;
        json.num_tokens = 5;
        CHECK(mliJson_malloc(&json));
        mliJson_free(&json);
    }

    {
        mliJson json = mliJson_init();
        uint64_t return_idx;
        int64_t myint;
        double myfloat;

        CHECK(mliJson_malloc_from_file(&json, "example.json"));
        CHECK(mliJson_write_debug(&json, "example.json.debug.tmp"));

        CHECK(mliJson_find_key(&json, 0, "name", &return_idx));
        CHECK(return_idx == 1);
        CHECK(mliJson_find_key(&json, 0, "skill", &return_idx));
        CHECK(return_idx == 3);
        CHECK(mliJson_as_float64(&json, 3 + 1, &myfloat));
        CHECK(myfloat == 1.337);
        CHECK(mliJson_find_key(&json, 0, "bug_rate", &return_idx));
        CHECK(return_idx == 5);
        CHECK(mliJson_as_int64(&json, 5 + 1, &myint));
        CHECK(myint == 42);

        CHECK(mliJson_find_key(&json, 0, "some_numbers", &return_idx));
        CHECK(return_idx == 7);
        CHECK(mliJson_find_key(&json, 0, "fair", &return_idx));
        CHECK(return_idx == 13);

        CHECK(mliJson_find_key(&json, 14, "trade", &return_idx));
        CHECK(return_idx == 15);

        CHECK(mliJson_find_key(&json, 14, "uff", &return_idx));
        CHECK(return_idx == 17);

        CHECK(mliJson_find_key(&json, 0, "not_exist", &return_idx) == 0);
        CHECK(mliJson_find_key(&json, 14, "not_exist", &return_idx) == 0);
        mliJson_free(&json);
    }

    /* mliJson */
    /* from string */
    {
        mliJson json = mliJson_init();
        char json_str[1024];
        uint64_t token = 0u;
        int64_t value = 0;
        sprintf(json_str, "{\"key\": 1337}");
        CHECK(mliJson_malloc_from_string(&json, json_str));
        CHECK(mliJson_find_key(&json, 0, "key", &token));
        CHECK(mliJson_as_int64(&json, token + 1, &value));
        CHECK(value == 1337);
        mliJson_free(&json);
    }

    /* json malloc mliFunc */
    {
        uint64_t token;
        mliJson json = mliJson_init();
        mliFunc f = mliFunc_init();
        CHECK(mliJson_malloc_from_file(&json, "test_mliFunc.json"));
        CHECK(mliJson_write_debug(&json, "test_mliFunc.json.debug.tmp"));
        CHECK(mliJson_find_key(&json, 0, "two_functions", &token));
        CHECK(json.tokens[token + 1].size == 2);
        token += 1;
        CHECK(token == 2);
        token += 1;
        CHECK(token == 3);
        /* the first function */
        CHECK(mliFunc_malloc_from_json_token(&f, &json, token));
        CHECK(f.num_points == 2);
        CHECK_MARGIN(f.x[0], 200e-9, 1e-9);
        CHECK_MARGIN(f.x[1], 1200e-9, 1e-9);
        CHECK_MARGIN(f.y[0], 0., 1e-9);
        CHECK_MARGIN(f.y[1], 0., 1e-9);
        mliFunc_free(&f);

        token = 10;
        /* the second function */
        f = mliFunc_init();
        CHECK(mliFunc_malloc_from_json_token(&f, &json, token));
        CHECK(f.num_points == 3);
        CHECK_MARGIN(f.x[0], -200e-9, 1e-9);
        CHECK_MARGIN(f.x[1], 600e-9, 1e-9);
        CHECK_MARGIN(f.x[2], 1200e-9, 1e-9);
        CHECK_MARGIN(f.y[0], 1.49, 1e-6);
        CHECK_MARGIN(f.y[1], -0.59, 1e-6);
        CHECK_MARGIN(f.y[2], -7.9, 1e-6);
        mliFunc_free(&f);

        mliJson_free(&json);
    }

    /* json parse mliVec and mliColor */
    {
        uint64_t token;
        mliJson json = mliJson_init();
        mliVec vec1 = mliVec_set(0., 0., 0.);
        mliVec vec2 = mliVec_set(0., 0., 0.);
        mliColor col = mliColor_set(0., 0., 0.);
        CHECK(mliJson_malloc_from_file(&json, "test_mliVec.json"));
        CHECK(mliJson_write_debug(&json, "test_mliVec.json.debug.tmp"));

        CHECK(mliJson_find_key(&json, 0, "vec1", &token));
        CHECK(mliVec_from_json_token(&vec1, &json, token + 1));
        CHECK_MARGIN(vec1.x, 1.5, 1e-6);
        CHECK_MARGIN(vec1.y, 2.5, 1e-6);
        CHECK_MARGIN(vec1.z, 3.5, 1e-6);

        CHECK(mliJson_find_key(&json, 0, "vec2", &token));
        CHECK(mliVec_from_json_token(&vec2, &json, token + 1));
        CHECK_MARGIN(vec2.x, 1.2, 1e-6);
        CHECK_MARGIN(vec2.y, 3.4, 1e-6);
        CHECK_MARGIN(vec2.z, -5.6, 1e-6);

        CHECK(mliJson_find_key(&json, 0, "color", &token));
        CHECK(mliColor_from_json_token(&col, &json, token + 1));
        CHECK_MARGIN(col.r, 128., 1e-6);
        CHECK_MARGIN(col.g, 255., 1e-6);
        CHECK_MARGIN(col.b, 12., 1e-6);

        mliJson_free(&json);
    }

    /* mliJson */
    /* rotation representations */
    {
        char json_str[1024];
        mliQuaternion q;
        mliQuaternion q_expected;
        mliJson json = mliJson_init();

        /* unity */
        q_expected = mliQuaternion_set_tait_bryan(0., 0., 0.);

        sprintf(
            json_str,
            "{\"repr\": \"tait_bryan\", \"xyz\": [0, 0, 0]}");
        CHECK(mliJson_malloc_from_string(&json, json_str));
        CHECK(__mliQuaternion_from_json(&q, &json, 0));
        mliJson_free(&json);
        CHECK(mliQuaternion_is_equal_margin(q, q_expected, 1e-6));

        sprintf(
            json_str,
            "{\"repr\": \"axis_angle\", \"axis\": [0, 0, 0], \"angle\": 0.}");
        CHECK(mliJson_malloc_from_string(&json, json_str));
        CHECK(__mliQuaternion_from_json(&q, &json, 0));
        mliJson_free(&json);
        CHECK(mliQuaternion_is_equal_margin(q, q_expected, 1e-6));

        sprintf(
            json_str,
            "{\"repr\": \"quaternion\", \"xyz\": [0, 0, 0]}");
        CHECK(mliJson_malloc_from_string(&json, json_str));
        CHECK(__mliQuaternion_from_json(&q, &json, 0));
        mliJson_free(&json);
        CHECK(mliQuaternion_is_equal_margin(q, q_expected, 1e-6));

        /* z-axis, 45deg */
        q_expected = mliQuaternion_set_tait_bryan(0., 0., -mli_deg2rad(45.));

        sprintf(
            json_str,
            "{\"repr\": \"tait_bryan\", \"xyz\": [0, 0, %f]}",
            -mli_deg2rad(45.));
        CHECK(mliJson_malloc_from_string(&json, json_str));
        CHECK(__mliQuaternion_from_json(&q, &json, 0));
        mliJson_free(&json);
        CHECK(mliQuaternion_is_equal_margin(q, q_expected, 1e-6));

        sprintf(
            json_str,
            "{\"repr\": \"axis_angle\", \"axis\": [0, 0, 1], \"angle\": %f}",
            mli_deg2rad(45.));
        CHECK(mliJson_malloc_from_string(&json, json_str));
        CHECK(__mliQuaternion_from_json(&q, &json, 0));
        mliJson_free(&json);
        CHECK(mliQuaternion_is_equal_margin(q, q_expected, 1e-6));

        sprintf(
            json_str,
            "{\"repr\": \"quaternion\", \"xyz\": [0, 0, %f]}", q_expected.z);
        CHECK(mliJson_malloc_from_string(&json, json_str));
        CHECK(__mliQuaternion_from_json(&q, &json, 0));
        mliJson_free(&json);
        CHECK(mliQuaternion_is_equal_margin(q, q_expected, 1e-6));
    }

    {
        mliJson json = mliJson_init();
        mliUserScenery uscn = mliUserScenery_init();
        mliFrame* f;
        CHECK(mliJson_malloc_from_file(&json, "small_scenery.json"));
        CHECK(mliJson_write_debug(&json, "small_scenery.json.debug.tmp"));

        CHECK(mliUserScenery_malloc_from_json(&uscn, &json));

        /* functions */
        CHECK(uscn.surface_resources.num_functions == 2u);
        CHECK(uscn.surface_resources.functions[0].num_points == 2u);
        CHECK_MARGIN(uscn.surface_resources.functions[0].x[0], 200e-9, 1e-9);
        CHECK_MARGIN(uscn.surface_resources.functions[0].y[0], 0., 1e-9);
        CHECK_MARGIN(uscn.surface_resources.functions[0].x[1], 1200e-9, 1e-9);
        CHECK_MARGIN(uscn.surface_resources.functions[0].y[1], 0., 1e-9);
        CHECK(uscn.surface_resources.functions[1].num_points == 2u);
        CHECK_MARGIN(uscn.surface_resources.functions[1].x[0], 200e-9, 1e-9);
        CHECK_MARGIN(uscn.surface_resources.functions[1].y[0], 1.49, 1e-9);
        CHECK_MARGIN(uscn.surface_resources.functions[1].x[1], 1200e-9, 1e-9);
        CHECK_MARGIN(uscn.surface_resources.functions[1].y[1], 1.49, 1e-9);

        /* colors */
        CHECK(uscn.surface_resources.num_colors == 4u);
        CHECK(mliColor_is_equal(
            uscn.surface_resources.colors[0],
            mliColor_set(22, 91, 49)));
        CHECK(mliColor_is_equal(
            uscn.surface_resources.colors[1],
            mliColor_set(122, 91, 49)));
        CHECK(mliColor_is_equal(
            uscn.surface_resources.colors[2],
            mliColor_set(22, 191, 49)));
        CHECK(mliColor_is_equal(
            uscn.surface_resources.colors[3],
            mliColor_set(22, 91, 149)));

        /* surfaces */
        CHECK(uscn.surface_resources.num_surfaces == 4u);
        CHECK(uscn.surface_resources.surfaces[0].color == 0);
        CHECK(uscn.surface_resources.surfaces[0].reflection == 0);
        CHECK(uscn.surface_resources.surfaces[0].refraction == 0);
        CHECK(uscn.surface_resources.surfaces[1].color == 1);
        CHECK(uscn.surface_resources.surfaces[1].reflection == 0);
        CHECK(uscn.surface_resources.surfaces[1].refraction == 0);
        CHECK(uscn.surface_resources.surfaces[2].color == 2);
        CHECK(uscn.surface_resources.surfaces[2].reflection == 0);
        CHECK(uscn.surface_resources.surfaces[2].refraction == 0);
        CHECK(uscn.surface_resources.surfaces[3].color == 3);
        CHECK(uscn.surface_resources.surfaces[3].reflection == 0);
        CHECK(uscn.surface_resources.surfaces[3].refraction == 2);

        /* frames */
        CHECK(uscn.root.children.size == 4u);
        f = mliFrame_child(&uscn.root, 0);
        CHECK(f->type == MLI_DISC);
        f = mliFrame_child(&uscn.root, 1);
        CHECK(f->type == MLI_FRAME);
        f = mliFrame_child(&uscn.root, 2);
        CHECK(f->type == MLI_MESH);
        CHECK(f->primitive.mesh->num_vertices == 4u);
        CHECK_MARGIN(f->primitive.mesh->vertices[0].x, 0., 1e-6);
        CHECK_MARGIN(f->primitive.mesh->vertices[0].y, 0., 1e-6);
        CHECK_MARGIN(f->primitive.mesh->vertices[0].z, 0., 1e-6);
        CHECK_MARGIN(f->primitive.mesh->vertices[1].x, 1., 1e-6);
        CHECK_MARGIN(f->primitive.mesh->vertices[1].y, 0., 1e-6);
        CHECK_MARGIN(f->primitive.mesh->vertices[1].z, 0., 1e-6);

        CHECK(f->primitive.mesh->num_faces == 4u);
        CHECK(f->primitive.mesh->faces[0].a == 0);
        CHECK(f->primitive.mesh->faces[0].b == 1);
        CHECK(f->primitive.mesh->faces[0].c == 2);
        CHECK(f->primitive.mesh->faces[1].a == 0);
        CHECK(f->primitive.mesh->faces[1].b == 1);
        CHECK(f->primitive.mesh->faces[1].c == 3);

        f = mliFrame_child(&uscn.root, 3);
        CHECK(f->type == MLI_SPHERE);

        mliUserScenery_free(&uscn);
        mliJson_free(&json);
    }

    printf("__SUCCESS__\n");
    return EXIT_SUCCESS;
}
