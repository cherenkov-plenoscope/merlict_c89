/* Copyright 2019-2020 Sebastian Achim Mueller                                */

CASE("mliHexagonalPrismZ") {
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
    struct mliVec c;
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

CASE("hexagon corners") {
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
        struct mliVec corner =  mli_hexagon_corner(i);
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
