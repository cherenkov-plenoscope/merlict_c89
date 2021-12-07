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
        CHECK(run.corsika_run_header[0] == mli_chars_to_float("RUNH"));

        CHECK(mliTarIoRun_has_still_events_left(&run));
        CHECK(mliTarIoEvent_malloc_from_run(&evt1, &run));
        CHECK(evt1.corsika_event_header[0] == mli_chars_to_float("EVTH"));
        mliTarIoEvent_free(&evt1);

        CHECK(mliTarIoRun_has_still_events_left(&run));
        CHECK(mliTarIoEvent_malloc_from_run(&evt2, &run));
        CHECK(evt2.corsika_event_header[0] == mli_chars_to_float("EVTH"));
        mliTarIoEvent_free(&evt2);

        CHECK(mliTarIoRun_has_still_events_left(&run));
        CHECK(mliTarIoEvent_malloc_from_run(&evt3, &run));
        CHECK(evt3.corsika_event_header[0] == mli_chars_to_float("EVTH"));
        mliTarIoEvent_free(&evt3);

        CHECK(!mliTarIoRun_has_still_events_left(&run));
        CHECK(!mliTarIoEvent_malloc_from_run(&evt_, &run));

        CHECK(mliTarIoRun_close(&run));
}

CASE("TarIo: while loop")
{
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
