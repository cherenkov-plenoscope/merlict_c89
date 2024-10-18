/* Copyright 2019-2020 Sebastian Achim Mueller                                */

#include "../src/mliColorObserver.h"
#include "../../mli_testing/src/mli_testing.h"

CASE("init ColorObserver")
{
        struct mliColorObserver obs = mliColorObserver_init();
        CHECK(obs.r.num_points == 0);
        CHECK(obs.g.num_points == 0);
        CHECK(obs.b.num_points == 0);
}

CASE("malloc ColorObserver")
{
        struct mliColorObserver obs = mliColorObserver_init();
        CHECK(mliColorObserver_malloc_cie1931(&obs));

        CHECK(obs.r.num_points > 0);
        CHECK(obs.g.num_points > 0);
        CHECK(obs.b.num_points > 0);

        mliColorObserver_free(&obs);

        CHECK(obs.r.num_points == 0);
        CHECK(obs.g.num_points == 0);
        CHECK(obs.b.num_points == 0);
}

CASE("fold ColorObserver")
{
        struct mliColor rgb = mliColor_set(1., 1., 1.);
        struct mliColorObserver obs = mliColorObserver_init();
        struct mliFunc spectrum = mliFunc_init();

        CHECK(mliColorObserver_malloc_cie1931(&obs));

        CHECK(mliFunc_malloc(&spectrum, 6));
        spectrum.x[0] = 200e-9;
        spectrum.y[0] = 0.0;

        spectrum.x[5] = 1200e-9;
        spectrum.y[5] = 0.0;

        /* blueish spectrum*/
        spectrum.x[1] = 380e-9;
        spectrum.y[1] = 0.0;

        spectrum.x[2] = 385e-9;
        spectrum.y[2] = 1.0;

        spectrum.x[3] = 475e-9;
        spectrum.y[3] = 1.0;

        spectrum.x[4] = 480e-9;
        spectrum.y[4] = 0.0;
        CHECK(mliColorObserver_evaluate(&obs, &spectrum, &rgb));
        CHECK_MARGIN(rgb.r, 1.0e-2, 1e-2);
        CHECK_MARGIN(rgb.g, 0.2e-2, 1e-2);
        CHECK_MARGIN(rgb.b, 5.0e-2, 1e-2);

        /* greenish spectrum*/
        spectrum.x[1] = 500e-9;
        spectrum.y[1] = 0.0;

        spectrum.x[2] = 505e-9;
        spectrum.y[2] = 1.0;

        spectrum.x[3] = 545e-9;
        spectrum.y[3] = 1.0;

        spectrum.x[4] = 550e-9;
        spectrum.y[4] = 0.0;
        CHECK(mliColorObserver_evaluate(&obs, &spectrum, &rgb));
        CHECK_MARGIN(rgb.r, 0.4e-2, 1e-3);
        CHECK_MARGIN(rgb.g, 2.0e-2, 1e-3);
        CHECK_MARGIN(rgb.b, 0.2e-2, 1e-3);

        /* reddish spectrum*/
        spectrum.x[1] = 600e-9;
        spectrum.y[1] = 0.0;

        spectrum.x[2] = 605e-9;
        spectrum.y[2] = 1.0;

        spectrum.x[3] = 695e-9;
        spectrum.y[3] = 1.0;

        spectrum.x[4] = 700e-9;
        spectrum.y[4] = 0.0;
        CHECK(mliColorObserver_evaluate(&obs, &spectrum, &rgb));
        CHECK_MARGIN(rgb.r, 2.1e-2, 1e-3);
        CHECK_MARGIN(rgb.g, 1.0e-2, 1e-3);
        CHECK_MARGIN(rgb.b, 0.1e-2, 1e-3);

        mliColorObserver_free(&obs);
        mliFunc_free(&spectrum);
}
