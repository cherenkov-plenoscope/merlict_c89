/* Copyright 2019 Sebastian Achim Mueller                                     */
/* Compile with: gcc test.c -o test -std=c89 -lm -Wall -pedantic              */
#include <math.h>
#include <assert.h>
#include "mliMath.h"
#include "mliVec.h"
#include "mliRay.h"
#include "mliUnitTest.h"
#include "mliMesh.h"
#include "mliFunc.h"
#include "mliTriangleIntersection.h"
#include "mliColor.h"
#include "mliImage.h"
#include "mliQuadraticEquation.h"
#include "mliHomTra.h"
#include "mliCamera.h"
#include "mliScenery.h"
#include "mliTracer.h"
#include "mliHexagonalPrismZ.h"
#include "mliSphericalCap.h"
#include "mliOuterPrismBound.h"
#include "mliVector.h"


int main(int argc, char *argv[]) {
    /* assert */
    {
        assert(1);
        /* printf("%u\n", UINT32_MAX); */
    }
    /* mliScenery */
    {
        int i;
        mliScenery scenery;
        mliScenery scenery_back;
        mliMesh diff_cube_sphere;
        mliColor red = {255., 0., 0.};
        scenery.num_functions = 1u;
        scenery.num_colors = 1u;
        scenery.num_surfaces = 1u;
        scenery.num_spherical_cap_hex = 1u;

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

        scenery.surfaces[0].color = 0u;
        scenery.surfaces[0].reflection = 0u;
        scenery.surfaces[0].refraction = 0u;
        scenery.surfaces[0].absorbtion = 0u;

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

        scenery.spherical_cap_hex[0].curvature_radius = 4.89*2.;
        scenery.spherical_cap_hex[0].inner_hex_radius = 0.32;

        scenery.spherical_cap_hex_surfaces[0].outer = 0u;
        scenery.spherical_cap_hex_surfaces[0].inner = 0u;

        mliScenery_write_to_path(&scenery, "my_scenery.mli.tmp");
        mliScenery_read_from_path(&scenery_back, "my_scenery.mli.tmp");

        CHECK(mliScenery_is_equal(&scenery, &scenery_back));

        mliScenery_free(&scenery);
    }

    /* render image */
    {
        mliScenery scenery;
        mliCamera camera;
        mliImage img;
        mliScenery_read_from_path(&scenery, "my_scenery.mli.tmp");

        camera.position.x = 0.;
        camera.position.y = 0.;
        camera.position.z = -20.;
        camera.rotation = mliRotMat_init_tait_bryan(0., 0., 0.);
        camera.field_of_view = mli_deg2rad(80.);

        mliImage_init(&img, 64u, 64u);
        mliCamera_render_image(&camera, &scenery, &img);
        mliImage_write_to_ppm(&img, "my_image.ppm.tmp");

        mliImage_free(&img);
        mliScenery_free(&scenery);
    }

    /* render image */
    {
        mliScenery scenery;
        mliScenery_read_from_path(&scenery, "my_scenery.mli.tmp");
        CHECK(mliScenery_valid(&scenery));     /* <--- to be tested */
        mliScenery_free(&scenery);
    }

    /* mliVector */
    {
        int i;
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
        rot = mliRotMat_init_axis(&axis, 0.);
        CHECK_MARGIN(rot.r00, 1., 1e-9);
        CHECK_MARGIN(rot.r01, 0., 1e-9);
        CHECK_MARGIN(rot.r02, 0., 1e-9);

        CHECK_MARGIN(rot.r10, 0., 1e-9);
        CHECK_MARGIN(rot.r11, 1., 1e-9);
        CHECK_MARGIN(rot.r12, 0., 1e-9);

        CHECK_MARGIN(rot.r20, 0., 1e-9);
        CHECK_MARGIN(rot.r21, 0., 1e-9);
        CHECK_MARGIN(rot.r22, 1., 1e-9);

        rot = mliRotMat_init_axis(&axis, mli_deg2rad(90.));
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
        a_rot = mli_transform_orientation(&rot, &a);
        CHECK_MARGIN(a_rot.x, 0., 1e-9);
        CHECK_MARGIN(a_rot.y, 0., 1e-9);
        CHECK_MARGIN(a_rot.z, 1., 1e-9);
    }

    {
        mliRotMat rot;
        mliVec x = {1., 0., 0.};
        mliVec x_rot;
        rot = mliRotMat_init_tait_bryan(0., 0., mli_deg2rad(45.));
        x_rot = mli_transform_orientation(&rot, &x);
        CHECK_MARGIN(x_rot.x, 1./sqrt(2.), 1e-6);
        CHECK_MARGIN(x_rot.y, -1./sqrt(2.), 1e-6);
        CHECK_MARGIN(x_rot.z, 0., 1e-9);

        x_rot = mli_transform_orientation_inverse(&rot, &x);
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
        x_t = mli_transform_position(&rot, &trans, &x);
        CHECK_MARGIN(x_t.x, 1., 1e-6);
        CHECK_MARGIN(x_t.y, 0., 1e-6);
        CHECK_MARGIN(x_t.z, 3., 1e-9);

        x_t = mli_transform_position_inverse(&rot, &trans, &x);
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
        CHECK_MARGIN(mli_deg2rad(90.), mli_PI/2, 1e-9);
        CHECK_MARGIN(mli_deg2rad(180.), mli_PI, 1e-9);
        CHECK_MARGIN(mli_deg2rad(-90.), -mli_PI/2.0, 1e-9);
        CHECK_MARGIN(mli_deg2rad(-180.), -mli_PI, 1e-9);
        CHECK_MARGIN(mli_deg2rad(360.), 2.*mli_PI, 1e-9);
    }

    {
        CHECK_MARGIN(mli_rad2deg(0.*mli_PI), 0., 1e-9);
        CHECK_MARGIN(mli_rad2deg(.25*mli_PI), 45., 1e-9);
        CHECK_MARGIN(mli_rad2deg(.5*mli_PI), 90., 1e-9);
        CHECK_MARGIN(mli_rad2deg(1.*mli_PI), 180., 1e-9);
        CHECK_MARGIN(mli_rad2deg(1.e3*mli_PI), 180.e3, 1e-9);
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
        mliColor mix = mliColor_mix(&red, &blue, 0.2);
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
        float tone;
        mliImage_init(&img, 3, 2);
        CHECK(img.num_cols == 3u);
        CHECK(img.num_rows == 2u);
        tone = 0.;
        for (col = 0; col < img.num_cols; col++) {
            for (row = 0; row < img.num_rows; row++) {
                mliColor color;
                tone = (float)col * (float)row;
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
        mliVec out = mliVec_add(&a, &b);
        CHECK_MARGIN(out.x, 5.0, 1e-6);
        CHECK_MARGIN(out.y, 7.0, 1e-6);
        CHECK_MARGIN(out.z, 9.0, 1e-6);
    }

    {
        mliVec a = {1., 2., 3.};
        mliVec b = {4., 6., 8.};
        mliVec out = mliVec_substract(&a, &b);
        CHECK_MARGIN(out.x, -3.0, 1e-6);
        CHECK_MARGIN(out.y, -4.0, 1e-6);
        CHECK_MARGIN(out.z, -5.0, 1e-6);
    }

    {
        mliVec a = {1., 2., 3.};
        mliVec b = {4., 5., 6.};
        CHECK_MARGIN(mliVec_dot(&a, &b), 32.0, 1e-6);
    }

    {
        mliVec a = {0., 0., 1.};
        mliVec b = {0., 1., 0.};
        CHECK_MARGIN(mliVec_dot(&a, &b), 0., 1e-6);
    }

    {
        mliVec a = {0., 0., 1.};
        mliVec b = {1., 0., 0.};
        CHECK_MARGIN(mliVec_dot(&a, &b), 0., 1e-6);
    }

    {
        mliVec a = {0., 0., 1.};
        mliVec b = {0., 0., 1.};
        CHECK_MARGIN(mliVec_dot(&a, &b), 1., 1e-6);
    }

    {
        mliVec a = {1., 0., 0.};
        mliVec b = {0., 1., 0.};
        mliVec out = mliVec_cross(&a, &b);
        CHECK_MARGIN(out.x, 0., 1e-6);
        CHECK_MARGIN(out.y, 0., 1e-6);
        CHECK_MARGIN(out.z, 1., 1e-6);
    }

    {
        mliVec a = {1., 0., 0.};
        mliVec b = {0., 0., 1.};
        mliVec out = mliVec_cross(&a, &b);
        CHECK_MARGIN(out.x, 0., 1e-6);
        CHECK_MARGIN(out.y, -1., 1e-6);
        CHECK_MARGIN(out.z, 0., 1e-6);
    }

    {
        mliVec a = {1., 0., 0.};
        CHECK_MARGIN(mliVec_norm(&a), 1., 1e-6);
    }

    {
        mliVec a = {3., 4., 0.};
        CHECK_MARGIN(mliVec_norm(&a), 5., 1e-6);
    }

    {
        mliVec a = {0., 0., -1.};
        CHECK_MARGIN(mliVec_norm(&a), 1., 1e-6);
    }

    {
        mliVec in = {0., 0., -1.};
        mliVec surface_normal = {0., 0., 1.};
        mliVec out = mliVec_mirror(&in, &surface_normal);
        CHECK_MARGIN(out.x, 0., 1e-6);
        CHECK_MARGIN(out.y, 0., 1e-6);
        CHECK_MARGIN(out.z, 1., 1e-6);
    }

    {
        mliVec in = {1., 0., -1.};
        mliVec surface_normal = {0., 0., 1.};
        mliVec out = mliVec_mirror(&in, &surface_normal);
        CHECK_MARGIN(out.x, 1., 1e-6);
        CHECK_MARGIN(out.y, 0., 1e-6);
        CHECK_MARGIN(out.z, 1., 1e-6);
    }

    {
        mliVec a = {0., 0., 0.};
        mliVec b = {0., 0., 0.};
        CHECK(mliVec_equal_margin(&a, &b, 1e-6));
    }

    {
        mliVec a = {1., 0., 0.};
        mliVec b = {1. + 1e-5, 0., 0.};
        CHECK(!mliVec_equal_margin(&a, &b, 1e-6));
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

        const float p = +1.;
        const float n = -1.;

        a.x = n;
        a.y = n;
        a.z = n;
        CHECK(mliVec_octant(&a) == 0u);

        a.x = n;
        a.y = n;
        a.z = p;
        CHECK(mliVec_octant(&a) == 1u);

        a.x = n;
        a.y = p;
        a.z = n;
        CHECK(mliVec_octant(&a) == 2u);

        a.x = n;
        a.y = p;
        a.z = p;
        CHECK(mliVec_octant(&a) == 3u);

        a.x = p;
        a.y = n;
        a.z = n;
        CHECK(mliVec_octant(&a) == 4u);

        a.x = p;
        a.y = n;
        a.z = p;
        CHECK(mliVec_octant(&a) == 5u);

        a.x = p;
        a.y = p;
        a.z = n;
        CHECK(mliVec_octant(&a) == 6u);

        a.x = p;
        a.y = p;
        a.z = p;
        CHECK(mliVec_octant(&a) == 7u);

        a.x = 0.; a.y = 0.; a.z = p;
        CHECK(mliVec_octant(&a) == 7u);

        a.x = 0.; a.y = p; a.z = 0.;
        CHECK(mliVec_octant(&a) == 7u);

        a.x = p; a.y = 0.; a.z = 0.;
        CHECK(mliVec_octant(&a) == 7u);

        a.x = 0.; a.y = p; a.z = p;
        CHECK(mliVec_octant(&a) == 7u);

        a.x = p; a.y = p; a.z = 0.;
        CHECK(mliVec_octant(&a) == 7u);

        a.x = p; a.y = 0.; a.z = p;
        CHECK(mliVec_octant(&a) == 7u);
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
        mliFunc_malloc(&func, 3u);
        func.x[0] = 0.;
        func.x[1] = 1.;
        func.x[2] = 2.;
        CHECK(mliFunc_x_is_causal(&func));
        CHECK(mliFunc_upper_compare(&func, -1.) == 0);
        CHECK(mliFunc_upper_compare(&func, 3.) == 3);
        mliFunc_free(&func);
    }

    {
        mliFunc func;
        mliFunc_malloc(&func, 0u);
        CHECK(mliFunc_x_is_causal(&func));
        CHECK(mliFunc_upper_compare(&func, -1.) == 1);
        CHECK(mliFunc_upper_compare(&func, 3.) == 1);
        mliFunc_free(&func);
    }

    {
        mliFunc func;
        float y;
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
        CHECK(mliFunc_upper_compare(&func, 1.5) == 2);
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
                    &support,
                    &direction,
                    &m.vertices[m.faces[i].a],
                    &m.vertices[m.faces[i].b],
                    &m.vertices[m.faces[i].c],
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
        mliVec c;
        double inner_radius = 0.5;
        double outer_radius = inner_radius/cos(mli_PI/6.0);
        c.z = 0.;
        for (c.x = -1.; c.x < 1.; c.x = c.x + 0.01) {
            for (c.y = -1.; c.y < 1.; c.y = c.y + 0.01) {
                if (mliVec_norm(&c) <= inner_radius)
                    CHECK(mli_inside_hexagonal_prism_z(&c, inner_radius));
                else if(mliVec_norm(&c) > outer_radius)
                    CHECK(!mli_inside_hexagonal_prism_z(&c, inner_radius));
                else
                    continue;
            }
        }
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
                &ray,
                radius,
                &plus_solution,
                &minus_solution));
        CHECK_MARGIN(plus_solution, 1.0, 1e-9);
        CHECK_MARGIN(minus_solution, (1.0 - 2.*radius), 1e-9);

        intersection_point = mliRay_at(&ray, plus_solution);
        surface_normal = mli_spherical_cap_surface_normal(
            &intersection_point,
            radius);
        CHECK_MARGIN(surface_normal.x, 0., 1e-9);
        CHECK_MARGIN(surface_normal.y, 0., 1e-9);
        CHECK_MARGIN(surface_normal.z, 1., 1e-9);
    }

    {
        double causal_solution;
        const mliVec support = {0., 0., 1.};
        const mliVec direction = {0., 0., -1.};
        const mliRay ray = mliRay_set(support, direction);

        mliBoundSurfaceChecklist cl;
        cl.plus_solution = 1.0;
        cl.minus_solution = 0.0;
        cl.plus_intersec = mliRay_at(&ray, cl.plus_solution);
        cl.minus_intersec = mliRay_at(&ray, cl.minus_solution);
        cl.plus_is_inside = 0;
        cl.minus_is_inside = 1;

        mli_outer_bound_surface_causal_intersection(
            cl,
            &causal_solution);
    }

    return EXIT_SUCCESS;
}
