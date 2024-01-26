/* Copyright 2020 Sebastian A. Mueller */
#include "mli_corsika_EventTape_testing.h"
#include "../../mli_core/src/chk.h"

void mliEventTape_testing_set_random_corsika_header(
        float *head,
        struct mliPrng *prng)
{
        uint64_t i;
        for (i = 0; i < 273; i++) {
                head[i] = (float)mli_random_uniform(prng);
        }
}

void mliEventTape_testing_set_random_RUNH(
        float *runh,
        const float run_number,
        struct mliPrng *prng)
{
        mliEventTape_testing_set_random_corsika_header(runh, prng);
        runh[0] = mli_chars_to_float("RUNH");
        runh[MLI_CORSIKA_RUNH_RUN_NUMBER] = run_number;
}

void mliEventTape_testing_set_random_EVTH(
        float *evth,
        const float event_number,
        const float run_number,
        struct mliPrng *prng)
{
        mliEventTape_testing_set_random_corsika_header(evth, prng);
        evth[0] = mli_chars_to_float("EVTH");
        evth[MLI_CORSIKA_EVTH_EVENT_NUMBER] = event_number;
        evth[MLI_CORSIKA_EVTH_RUN_NUMBER] = run_number;
}

void mliEventTape_testing_set_random_bunch(float *bunch, struct mliPrng *prng)
{
        bunch[0] = (float)mli_random_uniform(prng);
        bunch[1] = (float)mli_random_uniform(prng);
        bunch[2] = (float)mli_random_uniform(prng);
        bunch[3] = (float)mli_random_uniform(prng);
        bunch[4] = (float)mli_random_uniform(prng);
        bunch[5] = (float)mli_random_uniform(prng);
        bunch[6] = (float)mli_random_uniform(prng);
        bunch[7] = (float)mli_random_uniform(prng);
}

int mliEventTape_testing_bunches_are_equal(float *b1, float *b2)
{
        if (b1[0] != b2[0]) {
                fprintf(stderr, "Bunch missmatch x_cm.\n");
                return 0;
        }
        if (b1[1] != b2[1]) {
                fprintf(stderr, "Bunch missmatch y_cm.\n");
                return 0;
        }
        if (b1[2] != b2[2]) {
                fprintf(stderr, "Bunch missmatch cx_rad.\n");
                return 0;
        }
        if (b1[3] != b2[3]) {
                fprintf(stderr, "Bunch missmatch cy_rad.\n");
                return 0;
        }
        if (b1[4] != b2[4]) {
                fprintf(stderr, "Bunch missmatch time_ns.\n");
                return 0;
        }
        if (b1[5] != b2[5]) {
                fprintf(stderr, "Bunch missmatch z_emission_cm.\n");
                return 0;
        }
        if (b1[6] != b2[6]) {
                fprintf(stderr, "Bunch missmatch weight_photons.\n");
                return 0;
        }
        if (b1[7] != b2[7]) {
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
        float buncho[8] = {0.0};
        float bunchi[8] = {0.0};

        FILE *ostream = NULL;
        FILE *istream = NULL;
        struct mliEventTapeWriter taro = mliEventTapeWriter_init();
        struct mliEventTapeReader tari = mliEventTapeReader_init();

        struct mliPrng prng = mliPrng_init_PCG32(random_seed);
        mliPrng_reinit(&prng, random_seed);

        /* write RUN */
        /* ========= */
        ostream = fopen(path, "wb");
        chk_msg(mliEventTapeWriter_begin(&taro, ostream, buffer_size),
                "Can't begin writer.");
        /* set RUNH */
        mliEventTape_testing_set_random_RUNH(corho, 18.0, &prng);
        chk_msg(mliEventTapeWriter_write_runh(&taro, corho),
                "Can't write RUNH.");

        for (e = 0; e < num_events; e++) {
                /* set EVTH */
                mliEventTape_testing_set_random_EVTH(
                        corho, event_numbers[e], 18.0, &prng);
                chk_msg(mliEventTapeWriter_write_evth(&taro, corho),
                        "Can't write EVTH.");
                for (b = 0; b < num_bunches[e]; b++) {
                        mliEventTape_testing_set_random_bunch(buncho, &prng);
                        chk_msg(mliEventTapeWriter_write_cherenkov_bunch(
                                        &taro, buncho),
                                "Can't write bunch.");
                }
        }

        chk_msg(mliEventTapeWriter_finalize(&taro), "Can't finalize writer.");
        fclose(ostream);

        /* read RUN */
        /* ======== */
        mliPrng_reinit(&prng, random_seed);

        istream = fopen(path, "rb");
        chk_msg(mliEventTapeReader_begin(&tari, istream),
                "Can't begin reader.");

        /* check RUNH */
        mliEventTape_testing_set_random_RUNH(corho, 18.0, &prng);
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
                        corho, event_numbers[e], 18.0, &prng);
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
                                        &tari, bunchi),
                                "Can't read bunch.");
                        mliEventTape_testing_set_random_bunch(buncho, &prng);
                        chk_msg(mliEventTape_testing_bunches_are_equal(
                                        bunchi, buncho),
                                "Expected bunch to be equal.");
                }
                chk_msg(!mliEventTapeReader_read_cherenkov_bunch(&tari, buncho),
                        "Did not expect another cherenkov-bunch.");
        }
        chk_msg(!mliEventTapeReader_read_evth(&tari, corho),
                "Did not expect another EVTH.");

        chk_msg(mliEventTapeReader_finalize(&tari), "Can't finalize reader.");
        fclose(istream);
        return 1;
error:
        return 0;
}
