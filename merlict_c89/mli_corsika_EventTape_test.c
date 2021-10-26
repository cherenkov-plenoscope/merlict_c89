/* Copyright 2021 Sebastian A. Mueller */

CASE("EventTape: run normal")
{
        const uint64_t BUFF_NUM = 64u;
        const uint64_t NUM_EVENTS = 6u;
        const uint64_t NUM_BUNCHES[] = {23, 65, 0, 9, 7, 0};
        const float EVENT_NUMBERS[] = {1, 2, 3, 4, 5, 6};
        uint64_t e, b;
        struct mliEventTapeWriter taro = mliEventTapeWriter_init();
        struct mliEventTapeReader tari = mliEventTapeReader_init();
        float corh[273] = {0.0};
        struct mliCorsikaPhotonBunch bunch;

        /* write */
        /* ----- */
        CHECK(mliEventTapeWriter_open(
                &taro,
                "merlict_c89/"
                "mli_corsika_test_resources/"
                "run_normal.tar",
                BUFF_NUM));
        corh[0] = mli_4chars_to_float("RUNH");
        CHECK(mliEventTapeWriter_add_runh(&taro, corh));

        for (e = 0; e < NUM_EVENTS; e++) {
                corh[0] = mli_4chars_to_float("EVTH");
                corh[MLI_CORSIKA_EVTH_EVENT_NUMBER] = EVENT_NUMBERS[e];
                CHECK(mliEventTapeWriter_add_evth(&taro, corh));
                for (b = 0; b < NUM_BUNCHES[e]; b++) {
                        mliCorsikaPhotonBunch_testing_mark_bunch(&bunch, b);
                        CHECK(mliEventTapeWriter_add_cherenkov_bunch(&taro, bunch));
                }
        }
        CHECK(mliEventTapeWriter_close(&taro));

        /* read back */
        /* --------- */
        CHECK(mliEventTapeReader_open(
                &tari,
                "merlict_c89/"
                "mli_corsika_test_resources/"
                "run_normal.tar"));
        CHECK(mliEventTapeReader_read_runh(&tari, corh));
        corh[0] = mli_4chars_to_float("RUNH");

        e = 0;
        CHECK(tari.event_number == 0);
        for (e = 0; e < NUM_EVENTS; e++) {
                CHECK(mliEventTapeReader_read_evth(&tari, corh));
                CHECK(tari.event_number == EVENT_NUMBERS[e]);
                CHECK(corh[0] == mli_4chars_to_float("EVTH"));
                CHECK(corh[MLI_CORSIKA_EVTH_EVENT_NUMBER] == EVENT_NUMBERS[e]);
                for (b = 0; b < NUM_BUNCHES[e]; b++) {
                        CHECK(mliEventTapeReader_read_cherenkov_bunch(
                                &tari, &bunch));
                        CHECK(mliCorsikaPhotonBunch_testing_bunch_has_mark(
                                bunch, b));
                }
                CHECK(!mliEventTapeReader_read_cherenkov_bunch(&tari, &bunch));
        }
        CHECK(!mliEventTapeReader_read_evth(&tari, corh));
        CHECK(mliEventTapeReader_close(&tari));
}

CASE("EventTape: no events")
{
        struct mliEventTapeWriter taro = mliEventTapeWriter_init();
        struct mliEventTapeReader tari = mliEventTapeReader_init();
        float corh[273] = {0.0};
        float bunch[8] = {0.0};

        /* write */
        /* ----- */
        CHECK(mliEventTapeWriter_open(
                &taro,
                "merlict_c89/"
                "mli_corsika_test_resources/"
                "run_no_events.tar",
                128));
        corh[0] = mli_4chars_to_float("RUNH");
        CHECK(mliEventTapeWriter_add_runh(&taro, corh));
        CHECK(mliEventTapeWriter_close(&taro));

        /* read back */
        /* --------- */
        CHECK(mliEventTapeReader_open(
                &tari,
                "merlict_c89/"
                "mli_corsika_test_resources/"
                "run_no_events.tar"));
        CHECK(mliEventTapeReader_read_runh(&tari, corh));
        corh[0] = mli_4chars_to_float("RUNH");

        CHECK(!mliEventTapeReader_read_evth(&tari, corh));
        CHECK(!mliEventTapeReader_read_cherenkov_bunch_raw(&tari, bunch));
        CHECK(mliEventTapeReader_close(&tari));
}

CASE("EventTape: first event no bunches")
{
        struct mliEventTapeWriter taro = mliEventTapeWriter_init();
        struct mliEventTapeReader tari = mliEventTapeReader_init();
        float corh[273] = {0.0};
        float bunch[8] = {0.0};

        /* write */
        /* ----- */
        CHECK(mliEventTapeWriter_open(
                &taro,
                "merlict_c89/"
                "mli_corsika_test_resources/"
                "run_first_event_no_bunches.tar",
                64));
        corh[0] = mli_4chars_to_float("RUNH");
        CHECK(mliEventTapeWriter_add_runh(&taro, corh));

        corh[0] = mli_4chars_to_float("EVTH");
        corh[MLI_CORSIKA_EVTH_EVENT_NUMBER] = 1337.0;
        CHECK(mliEventTapeWriter_add_evth(&taro, corh));
        CHECK(mliEventTapeWriter_close(&taro));

        memset(corh, 0.0, sizeof(corh));

        /* read back */
        /* --------- */
        CHECK(mliEventTapeReader_open(
                &tari,
                "merlict_c89/"
                "mli_corsika_test_resources/"
                "run_first_event_no_bunches.tar"));
        CHECK(mliEventTapeReader_read_runh(&tari, corh));
        CHECK(corh[0] == mli_4chars_to_float("RUNH"));

        CHECK(mliEventTapeReader_read_evth(&tari, corh));
        CHECK(corh[0] == mli_4chars_to_float("EVTH"));
        CHECK(corh[MLI_CORSIKA_EVTH_EVENT_NUMBER] == 1337.0);
        CHECK(!mliEventTapeReader_read_cherenkov_bunch_raw(&tari, bunch));
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
