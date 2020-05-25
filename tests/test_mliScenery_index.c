/* Copyright 2019-2020 Sebastian Achim Mueller                                */

CASE("mliScenery object interface, init")
{
        struct mliScenery scenery = mliScenery_init();
        uint64_t num_primitives;
        uint64_t p, p_back;

        scenery.num_triangles = 100;
        scenery.num_spherical_cap_hex = 10;
        scenery.num_spheres = 20;
        scenery.num_cylinders = 30;
        scenery.num_hexagons = 40;
        scenery.num_bicircleplanes = 50;
        scenery.num_discs = 60;

        num_primitives = mliScenery_num_primitives(&scenery);

        CHECK(num_primitives == 100+10+20+30+40+50+60);

        for (p = 0; p < num_primitives; p++) {
                struct mliIndex ridx = _mliScenery_resolve_index(
                        &scenery,
                        p);
                p_back = _mliScenery_merge_index(
                        &scenery,
                        ridx.type,
                        ridx.idx);
                CHECK(p == p_back);
        }
}
