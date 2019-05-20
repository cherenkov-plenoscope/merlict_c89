/* Copyright 2019 Sebastian Achim Mueller                                     */
/* Compile with:                                                              */
/* gcc merlict-c89-test.c -o merlict-c89-test -std=c89 -lm -Wall -pedantic    */
#include <math.h>
#include <assert.h>
#include "merlict_c89.h"


int main(int argc, char *argv[]) {
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
        mliScenery scenery;
        mliScenery scenery_back;
        mliMesh diff_cube_sphere;
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

        mliMesh_init_from_off("diff_cube_sphere.off", &diff_cube_sphere);

        scenery.num_vertices = diff_cube_sphere.num_vertices;
        scenery.num_triangles = diff_cube_sphere.num_faces;

        mliScenery_malloc(&scenery);

        mliFunc_malloc(&scenery.functions[0] , 2u);
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
        scenery.spheres[0] = 2.5;
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

        mliScenery_write_to_path(&scenery, "my_scenery.mli.tmp");
        mliScenery_read_from_path(&scenery_back, "my_scenery.mli.tmp");

        CHECK(mliScenery_is_equal(&scenery, &scenery_back));

        mliScenery_free(&scenery);
        mliScenery_free(&scenery_back);
    }

    /* render image */
    {
        mliScenery scenery;
        mliOcTree octree;
        mliCamera camera;
        mliImage img;
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

        mliImage_init(&img, 640u, 480u);
        mliCamera_render_image(&camera, &scenery, &octree, &img);
        mliImage_write_to_ppm(&img, "my_image.ppm.tmp");

        mliImage_free(&img);
        mliOcTree_free(&octree);
        mliScenery_free(&scenery);
    }

    /* scenery indexes are valid */
    {
        mliScenery scenery;
        mliScenery_read_from_path(&scenery, "my_scenery.mli.tmp");
        CHECK(mliScenery_valid(&scenery));     /* <--- to be tested */
        mliScenery_free(&scenery);
    }

    /* scenery indexes are valid */
    {
        mliOBB obb;
        uint64_t i;
        uint64_t num_surface_entities;
        mliScenery scenery;
        mliScenery_read_from_path(&scenery, "my_scenery.mli.tmp");
        num_surface_entities = scenery.num_triangles;
        num_surface_entities += scenery.num_spherical_cap_hex;
        num_surface_entities += scenery.num_spheres;
        num_surface_entities += scenery.num_cylinders;
        num_surface_entities += scenery.num_hexagons;
        num_surface_entities += scenery.num_bicircleplanes;

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
        mliScenery scenery;
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
        mliScenery scenery;
        CHECK(!mliScenery_read_from_path(&scenery, "does_not_exist.tmp"));
    }


    /* scenery intersection interface -> sphere */
    {
        mliScenery scenery;
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
        mliScenery scenery;
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
        mliScenery scenery;
        mliVec offset;
        uint64_t i;
        scenery.num_functions = 1u;
        scenery.num_colors = 1u;
        scenery.num_surfaces = 1u;
        scenery.num_spherical_cap_hex = 0u;
        scenery.num_spheres = 50u;
        scenery.num_vertices = 0u;
        scenery.num_triangles = 0u;

        mliScenery_malloc(&scenery);

        mliFunc_malloc(&scenery.functions[0] , 2u);
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
            scenery.spheres[i] = 0.1;
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
        mliScenery scenery;
        mliOcTree octree;
        mliCamera camera;
        mliImage img;
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

        mliImage_init(&img, 640u, 480u);
        mliCamera_render_image(&camera, &scenery, &octree, &img);
        mliImage_write_to_ppm(&img, "asymetric_image.ppm.tmp");

        mliImage_free(&img);
        mliOcTree_free(&octree);
        mliScenery_free(&scenery);
    }

    /* mliVector */
    {
        uint64_t i;
        mliVector vec;
        mliVector_set(&vec, 0u, sizeof(mliVec));
        CHECK(vec.size == 0u);

        for (i = 0; i < 100; i++) {
            mliVec hans = {0., 1., 2.};
            CHECK(vec.size == i);
            mliVector_push_back(&vec, &hans);
            CHECK(vec.size == i + 1);
        }

        for (i = 0; i < vec.size; i++) {
            mliVec hans = *(mliVec*)mliVector_get(&vec, i);
            CHECK_MARGIN(hans.x, 0., 1e-9);
            CHECK_MARGIN(hans.y, 1., 1e-9);
            CHECK_MARGIN(hans.z, 2., 1e-9);
            CHECK(vec.size == 100);
        }

        mliVector_free(&vec);
        CHECK(vec.size == 0);
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
        mliImage img;
        mliImage_init(&img, 3, 2);
        CHECK(img.num_cols == 3u);
        CHECK(img.num_rows == 2u);
        mliImage_free(&img);
        CHECK(img.num_cols == 0u);
        CHECK(img.num_rows == 0u);
    }

    {
        mliImage img;
        mliImage back;
        uint32_t col;
        uint32_t row;
        double tone;
        mliImage_init(&img, 3, 2);
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

        mliImage_init_from_ppm(&back, "img.ppm.tmp");
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
        mliFunc func;
        mliFunc_malloc(&func, 0u);
        CHECK(func.num_points == 0u);
        CHECK(mliFunc_x_is_causal(&func));
        mliFunc_free(&func);
    }

    {
        mliFunc func;
        mliFunc_malloc(&func, 3u);
        CHECK(func.num_points == 3u);
        func.x[0] = 0.;
        func.x[1] = 1.;
        func.x[2] = 2.;
        CHECK(mliFunc_x_is_causal(&func));
        mliFunc_free(&func);
    }

    {
        mliFunc func;
        double y;
        mliFunc_malloc(&func, 5u);
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
        CHECK(mliFunc_evaluate(&func, 1.5, &y) == 0);
        CHECK(y == 2.);
        mliFunc_free(&func);
    }

    {
        mliMesh m;
        mliVec support = {0 ,0 ,0};
        mliVec direction = {0 ,0 ,1};
        uint64_t i;
        mliMesh_init_from_off("text_on_cube.off", &m);
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
        mliMesh m;
        mliMesh_init_from_off("diff_cube_sphere.off", &m);
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
        const double radius = 1.;
        mliIntersection intersection;
        mliHomTraComp local2root_comp;
        local2root_comp.trans = mliVec_set(0., 0., 0.);
        local2root_comp.rot = mliQuaternion_set_rotaxis_and_angle(
            mliVec_set(0., 0., 0.),
            0.);

        CHECK(!mliSphere_intersection(
            radius,
            local2root_comp,
            mliRay_set(mliVec_set(0., 0., 5.), mliVec_set(0., 0., 1.)),
            &intersection));

        CHECK(mliSphere_intersection(
            radius,
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
        CHECK_MARGIN(obb.lower.x, -1., 1e-6);
        CHECK_MARGIN(obb.upper.x, +1., 1e-6);
        CHECK_MARGIN(obb.lower.y, -MLI_SQRT3_OVER_2, 1e-6);
        CHECK_MARGIN(obb.upper.y, +MLI_SQRT3_OVER_2, 1e-6);
        CHECK_MARGIN(obb.lower.z, 0., 1e-6);
        CHECK_MARGIN(obb.upper.z, 0., 1e-6);

        /* translation */
        local2root_comp.trans = mliVec_set(1., 2., 3.);
        local2root_comp.rot = mliQuaternion_set_rotaxis_and_angle(
            mliVec_set(0., 0., 1.),
            mli_deg2rad(0.));
        obb = mliHexagon_obb(hex, local2root_comp);
        CHECK_MARGIN(obb.lower.x, -1. + 1., 1e-6);
        CHECK_MARGIN(obb.upper.x, +1. + 1., 1e-6);
        CHECK_MARGIN(obb.lower.y, -MLI_SQRT3_OVER_2 + 2., 1e-6);
        CHECK_MARGIN(obb.upper.y, +MLI_SQRT3_OVER_2 + 2., 1e-6);
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
        CHECK_MARGIN(obb.lower.y, -MLI_SQRT3_OVER_2, 1e-6);
        CHECK_MARGIN(obb.upper.y, +MLI_SQRT3_OVER_2, 1e-6);
        CHECK_MARGIN(obb.lower.z, -1., 1e-6);
        CHECK_MARGIN(obb.upper.z, +1., 1e-6);

        /* rotation y-axis 45deg */
        local2root_comp.trans = mliVec_set(0., 0., 0.);
        local2root_comp.rot = mliQuaternion_set_rotaxis_and_angle(
            mliVec_set(0., 1., 0.),
            mli_deg2rad(45.));
        obb = mliHexagon_obb(hex, local2root_comp);
        CHECK_MARGIN(obb.lower.x, -sqrt(2)/2, 1e-6);
        CHECK_MARGIN(obb.upper.x, +sqrt(2)/2, 1e-6);
        CHECK_MARGIN(obb.lower.y, -MLI_SQRT3_OVER_2, 1e-6);
        CHECK_MARGIN(obb.upper.y, +MLI_SQRT3_OVER_2, 1e-6);
        CHECK_MARGIN(obb.lower.z, -sqrt(2)/2, 1e-6);
        CHECK_MARGIN(obb.upper.z, +sqrt(2)/2., 1e-6);
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
    return EXIT_SUCCESS;
}
