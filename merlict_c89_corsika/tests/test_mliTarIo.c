/* Copyright 2014 Sebastian A. Mueller, Dominik Neise */

CASE("TarIo: read one by one")
{
        struct mliTarIoRun run = mliTarIoRun_init();
        struct mliTarIoEvent first_evt = mliTarIoEvent_init();
        struct mliTarIoEvent second_evt = mliTarIoEvent_init();
        struct mliTarIoEvent third_evt = mliTarIoEvent_init();
        struct mliTarIoEvent not_exist_evt = mliTarIoEvent_init();

        CHECK(mliTarIoRun_open(
                &run,
                "merlict_c89_corsika/tests/resources/corsika_primary_run.tar"));

        CHECK(run.corsika_run_header[0] == mli_4chars_to_float("RUNH"));

        CHECK(mliTarIoEvent_malloc_from_run(&first_evt, &run));
        CHECK(first_evt.corsika_event_header[0] == mli_4chars_to_float("EVTH"));
        mliTarIoEvent_free(&first_evt);

        CHECK(mliTarIoEvent_malloc_from_run(&second_evt, &run));
        CHECK(second_evt.corsika_event_header[0] ==
              mli_4chars_to_float("EVTH"));
        mliTarIoEvent_free(&second_evt);

        CHECK(mliTarIoEvent_malloc_from_run(&third_evt, &run));
        CHECK(third_evt.corsika_event_header[0] == mli_4chars_to_float("EVTH"));
        mliTarIoEvent_free(&third_evt);

        CHECK(!mliTarIoEvent_malloc_from_run(&not_exist_evt, &run));

        CHECK(mliTarIoRun_close(&run));
}

CASE("TarIo: while loop")
{
        struct mliPrng prng = mliPrng_init_MT19937(0u);
        struct mliTarIoRun run = mliTarIoRun_init();
        CHECK(mliTarIoRun_open(
                &run,
                "merlict_c89_corsika/tests/resources/corsika_primary_run.tar"));

        while (1) {
                uint64_t event_valid;
                uint64_t i;
                struct mliTarIoEvent event = mliTarIoEvent_init();

                event_valid = mliTarIoEvent_malloc_from_run(&event, &run);
                if (!event_valid) {
                        break;
                }

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
