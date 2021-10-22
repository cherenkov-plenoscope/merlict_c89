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
                "merlict_c89/mli_corsika_test_resources/corsika_primary_run.tar"));
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
                "merlict_c89/mli_corsika_test_resources/corsika_primary_run.tar"));

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

CASE("TarIoWriter: make run")
{
        const uint64_t BUFF_NUM = 64u;
        const uint64_t NUM_EVENTS = 4u;
        const uint64_t NUM_BUNCHES[] = {128, 23, 0, 117};
        uint64_t e, b;
        struct mliPrng prng = mliPrng_init_MT19937(0u);
        struct mliTarIoWriter taro = mliTarIoWriter_init();
        struct mliTarIoReader tari = mliTarIoReader_init();
        float corh[273];
        float bunch[8];
        memset(corh, 0, sizeof(corh));
        memset(bunch, 0, sizeof(bunch));

        CHECK(mliTarIoWriter_open(
                &taro, "merlict_c89/mli_corsika_test_resources/mini.tar", BUFF_NUM));
        corh[0] = mli_4chars_to_float("RUNH");
        CHECK(mliTarIoWriter_add_runh(&taro, corh));

        for (e = 0; e < NUM_EVENTS; e ++) {
                corh[0] = mli_4chars_to_float("EVTH");
                corh[MLI_CORSIKA_EVTH_EVENT_NUMBER] = 1.0 + (float)e;
                CHECK(mliTarIoWriter_add_evth(&taro, corh));
                for (b = 0; b < NUM_BUNCHES[e]; b ++) {
                        memset(bunch, 0, sizeof(bunch));
                        bunch[0] = (float)b;
                        CHECK(mliTarIoWriter_add_cherenkov_bunch(&taro, bunch));
                }
        }
        CHECK(mliTarIoWriter_close(&taro));

        /* read back */
        CHECK(mliTarIoReader_open(
                &tari, "merlict_c89/mli_corsika_test_resources/mini.tar"));
        CHECK(mliTarIoReader_read_runh(&tari, corh));
        corh[0] = mli_4chars_to_float("RUNH");

        e = 0;
        CHECK(tari.event_number == e + 1);
        while (mliTarIoReader_read_evth(&tari, corh)) {
                CHECK(tari.event_number == e + 1);
                CHECK(corh[0] == mli_4chars_to_float("EVTH"));
                CHECK(corh[MLI_CORSIKA_EVTH_EVENT_NUMBER] == 1.0 + (float)e);
                b = 0;
                CHECK(tari.bunch_number == b);
                while (mliTarIoReader_read_cherenkov_bunch(&tari, bunch)) {
                        CHECK(bunch[0] == (float)b);
                        b += 1;
                        CHECK(tari.bunch_number == b);
                }
                CHECK(!mliTarIoReader_read_cherenkov_bunch(&tari, bunch));
                e += 1;
        }
        CHECK(!mliTarIoReader_read_evth(&tari, corh));

        CHECK(mliTarIoReader_close(&tari));
}
