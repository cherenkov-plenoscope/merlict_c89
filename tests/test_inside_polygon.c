/* Copyright 2019-2020 Sebastian Achim Mueller                                */

CASE("is point in polygon")
{
        uint64_t i, row, col;
        const uint64_t num_rows = 100;
        const uint64_t num_cols = 100;
        double actual_area_hexagon = 0;
        double hexagon_radius = 30;
        double expected_area_hexagon =
                ((3.0 / 2.0) * sqrt(3.0) * hexagon_radius * hexagon_radius);

        struct mliFunc polygon = mliFunc_init();
        CHECK(mliFunc_malloc(&polygon, 6));

        for (i = 0; i < 6; i++) {
                double phi = MLI_2PI * ((double)i / 6);
                polygon.x[i] = hexagon_radius * cos(phi) + 50;
                polygon.y[i] = hexagon_radius * sin(phi) + 50;
        }

        for (row = 0; row < num_rows; row++) {
                for (col = 0; col < num_cols; col++) {
                        if (mli_inside_polygon(&polygon, row, col)) {
                                actual_area_hexagon += 1.0;
                        }
                }
        }

        CHECK(expected_area_hexagon > 2e3);
        CHECK_MARGIN(actual_area_hexagon, expected_area_hexagon, 50.0);
        mliFunc_free(&polygon);
}
