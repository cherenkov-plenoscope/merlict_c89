/* Copyright 2019-2020 Sebastian Achim Mueller                                */

CASE("mliMesh_malloc_from_object_file") {
    mliMesh m = mliMesh_init();
    mliVec support = {0 ,0 ,0};
    mliVec direction = {0 ,0 ,1};
    uint64_t i;
    mliMesh_malloc_from_object_file("tests/resources/text_on_cube.off", &m);
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

CASE("mliMesh_malloc_from_object_file, 2") {
    mliMesh m = mliMesh_init();
    mliMesh_malloc_from_object_file("tests/resources/diff_cube_sphere.off", &m);
    CHECK(m.num_vertices == 432);
    CHECK(m.num_faces == 880);
    mliMesh_free(&m);
}