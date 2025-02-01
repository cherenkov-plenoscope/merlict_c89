/* Copyright 2018-2021 Sebastian Achim Mueller */
#include "atmosphere_json.h"
#include "../chk/chk.h"

int mli_Atmosphere_from_json_token(
        struct mli_Atmosphere *atm,
        const struct mli_Json *json,
        const uint64_t tkn)
{
        (*atm) = mli_Atmosphere_init();

        chk(mli_Json_double_by_key(
                json, tkn, &atm->sunLatitude, "sunLatitude"));
        chk(mli_Json_double_by_key(
                json, tkn, &atm->sunHourAngle, "sunHourAngle"));
        mli_Atmosphere_set_sun_direction(
                atm, atm->sunLatitude, atm->sunHourAngle);

        chk(mli_Json_double_by_key(
                json, tkn, &atm->sunDistance, "sunDistance"));
        chk_msg(atm->sunDistance > 0, "Expected atmosphere->sunDistance > 0.");
        chk(mli_Json_double_by_key(json, tkn, &atm->sunRadius, "sunRadius"));
        chk_msg(atm->sunRadius > 0, "Expected atmosphere->sunRadius > 0.");

        chk(mli_Json_double_by_key(
                json, tkn, &atm->earthRadius, "earthRadius"));
        chk_msg(atm->earthRadius > 0, "Expected atmosphere->earthRadius > 0.");
        chk(mli_Json_double_by_key(
                json, tkn, &atm->atmosphereRadius, "atmosphereRadius"));
        chk_msg(atm->atmosphereRadius > atm->earthRadius,
                "Expected atmosphere->atmosphereRadius > atm->earthRadius.");

        chk(mli_Json_double_by_key(
                json, tkn, &atm->Height_Rayleigh, "Height_Rayleigh"));
        chk(mli_Json_double_by_key(json, tkn, &atm->Height_Mie, "Height_Mie"));

        chk(mli_Json_uint64_by_key(json, tkn, &atm->numSamples, "numSamples"));
        chk_msg(atm->numSamples > 0, "Expected atmosphere->numSamples > 0.");
        chk(mli_Json_uint64_by_key(
                json, tkn, &atm->numSamplesLight, "numSamplesLight"));
        chk_msg(atm->numSamplesLight > 0,
                "Expected atmosphere->numSamplesLight > 0.");

        chk(mli_Json_double_by_key(json, tkn, &atm->power, "power"));
        chk_msg(atm->power > 0, "Expected atmosphere->power > 0.");
        chk(mli_Json_double_by_key(json, tkn, &atm->altitude, "altitude"));
        chk_msg(atm->altitude > 0, "Expected atmosphere->altitude > 0.");

        return 1;
chk_error:
        return 0;
}
