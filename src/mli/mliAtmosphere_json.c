/* Copyright 2018-2021 Sebastian Achim Mueller */
#include "mliAtmosphere_json.h"
#include "mliColor_json.h"
#include "../chk/chk.h"

int mliAtmosphere_from_json_token(
        struct mliAtmosphere *atm,
        const struct mliJson *json,
        const uint64_t tkn)
{
        uint64_t beta_rayleigh_tkn;
        uint64_t beta_mie_tkn;

        (*atm) = mliAtmosphere_init();

        chk(mliJson_double_by_key(json, tkn, &atm->sunLatitude, "sunLatitude"));
        chk(mliJson_double_by_key(
                json, tkn, &atm->sunHourAngle, "sunHourAngle"));
        mliAtmosphere_set_sun_direction(
                atm, atm->sunLatitude, atm->sunHourAngle);

        chk(mliJson_double_by_key(json, tkn, &atm->sunDistance, "sunDistance"));
        chk_msg(atm->sunDistance > 0, "Expected atmosphere->sunDistance > 0.");
        chk(mliJson_double_by_key(json, tkn, &atm->sunRadius, "sunRadius"));
        chk_msg(atm->sunRadius > 0, "Expected atmosphere->sunRadius > 0.");

        chk(mliJson_double_by_key(json, tkn, &atm->earthRadius, "earthRadius"));
        chk_msg(atm->earthRadius > 0, "Expected atmosphere->earthRadius > 0.");
        chk(mliJson_double_by_key(
                json, tkn, &atm->atmosphereRadius, "atmosphereRadius"));
        chk_msg(atm->atmosphereRadius > atm->earthRadius,
                "Expected atmosphere->atmosphereRadius > atm->earthRadius.");

        chk(mliJson_double_by_key(
                json, tkn, &atm->Height_Rayleigh, "Height_Rayleigh"));
        chk(mliJson_double_by_key(json, tkn, &atm->Height_Mie, "Height_Mie"));

        chk(mliJson_uint64_by_key(json, tkn, &atm->numSamples, "numSamples"));
        chk_msg(atm->numSamples > 0, "Expected atmosphere->numSamples > 0.");
        chk(mliJson_uint64_by_key(
                json, tkn, &atm->numSamplesLight, "numSamplesLight"));
        chk_msg(atm->numSamplesLight > 0,
                "Expected atmosphere->numSamplesLight > 0.");

        chk(mliJson_token_by_key(
                json, tkn, "beta_Rayleigh", &beta_rayleigh_tkn));
        chk(mliColor_from_json_token(
                &atm->beta_Rayleigh, json, beta_rayleigh_tkn + 1));
        chk_msg(atm->beta_Rayleigh.r > 0.0,
                "Expected atmosphere->beta_Rayleigh.r > 0.");
        chk_msg(atm->beta_Rayleigh.g > 0.0,
                "Expected atmosphere->beta_Rayleigh.g > 0.");
        chk_msg(atm->beta_Rayleigh.b > 0.0,
                "Expected atmosphere->beta_Rayleigh.b > 0.");

        chk(mliJson_token_by_key(json, tkn, "beta_Mie", &beta_mie_tkn));
        chk(mliColor_from_json_token(&atm->beta_Mie, json, beta_mie_tkn + 1));
        chk_msg(atm->beta_Mie.r > 0.0, "Expected atmosphere->beta_Mie.r > 0.");
        chk_msg(atm->beta_Mie.g > 0.0, "Expected atmosphere->beta_Mie.g > 0.");
        chk_msg(atm->beta_Mie.b > 0.0, "Expected atmosphere->beta_Mie.b > 0.");

        chk(mliJson_double_by_key(json, tkn, &atm->power, "power"));
        chk_msg(atm->power > 0, "Expected atmosphere->power > 0.");
        chk(mliJson_double_by_key(json, tkn, &atm->altitude, "altitude"));
        chk_msg(atm->altitude > 0, "Expected atmosphere->altitude > 0.");

        return 1;
chk_error:
        return 0;
}
