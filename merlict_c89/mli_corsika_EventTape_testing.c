/* Copyright 2020 Sebastian A. Mueller */
#include "mli_corsika_EventTape_testing.h"

void mliEventTape_testing_set_random_RUNH(float *runh, struct mliPrng *prng)
{
        uint64_t i;
        runh[0] = mli_4chars_to_float("RUNH");
        for (i = 0; i < 273; i++) {
                if (i != 0) {
                        runh[i] = (float)mli_random_uniform(prng);
                }
        }
}

void mliEventTape_testing_set_random_EVTH(
        float *evth,
        const float event_number,
        struct mliPrng *prng)
{
        uint64_t i;
        evth[0] = mli_4chars_to_float("EVTH");
        evth[MLI_CORSIKA_EVTH_EVENT_NUMBER] = event_number;
        for (i = 0; i < 273; i++) {
                if (i != 0 && i != MLI_CORSIKA_EVTH_EVENT_NUMBER) {
                        evth[i] = (float)mli_random_uniform(prng);
                }
        }
}

void mliEventTape_testing_set_random_bunch(
        struct mliCorsikaPhotonBunch *bunch,
        struct mliPrng *prng)
{
        bunch->x_cm = (float)mli_random_uniform(prng);
        bunch->y_cm = (float)mli_random_uniform(prng);
        bunch->cx_rad = (float)mli_random_uniform(prng);
        bunch->cy_rad = (float)mli_random_uniform(prng);
        bunch->time_ns = (float)mli_random_uniform(prng);
        bunch->z_emission_cm = (float)mli_random_uniform(prng);
        bunch->weight_photons = (float)mli_random_uniform(prng);
        bunch->wavelength_nm = (float)mli_random_uniform(prng);
}

int mliEventTape_testing_bunches_are_equal(
        struct mliCorsikaPhotonBunch *b1,
        struct mliCorsikaPhotonBunch *b2)
{
        if (b1->x_cm != b2->x_cm) {
                fprintf(stderr, "Bunch missmatch x_cm.\n");
                return 0;
        }
        if (b1->y_cm != b2->y_cm) {
                fprintf(stderr, "Bunch missmatch y_cm.\n");
                return 0;
        }
        if (b1->cx_rad != b2->cx_rad) {
                fprintf(stderr, "Bunch missmatch cx_rad.\n");
                return 0;
        }
        if (b1->cy_rad != b2->cy_rad) {
                fprintf(stderr, "Bunch missmatch cy_rad.\n");
                return 0;
        }
        if (b1->time_ns != b2->time_ns) {
                fprintf(stderr, "Bunch missmatch time_ns.\n");
                return 0;
        }
        if (b1->z_emission_cm != b2->z_emission_cm) {
                fprintf(stderr, "Bunch missmatch z_emission_cm.\n");
                return 0;
        }
        if (b1->weight_photons != b2->weight_photons) {
                fprintf(stderr, "Bunch missmatch weight_photons.\n");
                return 0;
        }
        if (b1->wavelength_nm != b2->wavelength_nm) {
                fprintf(stderr, "Bunch missmatch wavelength_nm.\n");
                return 0;
        }
        return 1;
}

int mliEventTape_testing_corsika_headers_are_equal(
        const float *h1,
        const float *h2)
{
        int i;
        for (i = 0; i < 273; i++) {
                if (h1[i] != h2[i]) {
                        fprintf(stderr, "Corsika-header missmatch at %d.\n", i);
                        return 0;
                }
        }
        return 1;
}

int mliEventTape_testing_write_and_read(
        const char *path,
        const uint64_t num_events,
        const uint64_t buffer_size,
        const float *event_numbers,
        const uint64_t *num_bunches,
        const uint32_t random_seed)
{
        uint64_t e, b;
        float corho[273] = {0.0};
        float corhi[273] = {0.0};
        struct mliCorsikaPhotonBunch buncho;
        struct mliCorsikaPhotonBunch bunchi;

        struct mliEventTapeWriter taro = mliEventTapeWriter_init();
        struct mliEventTapeReader tari = mliEventTapeReader_init();

        struct mliPrng prng = mliPrng_init_PCG32(random_seed);
        mliPrng_reinit(&prng, random_seed);

        /* write RUN */
        /* ========= */
        chk_msg(mliEventTapeWriter_open(&taro, path, buffer_size),
                "Can't open writer.");
        /* set RUNH */
        mliEventTape_testing_set_random_RUNH(corho, &prng);
        chk_msg(mliEventTapeWriter_write_runh(&taro, corho),
                "Can't write RUNH.");

        for (e = 0; e < num_events; e++) {
                /* set EVTH */
                mliEventTape_testing_set_random_EVTH(
                        corho, event_numbers[e], &prng);
                chk_msg(mliEventTapeWriter_write_evth(&taro, corho),
                        "Can't write EVTH.");
                for (b = 0; b < num_bunches[e]; b++) {
                        mliEventTape_testing_set_random_bunch(&buncho, &prng);
                        chk_msg(mliEventTapeWriter_write_cherenkov_bunch(
                                        &taro, buncho),
                                "Can't write bunch.");
                }
        }

        chk_msg(mliEventTapeWriter_close(&taro), "Can't close writer.");

        /* read RUN */
        /* ======== */
        mliPrng_reinit(&prng, random_seed);

        chk_msg(mliEventTapeReader_open(&tari, path), "Can't open reader.");

        /* check RUNH */
        mliEventTape_testing_set_random_RUNH(corho, &prng);
        chk_msg(mliEventTapeReader_read_runh(&tari, corhi), "Can't read RUNH.");
        chk_msg(mliEventTape_testing_corsika_headers_are_equal(corho, corhi),
                "Expected RUNH to be equal.");

        chk_msg(tari.event_number == 0,
                "Expected reader's event-number == 0 "
                "before first EVTH is read.");

        for (e = 0; e < num_events; e++) {
                chk_msg(mliEventTapeReader_read_evth(&tari, corhi),
                        "Can't read EVTH.");
                /* check EVTH */
                mliEventTape_testing_set_random_EVTH(
                        corho, event_numbers[e], &prng);
                chk_msg(mliEventTape_testing_corsika_headers_are_equal(
                                corho, corhi),
                        "Expected EVTH to be equal.");
                chk_msg(corhi[MLI_CORSIKA_EVTH_EVENT_NUMBER] ==
                                event_numbers[e],
                        "Expected reader's event-number to match last EVTH.");
                chk_msg(tari.event_number == event_numbers[e],
                        "Expected a different event-number.");

                for (b = 0; b < num_bunches[e]; b++) {
                        chk_msg(mliEventTapeReader_read_cherenkov_bunch(
                                        &tari, &bunchi),
                                "Can't read bunch.");
                        mliEventTape_testing_set_random_bunch(&buncho, &prng);
                        chk_msg(mliEventTape_testing_bunches_are_equal(
                                        &bunchi, &buncho),
                                "Expected bunch to be equal.");
                }
                chk_msg(!mliEventTapeReader_read_cherenkov_bunch(
                                &tari, &buncho),
                        "Did not expect another cherenkov-bunch.");
        }
        chk_msg(!mliEventTapeReader_read_evth(&tari, corho),
                "Did not expect another EVTH.");

        chk_msg(mliEventTapeReader_close(&tari), "Can't close reader.");
        return 1;
error:
        return 0;
}
