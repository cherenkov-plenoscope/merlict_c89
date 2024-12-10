/* Copyright 2019-2020 Sebastian Achim Mueller                                */

CASE("Parse_from_jsonline")
{
        struct mli_Json json = mli_Json_init();
        struct mli_IO ss = mli_IO_init();
        struct mliRenderConfig cc = mliRenderConfig_init();

        mli_IO_open(&ss);
        mli_IO_text_write_cstr_format(&ss, "{");
        mli_IO_text_write_cstr_format(&ss, "    \"camera\": {");
        mli_IO_text_write_cstr_format(&ss, "        \"pos\": [1,2,3],");
        mli_IO_text_write_cstr_format(&ss, "        \"rot\": {");
        mli_IO_text_write_cstr_format(
                &ss, "            \"repr\": \"tait_bryan\", ");
        mli_IO_text_write_cstr_format(
                &ss, "            \"xyz_deg\": [0, 0, 90], ");
        mli_IO_text_write_cstr_format(&ss, "        }, ");
        mli_IO_text_write_cstr_format(&ss, "        \"focal_length\": 50e-3, ");
        mli_IO_text_write_cstr_format(
                &ss, "        \"aperture_radius\": 12.5e-3, ");
        mli_IO_text_write_cstr_format(
                &ss, "        \"image_sensor_distance\": 50e-3, ");
        mli_IO_text_write_cstr_format(
                &ss, "        \"image_sensor_width_x\": 36e-3, ");
        mli_IO_text_write_cstr_format(
                &ss, "        \"image_sensor_width_y\": 24e-3, ");
        mli_IO_text_write_cstr_format(&ss, "    }, ");
        mli_IO_text_write_cstr_format(&ss, "    \"tracer\": {");
        mli_IO_text_write_cstr_format(
                &ss, "        \"num_trails_global_light_source\": 3, ");
        mli_IO_text_write_cstr_format(
                &ss, "        \"background_color\": [110, 120, 130], ");
        mli_IO_text_write_cstr_format(&ss, "        \"have_atmosphere\": 1, ");
        mli_IO_text_write_cstr_format(&ss, "        \"atmosphere\": {");
        mli_IO_text_write_cstr_format(
                &ss, "            \"sunLatitude\": 0.0, ");
        mli_IO_text_write_cstr_format(
                &ss, "            \"sunHourAngle\": 12.0, ");
        mli_IO_text_write_cstr_format(
                &ss, "            \"sunDistance\": 1.5e11, ");
        mli_IO_text_write_cstr_format(&ss, "            \"sunRadius\": 7e8, ");
        mli_IO_text_write_cstr_format(&ss, "            \"altitude\": 2.3e3, ");
        mli_IO_text_write_cstr_format(
                &ss, "            \"earthRadius\": 6360e3, ");
        mli_IO_text_write_cstr_format(
                &ss, "            \"atmosphereRadius\": 6420e3, ");
        mli_IO_text_write_cstr_format(
                &ss, "            \"Height_Rayleigh\": 7994.0, ");
        mli_IO_text_write_cstr_format(
                &ss, "            \"Height_Mie\": 1200.0, ");
        mli_IO_text_write_cstr_format(
                &ss,
                "            \"beta_Rayleigh\": [3.8e-6, 13.5e-6, 33.1e-6], ");
        mli_IO_text_write_cstr_format(
                &ss, "            \"beta_Mie\": [41e-6, 41e-6, 41e-6], ");
        mli_IO_text_write_cstr_format(&ss, "            \"numSamples\": 16, ");
        mli_IO_text_write_cstr_format(
                &ss, "            \"numSamplesLight\": 8, ");
        mli_IO_text_write_cstr_format(&ss, "            \"power\": 3.6e3, ");
        mli_IO_text_write_cstr_format(&ss, "        },");
        mli_IO_text_write_cstr_format(&ss, "    }, ");
        mli_IO_text_write_cstr_format(&ss, "    \"image\": {");
        mli_IO_text_write_cstr_format(&ss, "        \"num_pixel_x\": 192, ");
        mli_IO_text_write_cstr_format(&ss, "        \"num_pixel_y\": 108, ");
        mli_IO_text_write_cstr_format(&ss, "    }, ");
        mli_IO_text_write_cstr_format(&ss, "    \"random_seed\": 1337");
        mli_IO_text_write_cstr_format(&ss, "}");

        CHECK(mli_Json_malloc_from_cstr(&json, (char *)ss.cstr));
        CHECK(mliRenderConfig_from_json(&cc, &json, 0));
        mli_Json_free(&json);
        mli_IO_close(&ss);

        CHECK(cc.random_seed == 1337);

        CHECK(cc.camera_to_root.translation.x == 1.0);
        CHECK(cc.camera_to_root.translation.y == 2.0);
        CHECK(cc.camera_to_root.translation.z == 3.0);

        CHECK(cc.num_pixel_x == 192);
        CHECK(cc.num_pixel_y == 108);

        CHECK(cc.tracer.num_trails_global_light_source = 3);
        CHECK_MARGIN(cc.tracer.background_color.r, 110.0, 0.1);
        CHECK_MARGIN(cc.tracer.background_color.g, 120.0, 0.1);
        CHECK_MARGIN(cc.tracer.background_color.b, 130.0, 0.1);
        CHECK(cc.tracer.have_atmosphere == 1);
}
