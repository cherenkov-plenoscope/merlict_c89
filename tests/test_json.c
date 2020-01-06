/* Copyright 2019 Sebastian Achim Mueller                                     */

CASE("Tokenize json-literal") {
    char s[] = "{\"hans\": 84, \"cosmo\": [1, 3, 7]}\0";
    char *end;
    int64_t num_tokens;
    jsmn_parser p;
    jsmntok_t t[128]; /* We expect no more than 128 JSON tokens */

    jsmn_init(&p);
    num_tokens = jsmn_parse(&p, s, strlen(s), t, 128);

    CHECK(num_tokens == 8);
    CHECK(t[0].type == JSMN_OBJECT);
    CHECK(t[0].size == 2);

    CHECK(t[1].type == JSMN_STRING);
    CHECK(t[1].size == 1);

    CHECK(t[2].type == JSMN_PRIMITIVE);
    CHECK(t[2].size == 0);

    CHECK(t[3].type == JSMN_STRING);
    CHECK(t[3].size == 1);

    CHECK(t[4].type == JSMN_ARRAY);
    CHECK(t[4].size == 3);

    CHECK(t[5].type == JSMN_PRIMITIVE);
    CHECK(t[5].size == 0);
    CHECK(strtol(s + t[5].start, &end, 10) == 1);
    CHECK(end == s + t[5].end);

    CHECK(t[6].type == JSMN_PRIMITIVE);
    CHECK(t[6].size == 0);
    CHECK(strtol(s + t[6].start, &end, 10) == 3);
    CHECK(end == s + t[6].end);

    CHECK(t[7].type == JSMN_PRIMITIVE);
    CHECK(t[7].size == 0);
    CHECK(strtol(s + t[7].start, &end, 10) == 7);
    CHECK(end == s + t[7].end);
}

CASE("mliJson_as_string") {
    mliJson json = mliJson_init();
    char json_str[] = "{\"hans\": 1337}";
    /*             0         1 */
    /*             01234567890 */
    /*             hans0       */
    char buff[] = "abcde";
    CHECK(mliJson_malloc_from_string(&json, json_str));
    CHECK(mliJson_as_string(&json, 1, buff, 5));
    CHECK(buff[0] == 'h');
    CHECK(buff[1] == 'a');
    CHECK(buff[2] == 'n');
    CHECK(buff[3] == 's');
    CHECK(buff[4] == '\0');

    CHECK(!mliJson_as_string(&json, 1, buff, 4));
    mliJson_free(&json);
}

CASE("mliJson_init, defaults") {
    mliJson json = mliJson_init();
    CHECK(json.num_chars == 0u);
    CHECK(json.chars == NULL);
    CHECK(json.num_tokens == 0u);
    CHECK(json.tokens == NULL);
}

CASE("mliJson malloc, and free") {
    mliJson json = mliJson_init();
    json.num_chars = 10;
    json.num_tokens = 5;
    CHECK(mliJson_malloc(&json));
    mliJson_free(&json);
}

CASE("mliJson_malloc_from_file") {
    mliJson json = mliJson_init();
    uint64_t return_idx;
    int64_t myint;
    double myfloat;

    CHECK(mliJson_malloc_from_file(&json, "tests/resources/example.json"));
    CHECK(mliJson_write_debug(&json, "tests/resources/example.debug.tmp"));

    CHECK(mliJson_find_key(&json, 0, "name", &return_idx));
    CHECK(return_idx == 1);
    CHECK(mliJson_find_key(&json, 0, "skill", &return_idx));
    CHECK(return_idx == 3);
    CHECK(mliJson_as_float64(&json, 3 + 1, &myfloat));
    CHECK(myfloat == 1.337);
    CHECK(mliJson_find_key(&json, 0, "bug_rate", &return_idx));
    CHECK(return_idx == 5);
    CHECK(mliJson_as_int64(&json, 5 + 1, &myint));
    CHECK(myint == 42);

    CHECK(mliJson_find_key(&json, 0, "some_numbers", &return_idx));
    CHECK(return_idx == 7);
    CHECK(mliJson_find_key(&json, 0, "fair", &return_idx));
    CHECK(return_idx == 13);

    CHECK(mliJson_find_key(&json, 14, "trade", &return_idx));
    CHECK(return_idx == 15);

    CHECK(mliJson_find_key(&json, 14, "uff", &return_idx));
    CHECK(return_idx == 17);

    CHECK(mliJson_find_key(&json, 0, "not_exist", &return_idx) == 0);
    CHECK(mliJson_find_key(&json, 14, "not_exist", &return_idx) == 0);
    mliJson_free(&json);
}

CASE("mliJson_malloc_from_string") {
    mliJson json = mliJson_init();
    char json_str[1024];
    uint64_t token = 0u;
    int64_t value = 0;
    sprintf(json_str, "{\"key\": 1337}");
    CHECK(mliJson_malloc_from_string(&json, json_str));
    CHECK(mliJson_find_key(&json, 0, "key", &token));
    CHECK(mliJson_as_int64(&json, token + 1, &value));
    CHECK(value == 1337);
    mliJson_free(&json);
}

CASE("parse mliFunc") {
    uint64_t token;
    mliJson json = mliJson_init();
    mliFunc f = mliFunc_init();
    CHECK(mliJson_malloc_from_file(&json, "tests/resources/function.json"));
    CHECK(mliJson_write_debug(&json, "tests/resources/function.debug.tmp"));
    CHECK(mliJson_find_key(&json, 0, "two_functions", &token));
    CHECK(json.tokens[token + 1].size == 2);
    token += 1;
    CHECK(token == 2);
    token += 1;
    CHECK(token == 3);
    /* the first function */
    CHECK(mliFunc_malloc_from_json_token(&f, &json, token));
    CHECK(f.num_points == 2);
    CHECK_MARGIN(f.x[0], 200e-9, 1e-9);
    CHECK_MARGIN(f.x[1], 1200e-9, 1e-9);
    CHECK_MARGIN(f.y[0], 0., 1e-9);
    CHECK_MARGIN(f.y[1], 0., 1e-9);
    mliFunc_free(&f);

    token = 10;
    /* the second function */
    f = mliFunc_init();
    CHECK(mliFunc_malloc_from_json_token(&f, &json, token));
    CHECK(f.num_points == 3);
    CHECK_MARGIN(f.x[0], -200e-9, 1e-9);
    CHECK_MARGIN(f.x[1], 600e-9, 1e-9);
    CHECK_MARGIN(f.x[2], 1200e-9, 1e-9);
    CHECK_MARGIN(f.y[0], 1.49, 1e-6);
    CHECK_MARGIN(f.y[1], -0.59, 1e-6);
    CHECK_MARGIN(f.y[2], -7.9, 1e-6);
    mliFunc_free(&f);

    mliJson_free(&json);
}

CASE("parse mliVec and mliColor") {
    uint64_t token;
    mliJson json = mliJson_init();
    mliVec vec1 = mliVec_set(0., 0., 0.);
    mliVec vec2 = mliVec_set(0., 0., 0.);
    mliColor col = mliColor_set(0., 0., 0.);
    CHECK(mliJson_malloc_from_file(&json, "tests/resources/vec.json"));
    CHECK(mliJson_write_debug(&json, "tests/resources/vec.debug.tmp"));

    CHECK(mliJson_find_key(&json, 0, "vec1", &token));
    CHECK(mliVec_from_json_token(&vec1, &json, token + 1));
    CHECK_MARGIN(vec1.x, 1.5, 1e-6);
    CHECK_MARGIN(vec1.y, 2.5, 1e-6);
    CHECK_MARGIN(vec1.z, 3.5, 1e-6);

    CHECK(mliJson_find_key(&json, 0, "vec2", &token));
    CHECK(mliVec_from_json_token(&vec2, &json, token + 1));
    CHECK_MARGIN(vec2.x, 1.2, 1e-6);
    CHECK_MARGIN(vec2.y, 3.4, 1e-6);
    CHECK_MARGIN(vec2.z, -5.6, 1e-6);

    CHECK(mliJson_find_key(&json, 0, "color", &token));
    CHECK(mliColor_from_json_token(&col, &json, token + 1));
    CHECK_MARGIN(col.r, 128., 1e-6);
    CHECK_MARGIN(col.g, 255., 1e-6);
    CHECK_MARGIN(col.b, 12., 1e-6);

    mliJson_free(&json);
}

CASE("rotation representations") {
    char json_str[1024];
    mliQuaternion q;
    mliQuaternion q_expected;
    mliJson json = mliJson_init();

    /* unity */
    q_expected = mliQuaternion_set_tait_bryan(0., 0., 0.);

    sprintf(
        json_str,
        "{\"repr\": \"tait_bryan\", \"xyz\": [0, 0, 0]}");
    CHECK(mliJson_malloc_from_string(&json, json_str));
    CHECK(__mliQuaternion_from_json(&q, &json, 0));
    mliJson_free(&json);
    CHECK(mliQuaternion_is_equal_margin(q, q_expected, 1e-6));

    sprintf(
        json_str,
        "{\"repr\": \"axis_angle\", \"axis\": [0, 0, 0], \"angle\": 0.}");
    CHECK(mliJson_malloc_from_string(&json, json_str));
    CHECK(__mliQuaternion_from_json(&q, &json, 0));
    mliJson_free(&json);
    CHECK(mliQuaternion_is_equal_margin(q, q_expected, 1e-6));

    sprintf(
        json_str,
        "{\"repr\": \"quaternion\", \"xyz\": [0, 0, 0]}");
    CHECK(mliJson_malloc_from_string(&json, json_str));
    CHECK(__mliQuaternion_from_json(&q, &json, 0));
    mliJson_free(&json);
    CHECK(mliQuaternion_is_equal_margin(q, q_expected, 1e-6));

    /* z-axis, 45deg */
    q_expected = mliQuaternion_set_tait_bryan(0., 0., -mli_deg2rad(45.));

    sprintf(
        json_str,
        "{\"repr\": \"tait_bryan\", \"xyz\": [0, 0, %f]}",
        -mli_deg2rad(45.));
    CHECK(mliJson_malloc_from_string(&json, json_str));
    CHECK(__mliQuaternion_from_json(&q, &json, 0));
    mliJson_free(&json);
    CHECK(mliQuaternion_is_equal_margin(q, q_expected, 1e-6));

    sprintf(
        json_str,
        "{\"repr\": \"axis_angle\", \"axis\": [0, 0, 1], \"angle\": %f}",
        mli_deg2rad(45.));
    CHECK(mliJson_malloc_from_string(&json, json_str));
    CHECK(__mliQuaternion_from_json(&q, &json, 0));
    mliJson_free(&json);
    CHECK(mliQuaternion_is_equal_margin(q, q_expected, 1e-6));

    sprintf(
        json_str,
        "{\"repr\": \"quaternion\", \"xyz\": [0, 0, %f]}", q_expected.z);
    CHECK(mliJson_malloc_from_string(&json, json_str));
    CHECK(__mliQuaternion_from_json(&q, &json, 0));
    mliJson_free(&json);
    CHECK(mliQuaternion_is_equal_margin(q, q_expected, 1e-6));
}

CASE("parse mliUserScenery") {
    mliJson json = mliJson_init();
    mliUserScenery uscn = mliUserScenery_init();
    mliFrame* f;
    mliSurface* uscn_srfs;
    CHECK(mliJson_malloc_from_file(&json,
        "tests/resources/small_scenery.json"));
    CHECK(mliJson_write_debug(&json,
        "tests/resources/small_scenery.debug.tmp"));

    CHECK(mliUserScenery_malloc_from_json(&uscn, &json));

    /* functions */
    CHECK(uscn.surface_resources.num_functions == 2u);
    CHECK(uscn.surface_resources.functions[0].num_points == 2u);
    CHECK_MARGIN(uscn.surface_resources.functions[0].x[0], 200e-9, 1e-9);
    CHECK_MARGIN(uscn.surface_resources.functions[0].y[0], 0., 1e-9);
    CHECK_MARGIN(uscn.surface_resources.functions[0].x[1], 1200e-9, 1e-9);
    CHECK_MARGIN(uscn.surface_resources.functions[0].y[1], 0., 1e-9);
    CHECK(uscn.surface_resources.functions[1].num_points == 2u);
    CHECK_MARGIN(uscn.surface_resources.functions[1].x[0], 200e-9, 1e-9);
    CHECK_MARGIN(uscn.surface_resources.functions[1].y[0], 1.49, 1e-9);
    CHECK_MARGIN(uscn.surface_resources.functions[1].x[1], 1200e-9, 1e-9);
    CHECK_MARGIN(uscn.surface_resources.functions[1].y[1], 1.49, 1e-9);

    /* colors */
    CHECK(uscn.surface_resources.num_colors == 4u);
    CHECK(mliColor_is_equal(
        uscn.surface_resources.colors[0],
        mliColor_set(22, 91, 49)));
    CHECK(mliColor_is_equal(
        uscn.surface_resources.colors[1],
        mliColor_set(122, 91, 49)));
    CHECK(mliColor_is_equal(
        uscn.surface_resources.colors[2],
        mliColor_set(22, 191, 49)));
    CHECK(mliColor_is_equal(
        uscn.surface_resources.colors[3],
        mliColor_set(22, 91, 149)));

    /* surfaces */
    CHECK(uscn.surface_resources.num_surfaces == 4u);
    uscn_srfs = uscn.surface_resources.surfaces;

    CHECK(uscn_srfs[0].color == 0u);
    CHECK(uscn_srfs[0].material == MLI_MATERIAL_PHONG);
    CHECK(uscn_srfs[0].boundary_layer_specular_reflection == 0u);
    CHECK(uscn_srfs[0].boundary_layer_diffuse_reflection == 0u);
    CHECK(uscn_srfs[0].medium_refraction == 0u);
    CHECK(uscn_srfs[0].medium_absorbtion == 0u);

    CHECK(uscn_srfs[1].color == 1u);
    CHECK(uscn_srfs[1].material == MLI_MATERIAL_PHONG);
    CHECK(uscn_srfs[1].boundary_layer_specular_reflection == 0u);
    CHECK(uscn_srfs[1].boundary_layer_diffuse_reflection == 0u);
    CHECK(uscn_srfs[1].medium_refraction == 0u);
    CHECK(uscn_srfs[1].medium_absorbtion == 0u);

    CHECK(uscn_srfs[2].color == 2u);
    CHECK(uscn_srfs[2].material == MLI_MATERIAL_PHONG);
    CHECK(uscn_srfs[2].boundary_layer_specular_reflection == 0u);
    CHECK(uscn_srfs[2].boundary_layer_diffuse_reflection == 0u);
    CHECK(uscn_srfs[2].medium_refraction == 0u);
    CHECK(uscn_srfs[2].medium_absorbtion == 0u);

    CHECK(uscn_srfs[3].color == 3u);
    CHECK(uscn_srfs[3].material == MLI_MATERIAL_PHONG);
    CHECK(uscn_srfs[3].boundary_layer_specular_reflection == 0u);
    CHECK(uscn_srfs[3].boundary_layer_diffuse_reflection == 0u);
    CHECK(uscn_srfs[3].medium_refraction == 1u);
    CHECK(uscn_srfs[3].medium_absorbtion == 0u);

    /* frames */
    CHECK(uscn.root.children.size == 4u);
    f = mliFrame_child(&uscn.root, 0);
    CHECK(f->type == MLI_DISC);
    f = mliFrame_child(&uscn.root, 1);
    CHECK(f->type == MLI_FRAME);
    f = mliFrame_child(&uscn.root, 2);
    CHECK(f->type == MLI_MESH);
    CHECK(f->primitive.mesh->num_vertices == 4u);
    CHECK_MARGIN(f->primitive.mesh->vertices[0].x, 0., 1e-6);
    CHECK_MARGIN(f->primitive.mesh->vertices[0].y, 0., 1e-6);
    CHECK_MARGIN(f->primitive.mesh->vertices[0].z, 0., 1e-6);
    CHECK_MARGIN(f->primitive.mesh->vertices[1].x, 1., 1e-6);
    CHECK_MARGIN(f->primitive.mesh->vertices[1].y, 0., 1e-6);
    CHECK_MARGIN(f->primitive.mesh->vertices[1].z, 0., 1e-6);

    CHECK(f->primitive.mesh->num_faces == 4u);
    CHECK(f->primitive.mesh->faces[0].a == 0);
    CHECK(f->primitive.mesh->faces[0].b == 1);
    CHECK(f->primitive.mesh->faces[0].c == 2);
    CHECK(f->primitive.mesh->faces[1].a == 0);
    CHECK(f->primitive.mesh->faces[1].b == 1);
    CHECK(f->primitive.mesh->faces[1].c == 3);

    f = mliFrame_child(&uscn.root, 3);
    CHECK(f->type == MLI_SPHERE);

    mliUserScenery_free(&uscn);
    mliJson_free(&json);
}
