/* Copyright 2014 Sebastian A. Mueller, Dominik Neise */

CASE("TarIo: read one by one")
{
        struct mliTarIoRun run = mliTarIoRun_init();
        struct mliTarIoEvent evt1 = mliTarIoEvent_init();
        struct mliTarIoEvent evt2 = mliTarIoEvent_init();
        struct mliTarIoEvent evt3 = mliTarIoEvent_init();
        struct mliTarIoEvent evt_ = mliTarIoEvent_init();

        CHECK(mliTarIoRun_open(
                &run,
                "merlict_c89/mli_corsika_test_resources/"
                "corsika_primary_run.tar"));
        CHECK(run.corsika_run_header[0] == mli_4chars_to_float("RUNH"));

        CHECK(mliTarIoRun_has_still_events_left(&run));
        CHECK(mliTarIoEvent_malloc_from_run(&evt1, &run));
        CHECK(evt1.corsika_event_header[0] == mli_4chars_to_float("EVTH"));
        mliTarIoEvent_free(&evt1);

        CHECK(mliTarIoRun_has_still_events_left(&run));
        CHECK(mliTarIoEvent_malloc_from_run(&evt2, &run));
        CHECK(evt2.corsika_event_header[0] == mli_4chars_to_float("EVTH"));
        mliTarIoEvent_free(&evt2);

        CHECK(mliTarIoRun_has_still_events_left(&run));
        CHECK(mliTarIoEvent_malloc_from_run(&evt3, &run));
        CHECK(evt3.corsika_event_header[0] == mli_4chars_to_float("EVTH"));
        mliTarIoEvent_free(&evt3);

        CHECK(!mliTarIoRun_has_still_events_left(&run));
        CHECK(!mliTarIoEvent_malloc_from_run(&evt_, &run));

        CHECK(mliTarIoRun_close(&run));
}

CASE("TarIo: while loop")
{
        struct mliPrng prng = mliPrng_init_MT19937(0u);
        struct mliTarIoRun run = mliTarIoRun_init();
        CHECK(mliTarIoRun_open(
                &run,
                "merlict_c89/mli_corsika_test_resources/"
                "corsika_primary_run.tar"));

        while (mliTarIoRun_has_still_events_left(&run)) {
                uint64_t i;
                struct mliTarIoEvent event = mliTarIoEvent_init();
                CHECK(mliTarIoEvent_malloc_from_run(&event, &run));

                for (i = 0; i < event.photon_bunches.size; i++) {
                        struct mliCorsikaPhotonBunch bunch =
                                event.photon_bunches.array[i];
                        CHECK(fabs(bunch.wavelength_nm) > 190);
                        CHECK(fabs(bunch.wavelength_nm) < 700);
                }

                mliTarIoEvent_free(&event);
        }
        CHECK(mliTarIoRun_close(&run));
}

CASE("TarIoWriter: run normal")
{
        const uint64_t BUFF_NUM = 64u;
        const uint64_t NUM_EVENTS = 6u;
        const uint64_t NUM_BUNCHES[] = {23, 65, 0, 9, 7, 0};
        const float EVENT_NUMBERS[] = {1, 2, 3, 4, 5, 6};
        uint64_t e, b;
        struct mliTarIoWriter taro = mliTarIoWriter_init();
        struct mliTarIoReader tari = mliTarIoReader_init();
        float corh[273] = {0.0};
        struct mliCorsikaPhotonBunch bunch;

        /* write */
        /* ----- */
        CHECK(mliTarIoWriter_open(
                &taro,
                "merlict_c89/"
                "mli_corsika_test_resources/"
                "run_normal.tar",
                BUFF_NUM));
        corh[0] = mli_4chars_to_float("RUNH");
        CHECK(mliTarIoWriter_add_runh(&taro, corh));

        for (e = 0; e < NUM_EVENTS; e++) {
                corh[0] = mli_4chars_to_float("EVTH");
                corh[MLI_CORSIKA_EVTH_EVENT_NUMBER] = EVENT_NUMBERS[e];
                CHECK(mliTarIoWriter_add_evth(&taro, corh));
                for (b = 0; b < NUM_BUNCHES[e]; b++) {
                        mliCorsikaPhotonBunch_testing_mark_bunch(&bunch, b);
                        CHECK(mliTarIoWriter_add_cherenkov_bunch(&taro, bunch));
                }
        }
        CHECK(mliTarIoWriter_close(&taro));

        /* read back */
        /* --------- */
        CHECK(mliTarIoReader_open(
                &tari,
                "merlict_c89/"
                "mli_corsika_test_resources/"
                "run_normal.tar"));
        CHECK(mliTarIoReader_read_runh(&tari, corh));
        corh[0] = mli_4chars_to_float("RUNH");

        e = 0;
        CHECK(tari.event_number == 0);
        for (e = 0; e < NUM_EVENTS; e++) {
                CHECK(mliTarIoReader_read_evth(&tari, corh));
                CHECK(tari.event_number == EVENT_NUMBERS[e]);
                CHECK(corh[0] == mli_4chars_to_float("EVTH"));
                CHECK(corh[MLI_CORSIKA_EVTH_EVENT_NUMBER] == EVENT_NUMBERS[e]);
                for (b = 0; b < NUM_BUNCHES[e]; b++) {
                        CHECK(mliTarIoReader_read_cherenkov_bunch(
                                &tari, &bunch));
                        CHECK(mliCorsikaPhotonBunch_testing_bunch_has_mark(
                                bunch, b));
                }
                CHECK(!mliTarIoReader_read_cherenkov_bunch(&tari, &bunch));
        }
        CHECK(!mliTarIoReader_read_evth(&tari, corh));
        CHECK(mliTarIoReader_close(&tari));
}

CASE("TarIoWriter: no events")
{
        struct mliTarIoWriter taro = mliTarIoWriter_init();
        struct mliTarIoReader tari = mliTarIoReader_init();
        float corh[273] = {0.0};
        float bunch[8] = {0.0};

        /* write */
        /* ----- */
        CHECK(mliTarIoWriter_open(
                &taro,
                "merlict_c89/"
                "mli_corsika_test_resources/"
                "run_no_events.tar",
                128));
        corh[0] = mli_4chars_to_float("RUNH");
        CHECK(mliTarIoWriter_add_runh(&taro, corh));
        CHECK(mliTarIoWriter_close(&taro));

        /* read back */
        /* --------- */
        CHECK(mliTarIoReader_open(
                &tari,
                "merlict_c89/"
                "mli_corsika_test_resources/"
                "run_no_events.tar"));
        CHECK(mliTarIoReader_read_runh(&tari, corh));
        corh[0] = mli_4chars_to_float("RUNH");

        CHECK(!mliTarIoReader_read_evth(&tari, corh));
        CHECK(!mliTarIoReader_read_cherenkov_bunch_raw(&tari, bunch));
        CHECK(mliTarIoReader_close(&tari));
}

CASE("TarIoWriter: first event no bunches")
{
        struct mliTarIoWriter taro = mliTarIoWriter_init();
        struct mliTarIoReader tari = mliTarIoReader_init();
        float corh[273] = {0.0};
        float bunch[8] = {0.0};

        /* write */
        /* ----- */
        CHECK(mliTarIoWriter_open(
                &taro,
                "merlict_c89/"
                "mli_corsika_test_resources/"
                "run_first_event_no_bunches.tar",
                64));
        corh[0] = mli_4chars_to_float("RUNH");
        CHECK(mliTarIoWriter_add_runh(&taro, corh));

        corh[0] = mli_4chars_to_float("EVTH");
        corh[MLI_CORSIKA_EVTH_EVENT_NUMBER] = 1337.0;
        CHECK(mliTarIoWriter_add_evth(&taro, corh));
        CHECK(mliTarIoWriter_close(&taro));

        memset(corh, 0.0, sizeof(corh));

        /* read back */
        /* --------- */
        CHECK(mliTarIoReader_open(
                &tari,
                "merlict_c89/"
                "mli_corsika_test_resources/"
                "run_first_event_no_bunches.tar"));
        CHECK(mliTarIoReader_read_runh(&tari, corh));
        CHECK(corh[0] == mli_4chars_to_float("RUNH"));

        CHECK(mliTarIoReader_read_evth(&tari, corh));
        CHECK(corh[0] == mli_4chars_to_float("EVTH"));
        CHECK(corh[MLI_CORSIKA_EVTH_EVENT_NUMBER] == 1337.0);
        CHECK(!mliTarIoReader_read_cherenkov_bunch_raw(&tari, bunch));
        CHECK(mliTarIoReader_close(&tari));
}
