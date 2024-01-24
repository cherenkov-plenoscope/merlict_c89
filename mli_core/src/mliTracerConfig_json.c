/* Copyright 2018-2021 Sebastian Achim Mueller */
#include "mliRenderConfig.h"
#include "mliFrame_json.h"
#include "mliAtmosphere_json.h"
#include "../../chk_debug/src/chk_debug.h"

int mliTracerConfig_from_json_token(
        struct mliTracerConfig *tc,
        const struct mliJson *json,
        const uint64_t tkn)
{
        uint64_t bgctkn;
        uint64_t atmtkn;
        uint64_t have_atmosphere;
        chk(mliJson_uint64_by_key(
                json,
                tkn,
                &tc->num_trails_global_light_source,
                "num_trails_global_light_source"));
        chk_msg(tc->num_trails_global_light_source > 0,
                "Expected num_trails_global_light_source > 0.");

        chk(mliJson_uint64_by_key(
                json, tkn, &have_atmosphere, "have_atmosphere"));
        tc->have_atmosphere = (int)have_atmosphere;

        chk(mliJson_token_by_key(json, tkn, "background_color", &bgctkn));
        chk(mliColor_from_json_token(&tc->background_color, json, bgctkn + 1));

        chk(mliJson_token_by_key(json, tkn, "atmosphere", &atmtkn));
        chk(mliAtmosphere_from_json_token(&tc->atmosphere, json, atmtkn + 1));

        return 1;
error:
        return 0;
}
