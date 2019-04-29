/* Copyright 2019 Sebastian Achim Mueller                                     */
/* Compile with: gcc test.c -o test -std=c89 -lm -Wall -pedantic              */
#include <math.h>
#include "mliMath.h"
#include "mliVec.h"
#include "mliUnitTest.h"
#include "mliMesh.h"
#include "mliFunc.h"
#include "mliTriangleIntersection.h"
#include "mliColor.h"
#include "mliImage.h"
#include "mliQuadraticEquation.h"
#include "mliHomTra.h"


int main(int argc, char *argv[]) {
    /* mliHomTra */
    {
        mliRotMat rot;
        mliRotMat_init_tait_bryan(0., 0., 0., &rot);
        CHECK_MARGIN(rot.r00, 1., 1e-9);
        CHECK_MARGIN(rot.r01, 0., 1e-9);
        CHECK_MARGIN(rot.r02, 0., 1e-9);

        CHECK_MARGIN(rot.r10, 0., 1e-9);
        CHECK_MARGIN(rot.r11, 1., 1e-9);
        CHECK_MARGIN(rot.r12, 0., 1e-9);

        CHECK_MARGIN(rot.r20, 0., 1e-9);
        CHECK_MARGIN(rot.r21, 0., 1e-9);
        CHECK_MARGIN(rot.r22, 1., 1e-9);

        mliRotMat_init_tait_bryan(0., 0., mli_deg2rad(90), &rot);
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
        mliRotMat_init_axis(&axis, 0., &rot);
        CHECK_MARGIN(rot.r00, 1., 1e-9);
        CHECK_MARGIN(rot.r01, 0., 1e-9);
        CHECK_MARGIN(rot.r02, 0., 1e-9);

        CHECK_MARGIN(rot.r10, 0., 1e-9);
        CHECK_MARGIN(rot.r11, 1., 1e-9);
        CHECK_MARGIN(rot.r12, 0., 1e-9);

        CHECK_MARGIN(rot.r20, 0., 1e-9);
        CHECK_MARGIN(rot.r21, 0., 1e-9);
        CHECK_MARGIN(rot.r22, 1., 1e-9);

        mliRotMat_init_axis(&axis, mli_deg2rad(90.), &rot);
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
        mliRotMat_init_tait_bryan(0., 0., mli_deg2rad(45.), &rot);
        mli_transform_orientation(&rot, &a, &a_rot);
        CHECK_MARGIN(a_rot.x, 0., 1e-9);
        CHECK_MARGIN(a_rot.y, 0., 1e-9);
        CHECK_MARGIN(a_rot.z, 1., 1e-9);
    }

    {
        mliRotMat rot;
        mliVec x = {1., 0., 0.};
        mliVec x_rot;
        mliRotMat_init_tait_bryan(0., 0., mli_deg2rad(45.), &rot);
        mli_transform_orientation(&rot, &x, &x_rot);
        CHECK_MARGIN(x_rot.x, 1./sqrt(2.), 1e-6);
        CHECK_MARGIN(x_rot.y, -1./sqrt(2.), 1e-6);
        CHECK_MARGIN(x_rot.z, 0., 1e-9);

        mli_transform_orientation_inverse(&rot, &x, &x_rot);
        CHECK_MARGIN(x_rot.x, 1./sqrt(2.), 1e-6);
        CHECK_MARGIN(x_rot.y, 1./sqrt(2.), 1e-6);
        CHECK_MARGIN(x_rot.z, 0., 1e-9);
    }

    {
        mliRotMat rot;
        mliVec trans = {0., 0., 3.};
        mliVec x = {1., 0., 0.};
        mliVec x_t;
        mliRotMat_init_tait_bryan(0., 0., 0., &rot);
        mli_transform_position(&rot, &trans, &x, &x_t);
        CHECK_MARGIN(x_t.x, 1., 1e-6);
        CHECK_MARGIN(x_t.y, 0., 1e-6);
        CHECK_MARGIN(x_t.z, 3., 1e-9);

        mli_transform_position_inverse(&rot, &trans, &x, &x_t);
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
        mliColor mix;
        mliColor_mix(&red, &blue, 0.2, &mix);
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
        mliColor_mean(colors, 3, &mean);
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
        mliVec out;
        mliVec_add(&a, &b, &out);
        CHECK_MARGIN(out.x, 5.0, 1e-6);
        CHECK_MARGIN(out.y, 7.0, 1e-6);
        CHECK_MARGIN(out.z, 9.0, 1e-6);
    }

    {
        mliVec a = {1., 2., 3.};
        mliVec b = {4., 6., 8.};
        mliVec out;
        mliVec_substract(&a, &b, &out);
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
        mliVec out;
        mliVec_cross(&a, &b, &out);
        CHECK_MARGIN(out.x, 0., 1e-6);
        CHECK_MARGIN(out.y, 0., 1e-6);
        CHECK_MARGIN(out.z, 1., 1e-6);
    }

    {
        mliVec a = {1., 0., 0.};
        mliVec b = {0., 0., 1.};
        mliVec out;
        mliVec_cross(&a, &b, &out);
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
        mliVec out;
        mliVec_mirror(&in, &surface_normal, &out);
        CHECK_MARGIN(out.x, 0., 1e-6);
        CHECK_MARGIN(out.y, 0., 1e-6);
        CHECK_MARGIN(out.z, 1., 1e-6);
    }

    {
        mliVec in = {1., 0., -1.};
        mliVec surface_normal = {0., 0., 1.};
        mliVec out;
        mliVec_mirror(&in, &surface_normal, &out);
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
        mliFunc func;
        mliFunc_init(&func, 0u);
        CHECK(func.num_points == 0u);
        CHECK(mliFunc_x_is_causal(&func));
        mliFunc_free(&func);
    }

    {
        mliFunc func;
        mliFunc_init(&func, 3u);
        CHECK(func.num_points == 3u);
        func.x[0] = 0.;
        func.x[1] = 1.;
        func.x[2] = 2.;
        CHECK(mliFunc_x_is_causal(&func));
        mliFunc_free(&func);
    }

    {
        mliFunc func;
        mliFunc_init(&func, 3u);
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
        mliFunc_init(&func, 0u);
        CHECK(mliFunc_x_is_causal(&func));
        CHECK(mliFunc_upper_compare(&func, -1.) == 1);
        CHECK(mliFunc_upper_compare(&func, 3.) == 1);
        mliFunc_free(&func);
    }

    {
        mliFunc func;
        float y;
        mliFunc_init(&func, 5u);
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
        mliMesch_init_from_off("text_on_cube.off", &m);
        for (i = 0; i < m.num_faces; i++) {
            mliVec intersection_position;
            if(
                mliRay_intersects_triangle(
                    &support,
                    &direction,
                    &m.vertices[m.faces[i].a],
                    &m.vertices[m.faces[i].b],
                    &m.vertices[m.faces[i].c],
                    &intersection_position)
            ) {
                CHECK(i == 1782);
                /* printf( "hit in idx %d\n", (int)i); */
            }
        }
        mliMesh_free(&m);
    }

    return EXIT_SUCCESS;
}
