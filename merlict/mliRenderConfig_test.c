/* Copyright 2019-2020 Sebastian Achim Mueller                                */

#include "mli_testing.h"
#include "mliRenderConfig.h"

CASE("Parse_from_jsonline")
{
        struct mliJson json = mliJson_init();
        struct mliStr ss = mliStr_init();
        struct mliRenderConfig cc = mliRenderConfig_init();

        mliStr_malloc(&ss);
        mliStr_add_cstr(&ss, "{");
        mliStr_add_cstr(&ss, "    \"camera\": {");
        mliStr_add_cstr(&ss, "        \"pos\": [1,2,3],");
        mliStr_add_cstr(&ss, "        \"rot\": {");
        mliStr_add_cstr(&ss, "            \"repr\": \"tait_bryan\", ");
        mliStr_add_cstr(&ss, "            \"xyz_deg\": [0, 0, 90], ");
        mliStr_add_cstr(&ss, "        }, ");
        mliStr_add_cstr(&ss, "        \"focal_length\": 50e-3, ");
        mliStr_add_cstr(&ss, "        \"aperture_radius\": 12.5e-3, ");
        mliStr_add_cstr(&ss, "        \"image_sensor_distance\": 50e-3, ");
        mliStr_add_cstr(&ss, "        \"image_sensor_width_x\": 36e-3, ");
        mliStr_add_cstr(&ss, "        \"image_sensor_width_y\": 24e-3, ");
        mliStr_add_cstr(&ss, "    }, ");
        mliStr_add_cstr(&ss, "    \"tracer\": {");
        mliStr_add_cstr(&ss, "        \"num_trails_global_light_source\": 3, ");
        mliStr_add_cstr(&ss, "        \"background_color\": [110, 120, 130], ");
        mliStr_add_cstr(&ss, "        \"have_atmosphere\": 1, ");
        mliStr_add_cstr(&ss, "        \"atmosphere\": {");
        mliStr_add_cstr(&ss, "            \"sunLatitude\": 0.0, ");
        mliStr_add_cstr(&ss, "            \"sunHourAngle\": 12.0, ");
        mliStr_add_cstr(&ss, "            \"sunDistance\": 1.5e11, ");
        mliStr_add_cstr(&ss, "            \"sunRadius\": 7e8, ");
        mliStr_add_cstr(&ss, "            \"altitude\": 2.3e3, ");
        mliStr_add_cstr(&ss, "            \"earthRadius\": 6360e3, ");
        mliStr_add_cstr(&ss, "            \"atmosphereRadius\": 6420e3, ");
        mliStr_add_cstr(&ss, "            \"Height_Rayleigh\": 7994.0, ");
        mliStr_add_cstr(&ss, "            \"Height_Mie\": 1200.0, ");
        mliStr_add_cstr(
                &ss,
                "            \"beta_Rayleigh\": [3.8e-6, 13.5e-6, 33.1e-6], ");
        mliStr_add_cstr(
                &ss, "            \"beta_Mie\": [41e-6, 41e-6, 41e-6], ");
        mliStr_add_cstr(&ss, "            \"numSamples\": 16, ");
        mliStr_add_cstr(&ss, "            \"numSamplesLight\": 8, ");
        mliStr_add_cstr(&ss, "            \"power\": 3.6e3, ");
        mliStr_add_cstr(&ss, "        },");
        mliStr_add_cstr(&ss, "    }, ");
        mliStr_add_cstr(&ss, "    \"image\": {");
        mliStr_add_cstr(&ss, "        \"num_pixel_x\": 192, ");
        mliStr_add_cstr(&ss, "        \"num_pixel_y\": 108, ");
        mliStr_add_cstr(&ss, "    }, ");
        mliStr_add_cstr(&ss, "    \"random_seed\": 1337");
        mliStr_add_cstr(&ss, "}");

        CHECK(mliJson_malloc_from_cstr(&json, ss.cstr));
        CHECK(mliRenderConfig_from_json(&cc, &json, 0));
        mliJson_free(&json);
        mliStr_free(&ss);

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