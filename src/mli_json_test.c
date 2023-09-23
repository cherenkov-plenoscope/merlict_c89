/* Copyright 2019 Sebastian Achim Mueller                                     */

CASE("Tokenize json-literal")
{
        char s[] = "{\"hans\": 84, \"cosmo\": [1, 3, 7]}\0";
        char *end;
        int64_t num_tokens;
        struct jsmn_parser p;
        struct jsmntok_t t[128]; /* We expect no more than 128 JSON tokens */

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

CASE("mliJson_cstr_by_token")
{
        struct mliJson json = mliJson_init();
        char json_str[] = "{\"hans\": 1337}";
        char buff[] = "abcde";
        CHECK(mliJson_malloc_from_cstr(&json, json_str));
        CHECK(mliJson_cstr_by_token(&json, 1, buff, 5));
        CHECK(buff[0] == 'h');
        CHECK(buff[1] == 'a');
        CHECK(buff[2] == 'n');
        CHECK(buff[3] == 's');
        CHECK(buff[4] == '\0');

        CHECK(!mliJson_cstr_by_token(&json, 1, buff, 4));
        mliJson_free(&json);
}

CASE("mliJson_init, defaults")
{
        struct mliJson json = mliJson_init();
        CHECK(json.raw.length == 0u);
        CHECK(json.raw.cstr == NULL);
        CHECK(json.num_tokens == 0u);
        CHECK(json.tokens == NULL);
}

CASE("mliJson_malloc_from_path")
{
        struct mliJson json = mliJson_init();
        uint64_t return_idx;
        int64_t myint;
        double myfloat;

        CHECK(mliJson_malloc_from_path(
                &json, "tests/resources/mliJson/example.json"));
        CHECK(mliJson_debug_to_path(
                &json, "tests/resources/mliJson/example.debug.tmp"));

        CHECK(mliJson_token_by_key(&json, 0, "name", &return_idx));
        CHECK(return_idx == 1);
        CHECK(mliJson_token_by_key(&json, 0, "skill", &return_idx));
        CHECK(return_idx == 3);
        CHECK(mliJson_double_by_token(&json, 3 + 1, &myfloat));
        CHECK(myfloat == 1.337);
        CHECK(mliJson_token_by_key(&json, 0, "bug_rate", &return_idx));
        CHECK(return_idx == 5);
        CHECK(mliJson_int64_by_token(&json, 5 + 1, &myint));
        CHECK(myint == 42);

        CHECK(mliJson_token_by_key(&json, 0, "some_numbers", &return_idx));
        CHECK(return_idx == 7);
        CHECK(mliJson_token_by_key(&json, 0, "fair", &return_idx));
        CHECK(return_idx == 13);

        CHECK(mliJson_token_by_key(&json, 14, "trade", &return_idx));
        CHECK(return_idx == 15);

        CHECK(mliJson_token_by_key(&json, 14, "uff", &return_idx));
        CHECK(return_idx == 17);

        CHECK(mliJson_token_by_key(&json, 0, "not_exist", &return_idx) == 0);
        CHECK(mliJson_token_by_key(&json, 14, "not_exist", &return_idx) == 0);
        mliJson_free(&json);
}

CASE("mliJson_malloc_from_cstr")
{
        struct mliJson json = mliJson_init();
        char json_str[1024];
        uint64_t token = 0u;
        int64_t value = 0;
        sprintf(json_str, "{\"key\": 1337}");
        CHECK(mliJson_malloc_from_cstr(&json, json_str));
        CHECK(mliJson_token_by_key(&json, 0, "key", &token));
        CHECK(mliJson_int64_by_token(&json, token + 1, &value));
        CHECK(value == 1337);
        mliJson_free(&json);
}

CASE("mliJson_int64_by_key")
{
        struct mliJson json = mliJson_init();
        char json_str[1024];
        int64_t int_val = 0;
        double dbl_val = 0.0;
        sprintf(json_str, "{\"aaa\": 1337, \"bbb\": 4.2}");
        CHECK(mliJson_malloc_from_cstr(&json, json_str));
        CHECK(mliJson_int64_by_key(&json, 0, &int_val, "aaa"));
        CHECK(mliJson_double_by_key(&json, 0, &dbl_val, "bbb"));
        CHECK(int_val == 1337);
        CHECK(dbl_val == 4.2);
        mliJson_free(&json);
}

CASE("parse mliVec and mliColor")
{
        uint64_t token;
        struct mliJson json = mliJson_init();
        struct mliVec vec1 = mliVec_init(0., 0., 0.);
        struct mliVec vec2 = mliVec_init(0., 0., 0.);
        struct mliColor col = mliColor_set(0., 0., 0.);
        CHECK(mliJson_malloc_from_path(&json, "tests/resources/vec.json"));
        CHECK(mliJson_debug_to_path(&json, "tests/resources/vec.debug.tmp"));

        CHECK(mliJson_token_by_key(&json, 0, "vec1", &token));
        CHECK(mliVec_from_json_token(&vec1, &json, token + 1));
        CHECK_MARGIN(vec1.x, 1.5, 1e-6);
        CHECK_MARGIN(vec1.y, 2.5, 1e-6);
        CHECK_MARGIN(vec1.z, 3.5, 1e-6);

        CHECK(mliJson_token_by_key(&json, 0, "vec2", &token));
        CHECK(mliVec_from_json_token(&vec2, &json, token + 1));
        CHECK_MARGIN(vec2.x, 1.2, 1e-6);
        CHECK_MARGIN(vec2.y, 3.4, 1e-6);
        CHECK_MARGIN(vec2.z, -5.6, 1e-6);

        CHECK(mliJson_token_by_key(&json, 0, "color", &token));
        CHECK(mliColor_from_json_token(&col, &json, token + 1));
        CHECK_MARGIN(col.r, 128., 1e-6);
        CHECK_MARGIN(col.g, 255., 1e-6);
        CHECK_MARGIN(col.b, 12., 1e-6);

        mliJson_free(&json);
}

CASE("rotation representations")
{
        char json_str[1024];
        struct mliQuaternion q;
        struct mliQuaternion q_expected;
        struct mliJson json = mliJson_init();

        /* unity */
        q_expected = mliQuaternion_set_tait_bryan(0., 0., 0.);

        sprintf(json_str,
                "{"
                "\"repr\": \"tait_bryan\", "
                "\"xyz_deg\": [0, 0, 0]"
                "}");
        CHECK(mliJson_malloc_from_cstr(&json, json_str));
        CHECK(mliQuaternion_from_json(&q, &json, 0));
        mliJson_free(&json);
        CHECK(mliQuaternion_equal_margin(q, q_expected, 1e-6));

        sprintf(json_str,
                "{"
                "\"repr\": \"axis_angle\", "
                "\"axis\": [0, 0, 0], "
                "\"angle_deg\": 0."
                "}");
        CHECK(mliJson_malloc_from_cstr(&json, json_str));
        CHECK(mliQuaternion_from_json(&q, &json, 0));
        mliJson_free(&json);
        CHECK(mliQuaternion_equal_margin(q, q_expected, 1e-6));

        sprintf(json_str,
                "{"
                "\"repr\": \"quaternion\", "
                "\"xyz\": [0, 0, 0]"
                "}");
        CHECK(mliJson_malloc_from_cstr(&json, json_str));
        CHECK(mliQuaternion_from_json(&q, &json, 0));
        mliJson_free(&json);
        CHECK(mliQuaternion_equal_margin(q, q_expected, 1e-6));

        /* z-axis, 45deg */
        q_expected = mliQuaternion_set_tait_bryan(0., 0., -mli_deg2rad(45.));

        sprintf(json_str,
                "{"
                "\"repr\": \"tait_bryan\", "
                "\"xyz_deg\": [0, 0, -45]"
                "}");
        CHECK(mliJson_malloc_from_cstr(&json, json_str));
        CHECK(mliQuaternion_from_json(&q, &json, 0));
        mliJson_free(&json);
        CHECK(mliQuaternion_equal_margin(q, q_expected, 1e-6));

        sprintf(json_str,
                "{"
                "\"repr\": \"axis_angle\", "
                "\"axis\": [0, 0, 1], "
                "\"angle_deg\": 45"
                "}");
        CHECK(mliJson_malloc_from_cstr(&json, json_str));
        CHECK(mliQuaternion_from_json(&q, &json, 0));
        mliJson_free(&json);
        CHECK(mliQuaternion_equal_margin(q, q_expected, 1e-6));

        sprintf(json_str,
                "{"
                "\"repr\": \"quaternion\", "
                "\"xyz\": [0, 0, %f]"
                "}",
                q_expected.z);
        CHECK(mliJson_malloc_from_cstr(&json, json_str));
        CHECK(mliQuaternion_from_json(&q, &json, 0));
        mliJson_free(&json);
        CHECK(mliQuaternion_equal_margin(q, q_expected, 1e-6));
}
