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

CASE("mli_Json_cstr_by_token")
{
        struct mli_Json json = mli_Json_init();
        struct mli_String str = mli_String_init();
        char buff[] = "abcde";

        CHECK(mli_String_from_cstr(&str, "{\"hans\": 1337}"));
        CHECK(mli_Json_from_string(&json, &str));
        CHECK(mli_Json_cstr_by_token(&json, 1, buff, 5));
        CHECK(buff[0] == 'h');
        CHECK(buff[1] == 'a');
        CHECK(buff[2] == 'n');
        CHECK(buff[3] == 's');
        CHECK(buff[4] == '\0');

        CHECK(!mli_Json_cstr_by_token(&json, 1, buff, 4));
        mli_Json_free(&json);
        mli_String_free(&str);
}

CASE("mli_Json_init, defaults")
{
        struct mli_Json json = mli_Json_init();
        CHECK(json.raw.size == 0u);
        CHECK(json.raw.array == NULL);
        CHECK(json.num_tokens == 0u);
        CHECK(json.tokens == NULL);
}

CASE("mli_Json_from_io_path")
{
        struct mli_IO file = mli_IO_init();
        struct mli_Json json = mli_Json_init();
        uint64_t return_idx;
        int64_t myint;
        double myfloat;

        CHECK(mli_IO_open_file_cstr(&file, "data/json/example.json", "r"));
        CHECK(mli_Json_from_io(&json, &file));
        mli_IO_close(&file);

        CHECK(mli_IO_open_file_cstr(&file, "data/json/example.debug.tmp", "w"));
        CHECK(mli_Json_debug_to_io(&json, &file));
        mli_IO_close(&file);

        CHECK(mli_Json_token_by_key(&json, 0, "name", &return_idx));
        CHECK(return_idx == 1);
        CHECK(mli_Json_token_by_key(&json, 0, "skill", &return_idx));
        CHECK(return_idx == 3);
        CHECK(mli_Json_double_by_token(&json, 3 + 1, &myfloat));
        CHECK(myfloat == 1.337);
        CHECK(mli_Json_token_by_key(&json, 0, "bug_rate", &return_idx));
        CHECK(return_idx == 5);
        CHECK(mli_Json_int64_by_token(&json, 5 + 1, &myint));
        CHECK(myint == 42);

        CHECK(mli_Json_token_by_key(&json, 0, "some_numbers", &return_idx));
        CHECK(return_idx == 7);
        CHECK(mli_Json_token_by_key(&json, 0, "fair", &return_idx));
        CHECK(return_idx == 13);

        CHECK(mli_Json_token_by_key(&json, 14, "trade", &return_idx));
        CHECK(return_idx == 15);

        CHECK(mli_Json_token_by_key(&json, 14, "uff", &return_idx));
        CHECK(return_idx == 17);

        CHECK(mli_Json_token_by_key(&json, 0, "not_exist", &return_idx) == 0);
        CHECK(mli_Json_token_by_key(&json, 14, "not_exist", &return_idx) == 0);
        mli_Json_free(&json);
}

CASE("mli_Json_malloc_from_string")
{
        struct mli_Json json = mli_Json_init();
        struct mli_String str = mli_String_init();
        uint64_t token = 0u;
        int64_t value = 0;

        CHECK(mli_String_from_cstr(&str, "{\"key\": 1337}"));
        CHECK(mli_Json_from_string(&json, &str));
        CHECK(mli_Json_token_by_key(&json, 0, "key", &token));
        CHECK(mli_Json_int64_by_token(&json, token + 1, &value));
        CHECK(value == 1337);
        mli_Json_free(&json);
        mli_String_free(&str);
}

CASE("mli_Json_int64_by_key")
{
        struct mli_Json json = mli_Json_init();
        struct mli_String str = mli_String_init();
        int64_t int_val = 0;
        double dbl_val = 0.0;

        CHECK(mli_String_from_cstr(&str, "{\"aaa\": 1337, \"bbb\": 4.2}"));
        CHECK(mli_Json_from_string(&json, &str));
        CHECK(mli_Json_int64_by_key(&json, 0, &int_val, "aaa"));
        CHECK(mli_Json_double_by_key(&json, 0, &dbl_val, "bbb"));
        CHECK(int_val == 1337);
        CHECK(dbl_val == 4.2);
        mli_Json_free(&json);
        mli_String_free(&str);
}

CASE("parse mli_Vec")
{
        uint64_t token;
        struct mli_IO io = mli_IO_init();
        struct mli_Json json = mli_Json_init();
        struct mli_Vec vec1 = mli_Vec_set(0., 0., 0.);
        struct mli_Vec vec2 = mli_Vec_set(0., 0., 0.);

        CHECK(mli_IO_open_file_cstr(&io, "data/json/vec.json", "r"));
        CHECK(mli_Json_from_io(&json, &io));
        mli_IO_close(&io);

        CHECK(mli_IO_open_file_cstr(&io, "data/json/vec.debug.tmp", "w"));
        CHECK(mli_Json_debug_to_io(&json, &io));
        mli_IO_close(&io);

        CHECK(mli_Json_token_by_key(&json, 0, "vec1", &token));
        CHECK(mli_Vec_from_json_token(&vec1, &json, token + 1));
        CHECK_MARGIN(vec1.x, 1.5, 1e-6);
        CHECK_MARGIN(vec1.y, 2.5, 1e-6);
        CHECK_MARGIN(vec1.z, 3.5, 1e-6);

        CHECK(mli_Json_token_by_key(&json, 0, "vec2", &token));
        CHECK(mli_Vec_from_json_token(&vec2, &json, token + 1));
        CHECK_MARGIN(vec2.x, 1.2, 1e-6);
        CHECK_MARGIN(vec2.y, 3.4, 1e-6);
        CHECK_MARGIN(vec2.z, -5.6, 1e-6);

        mli_Json_free(&json);
}

CASE("rotation representations")
{
        struct mli_String str = mli_String_init();
        struct mli_Quaternion q;
        struct mli_Quaternion q_expected;
        struct mli_Json json = mli_Json_init();

        /* unity */
        q_expected = mli_Quaternion_set_tait_bryan(0., 0., 0.);

        CHECK(mli_String_from_cstr(
                &str,
                "{"
                "\"repr\": \"tait_bryan\", "
                "\"xyz_deg\": [0, 0, 0]"
                "}"));
        CHECK(mli_Json_from_string(&json, &str));
        CHECK(mli_Quaternion_from_json(&q, &json, 0));
        mli_Json_free(&json);
        CHECK(mli_Quaternion_equal_margin(q, q_expected, 1e-6));

        CHECK(mli_String_from_cstr(
                &str,
                "{"
                "\"repr\": \"axis_angle\", "
                "\"axis\": [0, 0, 0], "
                "\"angle_deg\": 0."
                "}"));
        CHECK(mli_Json_from_string(&json, &str));
        CHECK(mli_Quaternion_from_json(&q, &json, 0));
        mli_Json_free(&json);
        CHECK(mli_Quaternion_equal_margin(q, q_expected, 1e-6));

        CHECK(mli_String_from_cstr(
                &str,
                "{"
                "\"repr\": \"quaternion\", "
                "\"xyz\": [0, 0, 0]"
                "}"));
        CHECK(mli_Json_from_string(&json, &str));
        CHECK(mli_Quaternion_from_json(&q, &json, 0));
        mli_Json_free(&json);
        CHECK(mli_Quaternion_equal_margin(q, q_expected, 1e-6));

        /* z-axis, 45deg */
        q_expected =
                mli_Quaternion_set_tait_bryan(0., 0., -mli_math_deg2rad(45.));

        CHECK(mli_String_from_cstr(
                &str,
                "{"
                "\"repr\": \"tait_bryan\", "
                "\"xyz_deg\": [0, 0, -45]"
                "}"));
        CHECK(mli_Json_from_string(&json, &str));
        CHECK(mli_Quaternion_from_json(&q, &json, 0));
        mli_Json_free(&json);
        CHECK(mli_Quaternion_equal_margin(q, q_expected, 1e-6));

        CHECK(mli_String_from_cstr(
                &str,
                "{"
                "\"repr\": \"axis_angle\", "
                "\"axis\": [0, 0, 1], "
                "\"angle_deg\": 45"
                "}"));
        CHECK(mli_Json_from_string(&json, &str));
        CHECK(mli_Quaternion_from_json(&q, &json, 0));
        mli_Json_free(&json);
        CHECK(mli_Quaternion_equal_margin(q, q_expected, 1e-6));

        CHECK(mli_String_from_cstr_fromat(
                &str,
                "{"
                "\"repr\": \"quaternion\", "
                "\"xyz\": [0, 0, %f]"
                "}",
                q_expected.z));
        CHECK(mli_Json_from_string(&json, &str));
        CHECK(mli_Quaternion_from_json(&q, &json, 0));
        mli_Json_free(&json);
        CHECK(mli_Quaternion_equal_margin(q, q_expected, 1e-6));

        mli_String_free(&str);
}

CASE("mli_JsonWalk")
{
        int64_t val;
        uint64_t SIZE;
        double favl;
        struct mli_Json json = mli_Json_init();
        struct mli_JsonWalk walk = mli_JsonWalk_init();
        struct mli_JsonWalk sidewalk = mli_JsonWalk_init();
        struct mli_String str = mli_String_init();
        struct mli_String payload = mli_String_init();

        CHECK(mli_String_from_cstr(
                &str,
                "{"
                "\"hans\": 1337, "
                "\"peter\": \"bla\", "
                "\"foo\": [42,43,44], "
                "\"rale\": 3.6"
                "}"));
        CHECK(mli_Json_from_string(&json, &str));

        walk = mli_JsonWalk_set(&json);
        CHECK(mli_JsonWalk_to_key(&walk, "hans"));
        CHECK(mli_JsonWalk_get_int64(&walk, &val));
        CHECK(val == 1337);

        walk = mli_JsonWalk_set(&json);
        CHECK(mli_JsonWalk_to_key(&walk, "peter"));
        CHECK(mli_JsonWalk_get_string(&walk, &payload));
        CHECK(mli_String_equal_cstr(&payload, "bla"));

        walk = mli_JsonWalk_set(&json);
        CHECK(mli_JsonWalk_to_key(&walk, "foo"));
        CHECK(mli_JsonWalk_get_array_size(&walk, &SIZE));
        CHECK(SIZE == 3);

        sidewalk = mli_JsonWalk_copy(&walk);
        CHECK(mli_JsonWalk_to_idx(&sidewalk, 1));
        CHECK(mli_JsonWalk_get_int64(&sidewalk, &val));
        CHECK(val == 43);

        sidewalk = mli_JsonWalk_copy(&walk);
        CHECK(mli_JsonWalk_to_idx(&sidewalk, 0));
        CHECK(mli_JsonWalk_get_int64(&sidewalk, &val));
        CHECK(val == 42);

        sidewalk = mli_JsonWalk_copy(&walk);
        CHECK(mli_JsonWalk_to_idx(&sidewalk, 2));
        CHECK(mli_JsonWalk_get_int64(&sidewalk, &val));
        CHECK(val == 44);

        mli_JsonWalk_to_root(&walk);
        CHECK(mli_JsonWalk_to_key(&walk, "rale"));
        CHECK(mli_JsonWalk_get_double(&walk, &favl));
        CHECK_MARGIN(favl, 3.6, 1e-9);

        mli_Json_free(&json);
        mli_String_free(&str);
        mli_String_free(&payload);
}
