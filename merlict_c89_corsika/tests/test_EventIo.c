#include <stdio.h>

CASE("EventIoHeader_works")
{
        FILE *f;
        struct mliEventIoHeader my_header;
        f = fopen("merlict_c89_corsika/tests/resources/telescope.dat", "rb");
        CHECK(f);
        CHECK(mliEventIoHeader_read_from_file(&my_header, f, 1));
        fclose(f);

        CHECK(my_header.is_sync);
        CHECK(my_header.type == 1200);
        CHECK(my_header.version == 0);
        CHECK(!my_header.user);
        CHECK(!my_header.extended);
        CHECK(!my_header.only_sub_objects);
        CHECK(my_header.length == 1096u);
        CHECK(my_header.id == 7);
}

CASE("EventIoHeader_fails_wrong_sync_marker")
{
        FILE *fin, *ftmp, *fbad;
        int i;
        struct mliEventIoHeader my_header;

        /* break a file */
        /* ------------ */
        fin = fopen("merlict_c89_corsika/tests/resources/telescope.dat", "rb");
        CHECK(fin);
        ftmp =
                fopen("merlict_c89_corsika/tests/resources/"
                      "telescope.dat-bad-sync.tmp",
                      "wb");
        CHECK(ftmp);
        for (i = 0; i < 100; i++) {
                char buff;
                CHECK(fread(&buff, sizeof(char), 1, fin));
                if (i == 2) {
                        buff = 0x00;
                }
                CHECK(fwrite(&buff, sizeof(char), 1, ftmp));
        }
        fclose(fin);
        fclose(ftmp);

        /* read broken file */
        /* ---------------- */
        fbad =
                fopen("merlict_c89_corsika/tests/resources/"
                      "telescope.dat-bad-sync.tmp",
                      "wb");
        CHECK(fbad);
        CHECK(!mliEventIoHeader_read_from_file(&my_header, fbad, 1));
        fclose(fbad);
}

CASE("EventIoHeader_fails_empty_file")
{
        FILE *fempty;
        int tmp;
        struct mliEventIoHeader my_header;
        fempty = fopen(
                "merlict_c89_corsika/tests/resources/telescope.dat", "rb");
        CHECK(fempty);
        tmp = getc(fempty);
        while (tmp != EOF) {
                tmp = getc(fempty);
        }
        CHECK(!mliEventIoHeader_read_from_file(&my_header, fempty, 1));
        fclose(fempty);
}

CASE("EventIoFile_telescope_dat__check_tel_pos")
{
        struct mliEventIoRun run = mliEventIoRun_init();
        CHECK(mliEventIoRun_open(
                &run, "merlict_c89_corsika/tests/resources/telescope.dat"));

        CHECK(run.corsika_run_header[0] == mli_4chars_to_float("RUNH"));

        CHECK(run.telescope_positions.size == 1u);
        CHECK(run.telescope_positions.array[0].x == 0.);
        CHECK(run.telescope_positions.array[0].y == 0.);
        CHECK(run.telescope_positions.array[0].z == 500.);
        CHECK(run.telescope_positions.array[0].r == 500.);

        mliEventIoRun_close(&run);
}

CASE("EventIoFile_telescope_dat__check_input_card")
{
        struct mliEventIoRun run = mliEventIoRun_init();
        CHECK(mliEventIoRun_open(
                &run, "merlict_c89_corsika/tests/resources/telescope.dat"));
        CHECK(run.corsika_input_card.c_str[0] == 'C');
        CHECK(run.corsika_input_card.c_str[1] == 'O');
        CHECK(run.corsika_input_card.c_str[2] == 'R');
        CHECK(run.corsika_input_card.c_str[3] == 'S');
        CHECK(run.corsika_input_card.c_str[4] == 'I');
        CHECK(run.corsika_input_card.c_str[5] == 'K');
        CHECK(run.corsika_input_card.c_str[6] == 'A');
        mliEventIoRun_close(&run);
}

CASE("EventIoFile_telescope_dat__mmcs_run_header")
{
        float runh[273];
        struct mliEventIoRun run = mliEventIoRun_init();
        CHECK(mliEventIoRun_open(
                &run, "merlict_c89_corsika/tests/resources/telescope.dat"));
        memcpy(runh, run.corsika_run_header, sizeof(runh));

        CHECK_MARGIN(7., runh[MLI_CORSIKA_RUNH_RUN_NUMBER], 1e-6);
        CHECK_MARGIN(
                -2.7, runh[MLI_CORSIKA_RUNH_SLOPE_OF_ENERGY_SPECTRUM], 1e-6);
        CHECK_MARGIN(1000., runh[MLI_CORSIKA_RUNH_ENERGY_RANGE_START], 1e-6);
        CHECK_MARGIN(50000., runh[MLI_CORSIKA_RUNH_ENERGY_RANGE_STOP], 1e-6);
        CHECK_MARGIN(1., runh[MLI_CORSIKA_RUNH_NUM_OBSERVATION_LEVELS], 1e-6);
        CHECK_MARGIN(
                220000.,
                runh[MLI_CORSIKA_RUNH_NUM_OBSERVATION_LEVELS + 1],
                1e-6);
        mliEventIoRun_close(&run);
}

CASE("EventIoFile_telescope_dat__next_call")
{
        struct mliEventIoRun run = mliEventIoRun_init();
        struct mliEventIoEvent event = mliEventIoEvent_init();
        CHECK(mliEventIoRun_open(
                &run, "merlict_c89_corsika/tests/resources/telescope.dat"));
        CHECK(mliEventIoEvent_malloc_from_run(&event, &run));
        mliEventIoRun_close(&run);
        mliEventIoEvent_free(&event);
}

CASE("EventIoFile_telescope_dat__event_header")
{
        struct mliEventIoRun run = mliEventIoRun_init();
        struct mliEventIoEvent event = mliEventIoEvent_init();
        float evth[273];
        CHECK(mliEventIoRun_open(
                &run, "merlict_c89_corsika/tests/resources/telescope.dat"));
        CHECK(mliEventIoEvent_malloc_from_run(&event, &run));

        CHECK(event.telescope_offsets.size == 1u);
        CHECK_MARGIN(379489.3125, event.telescope_offsets.array[0].toff, 1e-6);

        CHECK_MARGIN(-0., event.telescope_offsets.array[0].xoff, 1e-6);
        CHECK_MARGIN(
                -6589.96044922, event.telescope_offsets.array[0].yoff, 1e-6);
        memcpy(evth, event.corsika_event_header, sizeof(evth));

        CHECK_MARGIN(1., evth[MLI_CORSIKA_EVTH_EVENT_NUMBER], 1e-6);
        CHECK_MARGIN(1., evth[MLI_CORSIKA_EVTH_PARTICLE_ID], 1e-6);
        CHECK_MARGIN(2745.3125, evth[MLI_CORSIKA_EVTH_ENERGY_GEV], 1e-6);

        mliEventIoRun_close(&run);
        mliEventIoEvent_free(&event);
}

CASE("EventIoFile_telescope_dat__photon_bundle_size")
{
        uint64_t i;
        struct mliEventIoRun run = mliEventIoRun_init();
        struct mliEventIoEvent event = mliEventIoEvent_init();
        CHECK(mliEventIoRun_open(
                &run, "merlict_c89_corsika/tests/resources/telescope.dat"));
        CHECK(mliEventIoEvent_malloc_from_run(&event, &run));

        CHECK(event.photon_bunches.size == 42629u);
        for (i = 0; i < event.photon_bunches.size; i++) {
                struct mliCorsikaPhotonBunch bunch =
                        event.photon_bunches.array[i];
                CHECK(bunch.z_emission_cm > 0.0);
        }

        mliEventIoRun_close(&run);
        mliEventIoEvent_free(&event);
}

CASE("EventIoFile_telescope_dat__photon_bundle_values")
{
        int j;
        struct mliCorsikaPhotonBunch some[5] = {{161.90000915527344,
                                                 236.8000030517578,
                                                 0.23186667263507843,
                                                 0.01769999973475933,
                                                 -11.800000190734863,
                                                 1462178.625,
                                                 1.0,
                                                 -692.0},
                                                {-323.5,
                                                 373.3000183105469,
                                                 0.23149999976158142,
                                                 0.017799999564886093,
                                                 -15.40000057220459,
                                                 1458816.0,
                                                 1.0,
                                                 -380.0},
                                                {-1.899999976158142,
                                                 286.20001220703125,
                                                 0.23173333704471588,
                                                 0.017733333632349968,
                                                 -13.0,
                                                 1458816.0,
                                                 1.0,
                                                 -351.0},
                                                {43.5,
                                                 483.0,
                                                 0.23313333094120026,
                                                 0.0351666659116745,
                                                 -10.199999809265137,
                                                 1276440.0,
                                                 0.9899999499320984,
                                                 -373.0},
                                                {7.300000190734863,
                                                 -457.1000061035156,
                                                 0.23346666991710663,
                                                 0.03263333439826965,
                                                 -11.699999809265137,
                                                 1358314.625,
                                                 0.9899999499320984,
                                                 -523.0}};

        struct mliEventIoRun run = mliEventIoRun_init();
        struct mliEventIoEvent event = mliEventIoEvent_init();
        CHECK(mliEventIoRun_open(
                &run, "merlict_c89_corsika/tests/resources/telescope.dat"));
        CHECK(mliEventIoEvent_malloc_from_run(&event, &run));
        CHECK(event.photon_bunches.size == 42629u);

        for (j = 2; j < 5; j++) {
                struct mliCorsikaPhotonBunch b = event.photon_bunches.array[j];

                CHECK_MARGIN(b.x_cm, some[j].x_cm, 1e-6);
                CHECK_MARGIN(b.y_cm, some[j].y_cm, 1e-6);
                CHECK_MARGIN(b.cx_rad, some[j].cx_rad, 1e-6);
                CHECK_MARGIN(b.cy_rad, some[j].cy_rad, 1e-6);
                CHECK_MARGIN(b.time_ns, some[j].time_ns, 1e-6);
                CHECK_MARGIN(b.z_emission_cm, some[j].z_emission_cm, 10.0);
                /* the height can sometimes be way off! */
                CHECK_MARGIN(b.weight_photons, some[j].weight_photons, 1e-6);
                CHECK_MARGIN(b.wavelength_nm, some[j].wavelength_nm, 1e-6);
        }
        mliEventIoRun_close(&run);
        mliEventIoEvent_free(&event);
}
/*
TEST_CASE("EventIoTest: EventIoFile_telescope_dat_run_time", "[merlict]") {
    eventio::Run my_run("merlict_corsika/tests/resources/telescope.dat");

    while (my_run.has_still_events_left()) {
        eventio::Event event = my_run.next_event();

        merlict::random::Mt19937 prng;

        std::vector<merlict::Photon> photons;
        unsigned int id = 0;
        for (std::array<float, 8> corsika_photon : event.photons) {
            merlict::EventIoPhotonFactory cpf(corsika_photon, id++, &prng);

            if (cpf.passed_atmosphere()) {
                photons.push_back(cpf.make_photon());
            }
        }
    }
}

*/