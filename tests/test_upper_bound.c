/* Copyright 2019-2020 Sebastian Achim Mueller                                */

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
