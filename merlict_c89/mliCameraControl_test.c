/* Copyright 2019-2020 Sebastian Achim Mueller                                */

#include "mli_testing.h"
#include "mliCameraControl.h"

CASE("Parse_from_jsonline")
{
        struct mliJson json = mliJson_init();
        struct mliStr ss = mliStr_init();
        struct mliCameraControl cc = mliCameraControl_init();

        mliStr_malloc(&ss, 0);
        mliStr_push_back_c_str(&ss, "{");
        mliStr_push_back_c_str(&ss, "    \"camera\": {");
        mliStr_push_back_c_str(&ss, "        \"pos\": [1,2,3],");
        mliStr_push_back_c_str(&ss, "        \"rot\": {");
        mliStr_push_back_c_str(&ss, "            \"repr\": \"tait_bryan\", ");
        mliStr_push_back_c_str(&ss, "            \"xyz_deg\": [0, 0, 90], ");
        mliStr_push_back_c_str(&ss, "        }, ");
        mliStr_push_back_c_str(&ss, "        \"focal_length\": 50e-3, ");
        mliStr_push_back_c_str(&ss, "        \"aperture_radius\": 12.5e-3, ");
        mliStr_push_back_c_str(&ss, "        \"image_sensor_distance\": 50e-3, ");
        mliStr_push_back_c_str(&ss, "        \"image_sensor_width_x\": 36e-3, ");
        mliStr_push_back_c_str(&ss, "        \"image_sensor_width_y\": 24e-3, ");
        mliStr_push_back_c_str(&ss, "    }, ");
        mliStr_push_back_c_str(&ss, "    \"tracer\": {");
        mliStr_push_back_c_str(&ss, "        \"num_trails_global_light_source\": 3, ");
        mliStr_push_back_c_str(&ss, "        \"background_color\": [110, 120, 130], ");
        mliStr_push_back_c_str(&ss, "        \"have_atmosphere\": 1, ");
        mliStr_push_back_c_str(&ss, "        \"atmosphere\": {");
        mliStr_push_back_c_str(&ss, "            \"sunLatitude\": 0.0, ");
        mliStr_push_back_c_str(&ss, "            \"sunHourAngle\": 12.0, ");
        mliStr_push_back_c_str(&ss, "            \"sunDistance\": 1.5e11, ");
        mliStr_push_back_c_str(&ss, "            \"sunRadius\": 7e8, ");
        mliStr_push_back_c_str(&ss, "            \"altitude\": 2.3e3, ");
        mliStr_push_back_c_str(&ss, "            \"earthRadius\": 6360e3, ");
        mliStr_push_back_c_str(&ss, "            \"atmosphereRadius\": 6420e3, ");
        mliStr_push_back_c_str(&ss, "            \"Height_Rayleigh\": 7994.0, ");
        mliStr_push_back_c_str(&ss, "            \"Height_Mie\": 1200.0, ");
        mliStr_push_back_c_str(&ss, "            \"beta_Rayleigh\": [3.8e-6, 13.5e-6, 33.1e-6], ");
        mliStr_push_back_c_str(&ss, "            \"beta_Mie\": [41e-6, 41e-6, 41e-6], ");
        mliStr_push_back_c_str(&ss, "            \"numSamples\": 16, ");
        mliStr_push_back_c_str(&ss, "            \"numSamplesLight\": 8, ");
        mliStr_push_back_c_str(&ss, "            \"power\": 3.6e3, ");
        mliStr_push_back_c_str(&ss, "        },");
        mliStr_push_back_c_str(&ss, "    }, ");
        mliStr_push_back_c_str(&ss, "    \"image\": {");
        mliStr_push_back_c_str(&ss, "        \"num_pixel_x\": 192, ");
        mliStr_push_back_c_str(&ss, "        \"num_pixel_y\": 108, ");
        mliStr_push_back_c_str(&ss, "    }, ");
        mliStr_push_back_c_str(&ss, "    \"random_seed\": 1337");
        mliStr_push_back_c_str(&ss, "}");

        CHECK(mliJson_malloc_from_c_str(&json, ss.c_str));
        CHECK(mliCameraControl_from_json(&cc, &json, 0));
        mliJson_free(&json);
        mliStr_free(&ss);

        CHECK(cc.random_seed == 1337);

        CHECK(cc.camera_to_root.translation.x == 1.0);
        CHECK(cc.camera_to_root.translation.y == 2.0);
        CHECK(cc.camera_to_root.translation.z == 3.0);

        CHECK(cc.num_pixel_x == 192);
        CHECK(cc.num_pixel_y == 108);

        CHECK(cc.tracer_config.num_trails_global_light_source = 3);
        CHECK_MARGIN(cc.tracer_config.background_color.r, 110.0, 0.1);
        CHECK_MARGIN(cc.tracer_config.background_color.g, 120.0, 0.1);
        CHECK_MARGIN(cc.tracer_config.background_color.b, 130.0, 0.1);
        CHECK(cc.tracer_config.have_atmosphere == 1);
}
