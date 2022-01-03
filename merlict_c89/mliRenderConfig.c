/* Copyright 2018-2021 Sebastian Achim Mueller */
#include "mliRenderConfig.h"
#include "mliFrame_json.h"

struct mliRenderConfig mliRenderConfig_init(void)
{
        struct mliRenderConfig c;
        c.camera = mliApertureCamera_init();
        c.camera_to_root.translation = mliVec_set(0.0, 0.0, 0.0);
        c.camera_to_root.rotation = mliQuaternion_set_tait_bryan(0.0, 0.0, 0.0);
        c.tracer_config = mliTracerCongig_init();
        c.num_pixel_x = 0;
        c.num_pixel_y = 0;
        c.random_seed = 0;
        return c;
}

int mliRenderConfig_Camera_from_json(
        struct mliRenderConfig *cc,
        const struct mliJson *json,
        const uint64_t tkn)
{
        chk(mliFrame_pos_rot_from_json_token(&cc->camera_to_root, json, tkn));
        chk(mliJson_double_by_key(
                json,
                tkn,
                &cc->camera.image_sensor_width_x,
                "image_sensor_width_x"));
        chk(mliJson_double_by_key(
                json,
                tkn,
                &cc->camera.image_sensor_width_y,
                "image_sensor_width_y"));
        chk(mliJson_double_by_key(
                json, tkn, &cc->camera.focal_length, "focal_length"));
        chk(mliJson_double_by_key(
                json, tkn, &cc->camera.aperture_radius, "aperture_radius"));
        chk(mliJson_double_by_key(
                json,
                tkn,
                &cc->camera.image_sensor_distance,
                "image_sensor_distance"));

        chk_msg(cc->camera.image_sensor_width_x > 0.0,
                "Expected camera.image_sensor_width_x > 0.");
        chk_msg(cc->camera.image_sensor_width_y > 0.0,
                "Expected camera.image_sensor_width_y > 0.");
        chk_msg(cc->camera.focal_length > 0.0,
                "Expected camera.focal_length > 0.");
        chk_msg(cc->camera.aperture_radius > 0.0,
                "Expected camera.aperture_radius > 0.");
        chk_msg(cc->camera.image_sensor_distance > 0.0,
                "Expected camera.image_sensor_distance > 0.");
        chk_msg(cc->camera.image_sensor_distance >= cc->camera.focal_length,
                "Expected "
                "camera.image_sensor_distance >= camera.focal_length.");
        return 1;
error:
        return 0;
}

int mliRenderConfig_Image_from_json(
        struct mliRenderConfig *cc,
        const struct mliJson *json,
        const uint64_t tkn)
{
        chk(mliJson_uint64_by_key(json, tkn, &cc->num_pixel_x, "num_pixel_x"));
        chk(mliJson_uint64_by_key(json, tkn, &cc->num_pixel_y, "num_pixel_y"));
        chk_msg(cc->num_pixel_x > 0, "Expected image.num_pixel_x > 0.");
        chk_msg(cc->num_pixel_y > 0, "Expected image.num_pixel_y > 0.");
        return 1;
error:
        return 0;
}

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
error:
        return 0;
}

int mliRenderConfig_Tracer_from_json(
        struct mliTracerCongig *tc,
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

int mliRenderConfig_from_json(
        struct mliRenderConfig *cc,
        const struct mliJson *json,
        const uint64_t tkn)
{
        uint64_t camtkn;
        uint64_t imgtkn;
        uint64_t tratkn;
        chk(mliJson_uint64_by_key(json, tkn, &cc->random_seed, "random_seed"));
        chk(mliJson_token_by_key_eprint(json, tkn, "camera", &camtkn));
        chk(mliJson_token_by_key_eprint(json, tkn, "image", &imgtkn));
        chk(mliJson_token_by_key_eprint(json, tkn, "tracer", &tratkn));

        chk(mliRenderConfig_Camera_from_json(cc, json, camtkn + 1));
        chk(mliRenderConfig_Image_from_json(cc, json, imgtkn + 1));
        chk(mliRenderConfig_Tracer_from_json(
                &cc->tracer_config, json, tratkn + 1));

        return 1;
error:
        return 0;
}
