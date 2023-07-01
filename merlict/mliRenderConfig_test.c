/* Copyright 2019-2020 Sebastian Achim Mueller                                */

#include "mli_testing.h"
#include "mliRenderConfig.h"

CASE("Parse_from_jsonline")
{
        struct mliJson json = mliJson_init();
        struct mliIo ss = mliIo_init();
        struct mliRenderConfig cc = mliRenderConfig_init();

        mliIo_malloc(&ss);
        mliIo_printf(&ss, "{");
        mliIo_printf(&ss, "    \"camera\": {");
        mliIo_printf(&ss, "        \"pos\": [1,2,3],");
        mliIo_printf(&ss, "        \"rot\": {");
        mliIo_printf(&ss, "            \"repr\": \"tait_bryan\", ");
        mliIo_printf(&ss, "            \"xyz_deg\": [0, 0, 90], ");
        mliIo_printf(&ss, "        }, ");
        mliIo_printf(&ss, "        \"focal_length\": 50e-3, ");
        mliIo_printf(&ss, "        \"aperture_radius\": 12.5e-3, ");
        mliIo_printf(&ss, "        \"image_sensor_distance\": 50e-3, ");
        mliIo_printf(&ss, "        \"image_sensor_width_x\": 36e-3, ");
        mliIo_printf(&ss, "        \"image_sensor_width_y\": 24e-3, ");
        mliIo_printf(&ss, "    }, ");
        mliIo_printf(&ss, "    \"tracer\": {");
        mliIo_printf(&ss, "        \"num_trails_global_light_source\": 3, ");
        mliIo_printf(&ss, "        \"background_color\": [110, 120, 130], ");
        mliIo_printf(&ss, "        \"have_atmosphere\": 1, ");
        mliIo_printf(&ss, "        \"atmosphere\": {");
        mliIo_printf(&ss, "            \"sunLatitude\": 0.0, ");
        mliIo_printf(&ss, "            \"sunHourAngle\": 12.0, ");
        mliIo_printf(&ss, "            \"sunDistance\": 1.5e11, ");
        mliIo_printf(&ss, "            \"sunRadius\": 7e8, ");
        mliIo_printf(&ss, "            \"altitude\": 2.3e3, ");
        mliIo_printf(&ss, "            \"earthRadius\": 6360e3, ");
        mliIo_printf(&ss, "            \"atmosphereRadius\": 6420e3, ");
        mliIo_printf(&ss, "            \"Height_Rayleigh\": 7994.0, ");
        mliIo_printf(&ss, "            \"Height_Mie\": 1200.0, ");
        mliIo_printf(
                &ss,
                "            \"beta_Rayleigh\": [3.8e-6, 13.5e-6, 33.1e-6], ");
        mliIo_printf(
                &ss, "            \"beta_Mie\": [41e-6, 41e-6, 41e-6], ");
        mliIo_printf(&ss, "            \"numSamples\": 16, ");
        mliIo_printf(&ss, "            \"numSamplesLight\": 8, ");
        mliIo_printf(&ss, "            \"power\": 3.6e3, ");
        mliIo_printf(&ss, "        },");
        mliIo_printf(&ss, "    }, ");
        mliIo_printf(&ss, "    \"image\": {");
        mliIo_printf(&ss, "        \"num_pixel_x\": 192, ");
        mliIo_printf(&ss, "        \"num_pixel_y\": 108, ");
        mliIo_printf(&ss, "    }, ");
        mliIo_printf(&ss, "    \"random_seed\": 1337");
        mliIo_printf(&ss, "}");

        CHECK(mliJson_malloc_from_cstr(&json, (char *)ss.cstr));
        CHECK(mliRenderConfig_from_json(&cc, &json, 0));
        mliJson_free(&json);
        mliIo_free(&ss);

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
