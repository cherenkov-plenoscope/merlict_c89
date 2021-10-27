/* Copyright 2021 Sebastian A. Mueller */

CASE("EventTape: run_normal")
{
        const uint64_t random_seed = 42u;
        const uint64_t BUFFER_SIZE = 64u;
        const uint64_t NUM_EVENTS = 6u;
        const uint64_t NUM_BUNCHES[] = {23, 65, 0, 9, 7, 0};
        const float EVENT_NUMBERS[] = {1, 2, 3, 4, 5, 6};
        uint64_t e, b;
        struct mliEventTapeWriter taro = mliEventTapeWriter_init();
        struct mliEventTapeReader tari = mliEventTapeReader_init();
        float corh[273] = {0.0};
        struct mliCorsikaPhotonBunch bunch;
        struct mliCorsikaPhotonBunch bunch_expected;
        struct mliPrng prng = mliPrng_init_PCG32(random_seed);
        const char path[] = "merlict_c89/"
                            "mli_corsika_test_resources/"
                            "run_normal.tar";

        /* write */
        /* ----- */
        CHECK(mliEventTapeWriter_open(&taro, path, BUFFER_SIZE));
        corh[0] = mli_4chars_to_float("RUNH");
        CHECK(mliEventTapeWriter_write_runh(&taro, corh));

        for (e = 0; e < NUM_EVENTS; e++) {
                corh[0] = mli_4chars_to_float("EVTH");
                corh[MLI_CORSIKA_EVTH_EVENT_NUMBER] = EVENT_NUMBERS[e];
                CHECK(mliEventTapeWriter_write_evth(&taro, corh));
                for (b = 0; b < NUM_BUNCHES[e]; b++) {
                        mliEventTape_testing_set_random_bunch(&bunch, &prng);
                        CHECK(mliEventTapeWriter_write_cherenkov_bunch(
                                &taro, &bunch));
                }
        }
        CHECK(mliEventTapeWriter_close(&taro));

        /* read back */
        /* --------- */
        mliPrng_reinit(&prng, random_seed);

        CHECK(mliEventTapeReader_open(&tari, path));
        CHECK(mliEventTapeReader_read_runh(&tari, corh));

        /* work on RUNH */
        CHECK(corh[0] == mli_4chars_to_float("RUNH"));

        while (mliEventTapeReader_read_evth(&tari, corh)) {
                /* work on EVTH */
                CHECK(corh[0] == mli_4chars_to_float("EVTH"));
                CHECK(corh[MLI_CORSIKA_EVTH_EVENT_NUMBER] == tari.event_number);

                while (mliEventTapeReader_read_cherenkov_bunch(&tari, &bunch)) {
                        /* work on photon bunch */
                        mliEventTape_testing_set_random_bunch(
                                &bunch_expected, &prng);
                        CHECK(mliEventTape_testing_bunches_are_equal(
                                &bunch, &bunch_expected));
                }
                CHECK(!mliEventTapeReader_read_cherenkov_bunch(&tari, &bunch));
        }
        CHECK(!mliEventTapeReader_read_evth(&tari, corh));
        CHECK(mliEventTapeReader_close(&tari));
}

CASE("EventTape: run_no_events")
{
        const char path[] = "merlict_c89/"
                            "mli_corsika_test_resources/"
                            "run_no_events.tar";
        const uint64_t num_events = 0;
        const uint64_t buffer_size = 128;
        const float *event_numbers;
        const uint64_t *num_bunches;
        const uint32_t random_seed = 0;

        CHECK(mliEventTape_testing_write_and_read(
                path,
                num_events,
                buffer_size,
                event_numbers,
                num_bunches,
                random_seed));
}

CASE("EventTape: run_one_event_no_bunches")
{
        const char path[] = "merlict_c89/"
                            "mli_corsika_test_resources/"
                            "run_one_event_no_bunches.tar";
        const uint64_t num_events = 1;
        const uint64_t buffer_size = 128;
        const float event_numbers[] = {1.0f};
        const uint64_t num_bunches[] = {0};
        const uint32_t random_seed = 0;

        CHECK(mliEventTape_testing_write_and_read(
                path,
                num_events,
                buffer_size,
                event_numbers,
                num_bunches,
                random_seed));
}

CASE("EventTape: run_one_event_few_bunches")
{
        const char path[] = "merlict_c89/"
                            "mli_corsika_test_resources/"
                            "run_one_event_few_bunches.tar";
        const uint64_t num_events = 1;
        const uint64_t buffer_size = 128;
        const float event_numbers[] = {1.0f};
        const uint64_t num_bunches[] = {120};
        const uint32_t random_seed = 0;

        CHECK(mliEventTape_testing_write_and_read(
                path,
                num_events,
                buffer_size,
                event_numbers,
                num_bunches,
                random_seed));
}

CASE("EventTape: run_one_event_num_bunches_equals_buffer")
{
        const char path[] = "merlict_c89/"
                            "mli_corsika_test_resources/"
                            "run_one_event_num_bunches_equals_buffer.tar";
        const uint64_t num_events = 1;
        const uint64_t buffer_size = 128;
        const float event_numbers[] = {1.0f};
        const uint64_t num_bunches[] = {128};
        const uint32_t random_seed = 0;

        CHECK(mliEventTape_testing_write_and_read(
                path,
                num_events,
                buffer_size,
                event_numbers,
                num_bunches,
                random_seed));
}

CASE("EventTape: run_one_event_many_bunches")
{
        const char path[] = "merlict_c89/"
                            "mli_corsika_test_resources/"
                            "run_one_event_many_bunches.tar";
        const uint64_t num_events = 1;
        const uint64_t buffer_size = 128;
        const float event_numbers[] = {1.0f};
        const uint64_t num_bunches[] = {150};
        const uint32_t random_seed = 0;

        CHECK(mliEventTape_testing_write_and_read(
                path,
                num_events,
                buffer_size,
                event_numbers,
                num_bunches,
                random_seed));
}

CASE("EventTape: run_many_events_first_event_no_bunches")
{
        const char path[] = "merlict_c89/"
                            "mli_corsika_test_resources/"
                            "run_many_events_first_event_no_bunches.tar";
        const uint64_t num_events = 5;
        const uint64_t buffer_size = 128;
        const float event_numbers[] = {1, 2, 3, 4, 5};
        const uint64_t num_bunches[] = {0, 200, 30, 10, 18};
        const uint32_t random_seed = 0;

        CHECK(mliEventTape_testing_write_and_read(
                path,
                num_events,
                buffer_size,
                event_numbers,
                num_bunches,
                random_seed));
}

CASE("EventTape: run_many_events_last_event_no_bunches")
{
        const char path[] = "merlict_c89/"
                            "mli_corsika_test_resources/"
                            "run_many_events_last_event_no_bunches.tar";
        const uint64_t num_events = 5;
        const uint64_t buffer_size = 128;
        const float event_numbers[] = {1, 2, 3, 4, 5};
        const uint64_t num_bunches[] = {18, 200, 30, 10, 0};
        const uint32_t random_seed = 0;

        CHECK(mliEventTape_testing_write_and_read(
                path,
                num_events,
                buffer_size,
                event_numbers,
                num_bunches,
                random_seed));
}

CASE("EventTape: run_many_events_all_events_many_bunches")
{
        const char path[] = "merlict_c89/"
                            "mli_corsika_test_resources/"
                            "run_many_events_all_events_many_bunches.tar";
        const uint64_t num_events = 5;
        const uint64_t buffer_size = 128;
        const float event_numbers[] = {1, 2, 3, 4, 5};
        const uint64_t num_bunches[] = {260, 200, 129, 500, 220};
        const uint32_t random_seed = 0;

        CHECK(mliEventTape_testing_write_and_read(
                path,
                num_events,
                buffer_size,
                event_numbers,
                num_bunches,
                random_seed));
}

CASE("EventTape: run_many_events_all_events_no_bunches")
{
        const char path[] = "merlict_c89/"
                            "mli_corsika_test_resources/"
                            "run_many_events_all_events_no_bunches.tar";
        const uint64_t num_events = 5;
        const uint64_t buffer_size = 128;
        const float event_numbers[] = {1, 2, 3, 4, 5};
        const uint64_t num_bunches[] = {0, 0, 0, 0, 0};
        const uint32_t random_seed = 0;

        CHECK(mliEventTape_testing_write_and_read(
                path,
                num_events,
                buffer_size,
                event_numbers,
                num_bunches,
                random_seed));
}

CASE("EventTape: run_many_events_all_events_num_bunches_equal_buffer")
{
        const char path[] =
                "merlict_c89/"
                "mli_corsika_test_resources/"
                "run_many_events_all_events_num_bunches_equal_buffer.tar";
        const uint64_t num_events = 5;
        const uint64_t buffer_size = 128;
        const float event_numbers[] = {1, 2, 3, 4, 5};
        const uint64_t num_bunches[] = {128, 128, 128, 128, 128};
        const uint32_t random_seed = 0;

        CHECK(mliEventTape_testing_write_and_read(
                path,
                num_events,
                buffer_size,
                event_numbers,
                num_bunches,
                random_seed));
}

CASE("EventTape: run_many_events_event_numbers_do_not_start_at_one")
{
        const char path[] =
                "merlict_c89/"
                "mli_corsika_test_resources/"
                "run_many_events_event_numbers_do_not_start_at_one.tar";
        const uint64_t num_events = 5;
        const uint64_t buffer_size = 128;
        const float event_numbers[] = {3, 4, 5, 6, 7};
        const uint64_t num_bunches[] = {110, 105, 244, 191, 131};
        const uint32_t random_seed = 0;

        CHECK(mliEventTape_testing_write_and_read(
                path,
                num_events,
                buffer_size,
                event_numbers,
                num_bunches,
                random_seed));
}

CASE("EventTape: run_many_events_buffer_set_to_one")
{
        const char path[] = "merlict_c89/"
                            "mli_corsika_test_resources/"
                            "run_many_events_buffer_set_to_one.tar";
        const uint64_t num_events = 5;
        const uint64_t buffer_size = 1;
        const float event_numbers[] = {3, 4, 5, 6, 7};
        const uint64_t num_bunches[] = {5, 6, 10, 19, 0};
        const uint32_t random_seed = 0;

        CHECK(mliEventTape_testing_write_and_read(
                path,
                num_events,
                buffer_size,
                event_numbers,
                num_bunches,
                random_seed));
}
