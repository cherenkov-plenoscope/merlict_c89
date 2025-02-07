/* Copyright 2018-2021 Sebastian Achim Mueller */
#include "pathtracer_config_json.h"
#include "../frame/frame_json.h"
#include "../atmosphere/atmosphere_json.h"

chk_rc mli_pathtracer_Config_from_json_token(
        struct mli_pathtracer_Config *tc,
        const struct mli_Json *json,
        const uint64_t tkn)
{
        uint64_t atmtkn;
        uint64_t have_atmosphere;
        chk(mli_Json_uint64_by_key(
                json,
                tkn,
                &tc->num_trails_global_light_source,
                "num_trails_global_light_source"));
        chk_msg(tc->num_trails_global_light_source > 0,
                "Expected num_trails_global_light_source > 0.");

        chk(mli_Json_uint64_by_key(
                json, tkn, &have_atmosphere, "have_atmosphere"));
        tc->have_atmosphere = (int)have_atmosphere;

        chk(mli_Json_token_by_key(json, tkn, "atmosphere", &atmtkn));
        chk(mli_Atmosphere_from_json_token(&tc->atmosphere, json, atmtkn + 1));

        return CHK_SUCCESS;
chk_error:
        return CHK_FAIL;
}
