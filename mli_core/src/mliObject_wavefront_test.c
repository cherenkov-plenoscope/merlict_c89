/* Copyright 2019-2020 Sebastian Achim Mueller                                */

#include "../../mli_testing/include/mli_testing.h"
#include "mliObject_wavefront.h"

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

        CHECK(mliObject_has_valid_faces(&obj));
}

CASE("mliObject, malloc")
{
        const uint64_t num_vertices = 9;
        const uint64_t num_vertex_normals = 9;
        const uint64_t num_faces = 3;
        const uint64_t num_materials = 1;

        struct mliObject obj = mliObject_init();
        CHECK(mliObject_malloc(
                &obj,
                num_vertices,
                num_vertex_normals,
                num_faces,
                num_materials));

        CHECK(obj.num_vertices == num_vertices);
        CHECK(obj.num_vertex_normals == num_vertex_normals);
        CHECK(obj.num_faces == num_faces);
        CHECK(obj.num_materials == num_materials);

        CHECK(obj.vertices != NULL);
        CHECK(obj.vertex_normals != NULL);
        CHECK(obj.faces_vertices != NULL);
        CHECK(obj.faces_vertex_normals != NULL);
        CHECK(obj.faces_materials != NULL);

        mliObject_free(&obj);

        CHECK(obj.num_vertices == 0u);
        CHECK(obj.num_vertex_normals == 0u);
        CHECK(obj.num_faces == 0u);
        CHECK(obj.num_materials == 0u);

        CHECK(obj.vertices == NULL);
        CHECK(obj.vertex_normals == NULL);
        CHECK(obj.faces_vertices == NULL);
        CHECK(obj.faces_vertex_normals == NULL);
        CHECK(obj.faces_materials == NULL);
}

CASE("mliObject, parse valid obj face lines")
{
        int line_mode = 0;
        struct mliFace faces_vertices;
        struct mliFace faces_texture_points;
        struct mliFace faces_vertex_normals;
        char line[256];

        strcpy(line, "f 1 2 3");
        CHECK(mliObject_parse_face_line(
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
        CHECK(mliObject_parse_face_line(
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
        CHECK(mliObject_parse_face_line(
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
        CHECK(mliObject_parse_face_line(
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
        CHECK(mliObject_parse_face_line(
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
        CHECK(mliObject_parse_face_line(
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
        CHECK(mliObject_parse_face_line(
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
        CHECK(mliObject_parse_face_line(
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
        CHECK(mliObject_parse_face_line(
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
        CHECK(!mliObject_parse_face_line(
                line,
                &faces_vertices,
                &faces_texture_points,
                &faces_vertex_normals,
                &line_mode));
        CHECK(line_mode == -1);

        strcpy(line, "f 34");
        CHECK(!mliObject_parse_face_line(
                line,
                &faces_vertices,
                &faces_texture_points,
                &faces_vertex_normals,
                &line_mode));
        CHECK(line_mode == -1);

        strcpy(line, "f 34 242 44/");
        CHECK(!mliObject_parse_face_line(
                line,
                &faces_vertices,
                &faces_texture_points,
                &faces_vertex_normals,
                &line_mode));
        CHECK(line_mode == -1);

        strcpy(line, "");
        CHECK(!mliObject_parse_face_line(
                line,
                &faces_vertices,
                &faces_texture_points,
                &faces_vertex_normals,
                &line_mode));
        CHECK(line_mode == -1);

        /* provoce int64 overflow */
        strcpy(line,
               "f "
               "999999999999999999999999999999"
               "999999999999999999999999999999"
               " 0 0");
        CHECK(!mliObject_parse_face_line(
                line,
                &faces_vertices,
                &faces_texture_points,
                &faces_vertex_normals,
                &line_mode));
        CHECK(line_mode == -1);

        /* provoce int buffer length */
        strcpy(line,
               "f "
               "9999999999999999999999999999999999999"
               "9999999999999999999999999999999999999"
               "9999999999999999999999999999999999999"
               " 0 0");
        CHECK(!mliObject_parse_face_line(
                line,
                &faces_vertices,
                &faces_texture_points,
                &faces_vertex_normals,
                &line_mode));
        CHECK(line_mode == -1);
}

CASE("mliObject, parse valid obj-float-lines")
{
        struct mliVec v;
        char line[128];

        strcpy(line, " 0 0 0");
        CHECK(mliObject_parse_three_float_line(line, &v));
        CHECK(v.x == 0.0);
        CHECK(v.y == 0.0);
        CHECK(v.z == 0.0);

        strcpy(line, " 0.0 0.0 0.0");
        CHECK(mliObject_parse_three_float_line(line, &v));
        CHECK(v.x == 0.0);
        CHECK(v.y == 0.0);
        CHECK(v.z == 0.0);

        strcpy(line, " 0.0   0.0    0.0   whatever");
        CHECK(mliObject_parse_three_float_line(line, &v));
        CHECK(v.x == 0.0);
        CHECK(v.y == 0.0);
        CHECK(v.z == 0.0);

        strcpy(line, "     0 0 0");
        CHECK(mliObject_parse_three_float_line(line, &v));
        CHECK(v.x == 0.0);
        CHECK(v.y == 0.0);
        CHECK(v.z == 0.0);

        strcpy(line, " 1 2 3");
        CHECK(mliObject_parse_three_float_line(line, &v));
        CHECK(v.x == 1.0);
        CHECK(v.y == 2.0);
        CHECK(v.z == 3.0);

        strcpy(line, " 1 -2 3");
        CHECK(mliObject_parse_three_float_line(line, &v));
        CHECK(v.x == 1.0);
        CHECK(v.y == -2.0);
        CHECK(v.z == 3.0);

        strcpy(line, " 1.4 2.5 3.6");
        CHECK(mliObject_parse_three_float_line(line, &v));
        CHECK_MARGIN(v.x, 1.4, 1e-9);
        CHECK_MARGIN(v.y, 2.5, 1e-9);
        CHECK_MARGIN(v.z, 3.6, 1e-9);

        strcpy(line, " 1.4e0 2.5e1 3.6e2");
        CHECK(mliObject_parse_three_float_line(line, &v));
        CHECK_MARGIN(v.x, 1.4e0, 1e-9);
        CHECK_MARGIN(v.y, 2.5e1, 1e-9);
        CHECK_MARGIN(v.z, 3.6e2, 1e-9);

        strcpy(line, " 1.4e-0 2.5e-1 3.6e2 ");
        CHECK(mliObject_parse_three_float_line(line, &v));
        CHECK_MARGIN(v.x, 1.4e0, 1e-9);
        CHECK_MARGIN(v.y, 2.5e-1, 1e-9);
        CHECK_MARGIN(v.z, 3.6e2, 1e-9);

        strcpy(line, " 1.4e-0       2.5e-1     3.6e2  do not care");
        CHECK(mliObject_parse_three_float_line(line, &v));
        CHECK_MARGIN(v.x, 1.4e0, 1e-9);
        CHECK_MARGIN(v.y, 2.5e-1, 1e-9);
        CHECK_MARGIN(v.z, 3.6e2, 1e-9);
}

CASE("mliObject, parse bad obj-float-lines")
{
        struct mliVec v;
        char line[128];

        strcpy(line, "");
        CHECK(!mliObject_parse_three_float_line(line, &v));

        strcpy(line, " ");
        CHECK(!mliObject_parse_three_float_line(line, &v));

        strcpy(line, "0 0 0");
        CHECK(!mliObject_parse_three_float_line(line, &v));

        strcpy(line, "abc");
        CHECK(!mliObject_parse_three_float_line(line, &v));
}

CASE("mliObject, read wavefront file")
{
        struct mliIo str = mliIo_init();
        struct mliObject obj = mliObject_init();
        CHECK(mliIo_malloc_from_path(
                &str,
                "tests/"
                "resources/"
                "sceneries/"
                "001/"
                "geometry/"
                "objects/"
                "hexagonal_mirror_facet.obj"));
        CHECK(mliObject_malloc_from_wavefront(&obj, (char *)str.cstr));
        mliIo_free(&str);

        CHECK(obj.num_faces == 600);
        CHECK(obj.num_vertices == 331);
        CHECK(obj.num_vertex_normals == 331);
        CHECK(mliObject_has_valid_faces(&obj));
        mliObject_free(&obj);
}

CASE("mliObject, write and read binary-string")
{
        uint64_t i;
        struct mliIo str = mliIo_init();
        struct mliObject obj = mliObject_init();
        struct mliObject obj_back = mliObject_init();
        FILE *f;
        CHECK(mliIo_malloc_from_path(
                &str,
                "tests/"
                "resources/"
                "sceneries/"
                "001/"
                "geometry/"
                "objects/"
                "hexagonal_mirror_facet.obj"));
        CHECK(mliObject_malloc_from_wavefront(&obj, (char *)str.cstr));
        mliIo_free(&str);

        f = fopen("tests/resources/hexagonal_mirror_facet.bin.tmp", "w");
        CHECK(f != NULL);
        mliObject_fwrite(&obj, f);
        fclose(f);

        f = fopen("tests/resources/hexagonal_mirror_facet.bin.tmp", "r");
        CHECK(f != NULL);
        mliObject_malloc_fread(&obj_back, f);
        fclose(f);

        CHECK(obj.num_vertices == obj_back.num_vertices);
        CHECK(obj.num_vertex_normals == obj_back.num_vertex_normals);
        CHECK(obj.num_faces == obj_back.num_faces);

        for (i = 0; i < obj.num_vertices; i++) {
                CHECK(mliVec_equal(obj.vertices[i], obj_back.vertices[i]));
        }
        for (i = 0; i < obj.num_vertex_normals; i++) {
                CHECK(mliVec_equal(
                        obj.vertex_normals[i], obj_back.vertex_normals[i]));
        }
        for (i = 0; i < obj.num_faces; i++) {
                CHECK(mliFace_equal(
                        obj.faces_vertices[i], obj_back.faces_vertices[i]));
                CHECK(mliFace_equal(
                        obj.faces_vertex_normals[i],
                        obj_back.faces_vertex_normals[i]));
        }

        CHECK(mliObject_equal(&obj, &obj_back));

        mliObject_free(&obj);
        mliObject_free(&obj_back);
}

CASE("mliObject, write and read ascii-text-string")
{
        uint64_t i;
        struct mliIo f = mliIo_init();
        struct mliIo str = mliIo_init();
        struct mliObject obj = mliObject_init();
        struct mliObject obj_back = mliObject_init();
        CHECK(mliIo_malloc_from_path(
                &str,
                "tests/"
                "resources/"
                "sceneries/"
                "001/"
                "geometry/"
                "objects/"
                "hexagonal_mirror_facet.obj"));
        CHECK(mliObject_malloc_from_wavefront(&obj, (char *)str.cstr));
        mliIo_free(&str);

        mliObject_fprint_to_wavefront(&f, &obj);
        mliIo_rewind(&f);
        mliIo_read_to_path(
                &f, "tests/resources/hexagonal_mirror_facet.obj.tmp");
        mliIo_free(&f);

        CHECK(mliIo_malloc_from_path(
                &str, "tests/resources/hexagonal_mirror_facet.obj.tmp"));
        CHECK(mliObject_malloc_from_wavefront(&obj_back, (char *)str.cstr));
        mliIo_free(&str);

        CHECK(obj.num_vertices == obj_back.num_vertices);
        CHECK(obj.num_vertex_normals == obj_back.num_vertex_normals);
        CHECK(obj.num_faces == obj_back.num_faces);
        CHECK(obj.num_materials == obj_back.num_materials);

        for (i = 0; i < obj.num_vertices; i++) {
                CHECK(mliVec_equal_margin(
                        obj.vertices[i], obj_back.vertices[i], 1e-6));
        }
        for (i = 0; i < obj.num_vertex_normals; i++) {
                CHECK(mliVec_equal_margin(
                        obj.vertex_normals[i],
                        obj_back.vertex_normals[i],
                        1e-6));
        }
        for (i = 0; i < obj.num_faces; i++) {
                CHECK(mliFace_equal(
                        obj.faces_vertices[i], obj_back.faces_vertices[i]));
                CHECK(mliFace_equal(
                        obj.faces_vertex_normals[i],
                        obj_back.faces_vertex_normals[i]));
                CHECK(obj.faces_materials[i] == obj_back.faces_materials[i]);
        }

        mliObject_free(&obj);
        mliObject_free(&obj_back);
}

CASE("mliObject, read and write multiple materials")
{
        struct mliIo f = mliIo_init();
        struct mliIo str = mliIo_init();
        struct mliObject obj_orig = mliObject_init();
        struct mliObject obj_back = mliObject_init();
        CHECK(mliIo_malloc_from_path(
                &str,
                "tests/"
                "resources/"
                "sceneries/"
                "002/"
                "geometry/"
                "objects/"
                "cube_with_materials.obj"));
        CHECK(mliObject_malloc_from_wavefront(&obj_orig, (char *)str.cstr));
        mliIo_free(&str);
        CHECK(obj_orig.num_vertices == 8);
        CHECK(obj_orig.num_vertex_normals == 6);
        CHECK(obj_orig.num_faces == 12);
        CHECK(obj_orig.num_materials == 6);

        mliObject_fprint_to_wavefront(&f, &obj_orig);
        mliIo_rewind(&f);
        mliIo_read_to_path(&f, "tests/resources/cube_with_materials.obj.tmp");
        mliIo_free(&f);

        CHECK(mliIo_malloc_from_path(
                &str,
                "tests/"
                "resources/"
                "cube_with_materials.obj.tmp"));
        CHECK(mliObject_malloc_from_wavefront(&obj_back, (char *)str.cstr));
        mliIo_free(&str);

        CHECK(obj_back.num_vertices == 8);
        CHECK(obj_back.num_vertex_normals == 6);
        CHECK(obj_back.num_faces == 12);
        CHECK(obj_back.num_materials == 6);

        CHECK(0 == mliObject_resolve_material_idx(&obj_back, 0));
        CHECK(0 == mliObject_resolve_material_idx(&obj_back, 1));

        CHECK(1 == mliObject_resolve_material_idx(&obj_back, 2));
        CHECK(1 == mliObject_resolve_material_idx(&obj_back, 3));

        CHECK(2 == mliObject_resolve_material_idx(&obj_back, 4));
        CHECK(2 == mliObject_resolve_material_idx(&obj_back, 5));

        CHECK(3 == mliObject_resolve_material_idx(&obj_back, 6));
        CHECK(3 == mliObject_resolve_material_idx(&obj_back, 7));

        CHECK(4 == mliObject_resolve_material_idx(&obj_back, 8));
        CHECK(4 == mliObject_resolve_material_idx(&obj_back, 9));

        CHECK(5 == mliObject_resolve_material_idx(&obj_back, 10));
        CHECK(5 == mliObject_resolve_material_idx(&obj_back, 11));

        mliObject_free(&obj_orig);
        mliObject_free(&obj_back);
}

CASE("mliObject, read and write repeating materials")
{
        struct mliIo f = mliIo_init();
        struct mliIo str = mliIo_init();
        struct mliObject obj_orig = mliObject_init();
        struct mliObject obj_back = mliObject_init();
        CHECK(mliIo_malloc_from_path(
                &str,
                "tests/"
                "resources/"
                "repeating_material.obj"));
        CHECK(mliObject_malloc_from_wavefront(&obj_orig, (char *)str.cstr));
        mliIo_free(&str);
        CHECK(obj_orig.num_vertices == 5);
        CHECK(obj_orig.num_vertex_normals == 1);
        CHECK(obj_orig.num_faces == 3);
        CHECK(obj_orig.num_materials == 2);

        mliObject_fprint_to_wavefront(&f, &obj_orig);
        mliIo_rewind(&f);
        mliIo_read_to_path(&f, "tests/resources/repeating_material.obj.tmp");
        mliIo_free(&f);

        CHECK(mliIo_malloc_from_path(
                &str,
                "tests/"
                "resources/"
                "repeating_material.obj.tmp"));
        CHECK(mliObject_malloc_from_wavefront(&obj_back, (char *)str.cstr));
        mliIo_free(&str);

        CHECK(obj_back.num_vertices == obj_orig.num_vertices);
        CHECK(obj_back.num_vertex_normals == obj_orig.num_vertex_normals);
        CHECK(obj_back.num_faces == obj_orig.num_faces);
        CHECK(obj_back.num_materials == obj_orig.num_materials);

        CHECK(0 == mliObject_resolve_material_idx(&obj_back, 0));
        CHECK(1 == mliObject_resolve_material_idx(&obj_back, 1));
        CHECK(0 == mliObject_resolve_material_idx(&obj_back, 2));

        mliObject_free(&obj_orig);
        mliObject_free(&obj_back);
}
