/* Copyright 2019-2020 Sebastian Achim Mueller                                */

CASE("mliObject, init")
{
        struct mliObject obj = mliObject_init();
        CHECK(obj.num_vertices == 0u);
        CHECK(obj.num_vertex_normals == 0u);
        CHECK(obj.num_faces == 0u);

        CHECK(obj.vertices == NULL);
        CHECK(obj.vertex_normals == NULL);
        CHECK(obj.faces_vertices == NULL);
        CHECK(obj.faces_vertex_normals == NULL);

        CHECK(mliObject_assert_valid_faces(&obj));
}

CASE("mliObject, malloc")
{
        const uint64_t num_vertices = 9;
        const uint64_t num_vertex_normals = 9;
        const uint64_t num_faces = 3;

        struct mliObject obj = mliObject_init();
        CHECK(mliObject_malloc(&obj, num_vertices, num_vertex_normals, num_faces));

        CHECK(obj.num_vertices == num_vertices);
        CHECK(obj.num_vertex_normals == num_vertex_normals);
        CHECK(obj.num_faces == num_faces);

        CHECK(obj.vertices != NULL);
        CHECK(obj.vertex_normals != NULL);
        CHECK(obj.faces_vertices != NULL);
        CHECK(obj.faces_vertex_normals != NULL);

        mliObject_free(&obj);

        CHECK(obj.num_vertices == 0u);
        CHECK(obj.num_vertex_normals == 0u);
        CHECK(obj.num_faces == 0u);

        CHECK(obj.vertices == NULL);
        CHECK(obj.vertex_normals == NULL);
        CHECK(obj.faces_vertices == NULL);
        CHECK(obj.faces_vertex_normals == NULL);
}

CASE("mliObject, parse valid obj face lines")
{
        int line_mode = 0;
        struct mliFace faces_vertices;
        struct mliFace faces_texture_points;
        struct mliFace faces_vertex_normals;
        char line[256];

        strcpy(line, "f 1 2 3");
        CHECK(_mliObject_parse_face_line(
                line,
                &faces_vertices,
                &faces_texture_points,
                &faces_vertex_normals,
                &line_mode));
        CHECK(line_mode == MLI_WAVEFRONT_FACE_LINE_V);
        CHECK(faces_vertices.a == 1);
        CHECK(faces_vertices.b == 2);
        CHECK(faces_vertices.c == 3);

        strcpy(line, "f   2  3   4");
        CHECK(_mliObject_parse_face_line(
                line,
                &faces_vertices,
                &faces_texture_points,
                &faces_vertex_normals,
                &line_mode));
        CHECK(line_mode == MLI_WAVEFRONT_FACE_LINE_V);
        CHECK(faces_vertices.a == 2);
        CHECK(faces_vertices.b == 3);
        CHECK(faces_vertices.c == 4);

        strcpy(line, "f   12132  2432353   34234");
        CHECK(_mliObject_parse_face_line(
                line,
                &faces_vertices,
                &faces_texture_points,
                &faces_vertex_normals,
                &line_mode));
        CHECK(line_mode == MLI_WAVEFRONT_FACE_LINE_V);
        CHECK(faces_vertices.a == 12132);
        CHECK(faces_vertices.b == 2432353);
        CHECK(faces_vertices.c == 34234);

        strcpy(line, "f   12132  2432353   34234 NOT_RELEVANT");
        CHECK(_mliObject_parse_face_line(
                line,
                &faces_vertices,
                &faces_texture_points,
                &faces_vertex_normals,
                &line_mode));
        CHECK(line_mode == MLI_WAVEFRONT_FACE_LINE_V);
        CHECK(faces_vertices.a == 12132);
        CHECK(faces_vertices.b == 2432353);
        CHECK(faces_vertices.c == 34234);

        strcpy(line, "f 2//13 3//14 4//15");
        CHECK(_mliObject_parse_face_line(
                line,
                &faces_vertices,
                &faces_texture_points,
                &faces_vertex_normals,
                &line_mode));
        CHECK(line_mode == MLI_WAVEFRONT_FACE_LINE_V_VN);
        CHECK(faces_vertices.a == 2);
        CHECK(faces_vertices.b == 3);
        CHECK(faces_vertices.c == 4);
        CHECK(faces_vertex_normals.a == 13);
        CHECK(faces_vertex_normals.b == 14);
        CHECK(faces_vertex_normals.c == 15);

        strcpy(line, "f 2//13 3//14 4//15 WHATEVER");
        CHECK(_mliObject_parse_face_line(
                line,
                &faces_vertices,
                &faces_texture_points,
                &faces_vertex_normals,
                &line_mode));
        CHECK(line_mode == MLI_WAVEFRONT_FACE_LINE_V_VN);
        CHECK(faces_vertices.a == 2);
        CHECK(faces_vertices.b == 3);
        CHECK(faces_vertices.c == 4);
        CHECK(faces_vertex_normals.a == 13);
        CHECK(faces_vertex_normals.b == 14);
        CHECK(faces_vertex_normals.c == 15);

        strcpy(line, "f 2/3/1 3/4/8 4/6/9");
        CHECK(_mliObject_parse_face_line(
                line,
                &faces_vertices,
                &faces_texture_points,
                &faces_vertex_normals,
                &line_mode));
        CHECK(line_mode == MLI_WAVEFRONT_FACE_LINE_V_VT_VN);
        CHECK(faces_vertices.a == 2);
        CHECK(faces_vertices.b == 3);
        CHECK(faces_vertices.c == 4);
        CHECK(faces_texture_points.a == 3);
        CHECK(faces_texture_points.b == 4);
        CHECK(faces_texture_points.c == 6);
        CHECK(faces_vertex_normals.a == 1);
        CHECK(faces_vertex_normals.b == 8);
        CHECK(faces_vertex_normals.c == 9);

        strcpy(line, "f  2/3/81   3/4/82   4/6/83");
        CHECK(_mliObject_parse_face_line(
                line,
                &faces_vertices,
                &faces_texture_points,
                &faces_vertex_normals,
                &line_mode));
        CHECK(line_mode == MLI_WAVEFRONT_FACE_LINE_V_VT_VN);
        CHECK(faces_vertices.a == 2);
        CHECK(faces_vertices.b == 3);
        CHECK(faces_vertices.c == 4);
        CHECK(faces_texture_points.a == 3);
        CHECK(faces_texture_points.b == 4);
        CHECK(faces_texture_points.c == 6);
        CHECK(faces_vertex_normals.a == 81);
        CHECK(faces_vertex_normals.b == 82);
        CHECK(faces_vertex_normals.c == 83);

        strcpy(line, "f  2/3/81   3/4/82   4/6/83 I dont care");
        CHECK(_mliObject_parse_face_line(
                line,
                &faces_vertices,
                &faces_texture_points,
                &faces_vertex_normals,
                &line_mode));
        CHECK(line_mode == MLI_WAVEFRONT_FACE_LINE_V_VT_VN);
        CHECK(faces_vertices.a == 2);
        CHECK(faces_vertices.b == 3);
        CHECK(faces_vertices.c == 4);
        CHECK(faces_texture_points.a == 3);
        CHECK(faces_texture_points.b == 4);
        CHECK(faces_texture_points.c == 6);
        CHECK(faces_vertex_normals.a == 81);
        CHECK(faces_vertex_normals.b == 82);
        CHECK(faces_vertex_normals.c == 83);
}


CASE("mliObject, parse bad obj face lines")
{
        int line_mode = 0;
        struct mliFace faces_vertices;
        struct mliFace faces_texture_points;
        struct mliFace faces_vertex_normals;
        char line[128];

        strcpy(line, "f");
        CHECK(!_mliObject_parse_face_line(
                line,
                &faces_vertices,
                &faces_texture_points,
                &faces_vertex_normals,
                &line_mode));
        CHECK(line_mode == -1);

        strcpy(line, "f 34");
        CHECK(!_mliObject_parse_face_line(
                line,
                &faces_vertices,
                &faces_texture_points,
                &faces_vertex_normals,
                &line_mode));
        CHECK(line_mode == -1);

        strcpy(line, "f 34 242 44/");
        CHECK(!_mliObject_parse_face_line(
                line,
                &faces_vertices,
                &faces_texture_points,
                &faces_vertex_normals,
                &line_mode));
        CHECK(line_mode == -1);

        strcpy(line, "");
        CHECK(!_mliObject_parse_face_line(
                line,
                &faces_vertices,
                &faces_texture_points,
                &faces_vertex_normals,
                &line_mode));
        CHECK(line_mode == -1);

        /* provoce int64 overflow */
        strcpy(
                line,
                "f "
                "999999999999999999999999999999"
                "999999999999999999999999999999"
                " 0 0"
        );
        CHECK(!_mliObject_parse_face_line(
                line,
                &faces_vertices,
                &faces_texture_points,
                &faces_vertex_normals,
                &line_mode));
        CHECK(line_mode == -1);

        /* provoce int buffer length */
        strcpy(
                line,
                "f "
                "9999999999999999999999999999999999999"
                "9999999999999999999999999999999999999"
                "9999999999999999999999999999999999999"
                " 0 0"
        );
        CHECK(!_mliObject_parse_face_line(
                line,
                &faces_vertices,
                &faces_texture_points,
                &faces_vertex_normals,
                &line_mode));
        CHECK(line_mode == -1);
}
