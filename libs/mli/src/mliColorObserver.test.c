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

        CHECK(mliFunc_malloc(&spectrum, 6));
        spectrum.x[0] = 200e-9;
        spectrum.y[0] = 0.0;

        spectrum.x[1] = 380e-9;
        spectrum.y[1] = 0.0;

        spectrum.x[2] = 385e-9;
        spectrum.y[2] = 1.0;

        spectrum.x[3] = 475e-9;
        spectrum.y[3] = 1.0;

        spectrum.x[4] = 480e-9;
        spectrum.y[4] = 0.0;

        spectrum.x[5] = 1200e-9;
        spectrum.y[5] = 0.0;

        CHECK(mliColorObserver_malloc_cie1931(&obs));

        CHECK(mliColorObserver_evaluate(&obs, &spectrum, &rgb));

        printf("color (%f, %f, %f)\n", rgb.r, rgb.b, rgb.b);

        CHECK(0);

        mliColorObserver_free(&obs);
        mliFunc_free(&spectrum);
}
