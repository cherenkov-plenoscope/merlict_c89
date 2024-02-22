/* Copyright 2014 Sebastian A. Mueller */
#include "../../mli_testing/src/mli_testing.h"

CASE("EventIoPhotonFactoryTest: intersection_point_on_ground")
{
        /* compare the input ground intersection point with the actual
         * intersection point of the merlict photons when they are absorbed on
         * the ground.
         */

        /*
        const double production_distance_offset = 1e3;
        struct mliDynPhoton merlict_photons = mliDynPhoton_init();
        struct mliDynCorsikaPhotonBunch corsika_photons =
                mliDynCorsikaPhotonBunch_init();
        CHECK(mliDynPhoton_malloc(&merlict_photons, 0u));
        CHECK(mliDynCorsikaPhotonBunch_malloc(&corsika_photons, 0u));

        int64_t id = 0;
        for (float x = -1e4; x < 1e4; x = x + 1495.0) {
                for (float y = -1e4; y < 1e4; y = y + 1495.0) {
                        for (float ux = -0.5; ux < 0.5; ux = ux + 0.11) {
                                for (float vy = -0.5; vy < 0.5;
                                     vy = vy + 0.11) {
                                        struct mliPhoton merlict_photon;
                                        struct mliCorsikaPhotonBunch
                                                corsika_photon;
                                        corsika_photon.x_cm = x;
                                        corsika_photon.y_cm = y;
                                        corsika_photon.ux = ux;
                                        corsika_photon.vy = vy;
                                        corsika_photon.time_ns = 0.0;
                                        corsika_photon.z_emission_cm = 1e5;
                                        corsika_photon.weight_photons = 1.0;
                                        corsika_photon.wavelength_nm = 433.0;
                                        CHECK(mliDynCorsikaPhotonBunch_push_back(
                                                &corsika_photons,
                                                corsika_photon));

                                        merlict_photon =
                                                mliCorsikaPhotonBunch_to_merlict_photon(
                                                        corsika_photon
                                                                production_distance_offset,
                                                        id);

                                        CHECK(mliDynPhoton_push_back(
                                                &merlict_photons,
                                                merlict_photon));
                                        id += 1;
                                }
                        }
                }
        }

        // propagate merlict photons down to ground
        ml::Scenery scenery;
        ml::Disc *ground = scenery.root.add<ml::Disc>();
        ground->set_name_pos_rot("ground", ml::VEC3_ORIGIN, ml::ROT3_UNITY);
        scenery.colors.add("ground_color", ml::COLOR_GRAY);
        const unsigned int ground_sensor_id = 0;
        ground->outer_color = scenery.colors.get("ground_color");
        ground->inner_color = scenery.colors.get("ground_color");
        ground->set_radius(1e3);

        ml::sensor::Sensor sensor(ground_sensor_id, ground);
        std::vector<ml::sensor::Sensor *> sensor_vec = {&sensor};
        ml::sensor::Sensors sensor_list(sensor_vec);

        scenery.root.init_tree_based_on_mother_child_relations();

        // propagation settings
        ml::PropagationConfig settings;

        // photon propagation down to the ground
        ml::propagate_photons_in_frame_with_config(
                &photons, &scenery.root, &settings, &prng);

        // detect photons in ground sensor
        sensor_list.clear_history();
        sensor_list.assign_photons(&photons);

        REQUIRE(sensor.photon_arrival_history.size() == 1u);

        CHECK(x * 1e-2 == Approx(sensor.photon_arrival_history[0].x_intersect)
                                  .margin(1e-6));
        CHECK(y * 1e-2 == Approx(sensor.photon_arrival_history[0].y_intersect)
                                  .margin(1e-6));

        mliDynPhoton_free(&merlict_photons);
        mliDynCorsikaPhotonBunch_free(&corsika_photons);
        */
}

/*

CASE("EventIoPhotonFactoryTest: wavelength_sign") {
    ml::random::FakeConstant prng(0.0);
    for (int l = 1; l < 1337; l++) {
        const float sign = (l%2 == 0) ? +1.0: -1.0;
        const float lambda = static_cast<float>(l)*sign;
        const std::array<float, 8> corsika_photon = {
             1.2, 3.4, 0.0, 0.0, 1e-9, 1e5, 0.999, lambda};
        //   x    y    xcos ycos time  zem  weight lambda
        //   cm   cm   1    1    ns    cm   1      nm

        const int id = 1337;
        ml::EventIoPhotonFactory cpf(corsika_photon, id, &prng);
        REQUIRE(fabs(lambda) == Approx(cpf.wavelength()*1e9).margin(1e-2));
        REQUIRE(cpf.wavelength() > 0.0);
    }
}

CASE("EventIoPhotonFactoryTest: convert_photons", "[merlict]") {
    const std::array<float, 8> corsika_photon =
        {1.2, 3.4, 0.0, 0.0, 1e-9, 1e5, 0.999, 433};
    //   x    y    xcos ycos time  zem  weight lambda
    //   cm   cm   1    1    ns    cm   1    nm

    const int id = 1337;
    ml::random::FakeConstant prng(0.0);

    ml::EventIoPhotonFactory cpf(corsika_photon, id, &prng);

    REQUIRE(cpf.has_still_photons_to_be_made());
    ml::Photon ph = cpf.make_photon();

    CHECK(ph.simulation_truth_id == id);
    CHECK(433e-9 == Approx(ph.wavelength).margin(1e-9));

    // since the x,y angles are zero, the support vector can be tested
    CHECK(0.012 == Approx(ph.support().x).margin(1e-9));
    CHECK(0.034 == Approx(ph.support().y).margin(1e-9));
    CHECK(1e3 == Approx(ph.support().z).margin(1e-9));
}

TEST_CASE("EventIoPhotonFactoryTest: execute_atmospheric_absorption") {
    // In Corsika photons can have a 'weight' to represent a bunch of photons.
    // This was introduced in Corsika to save computations and disk space.
    // Corsika does not recomend to have weights above 5.
    // Merlict has no concet of weighted photon bunches.
    // We force Corsika to produce single photons, i.e. the maximum weight
    // in the Corsika output is 1.0.
    // A weight below 1.0 means, that the photon passed Corsika's atmosphere
    // simulation with a probability of this weigth.
    // In merlict we collapse this photons probability when the photon
    // is created in merlict. This is why the EventIoPhotonFactory is
    // given a PseudoRandomNumberGenerator.

    double absorbed = 0;
    double passed = 0;
    double total = 1e5;

    ml::random::Mt19937 prng(0u);

    for (double i = 0; i < total-1; i++) {
        float weight = i/total;  // from 0.0 to 1.0
        const std::array<float, 8> corsika_photon =
            {1.2, 3.4, 0.0, 0.0, 1e-9, 1e3, weight, 433};
        //   x    y    xcos ycos time  zem  weight lambda
        //   cm   cm   1    1    ns    cm   1    nm

        ml::EventIoPhotonFactory cpf(corsika_photon, 1337, &prng);

        if (cpf.has_still_photons_to_be_made())
            passed++;
        else
            absorbed++;
    }

    double passed_ratio = passed/total;
    CHECK(0.5 == Approx(passed_ratio).margin(1e-2));
}

CASE("EventIoPhotonFactoryTest: number_photons_made") {
    ml::random::Mt19937 prng(0u);
    for (int i = 0; i < 100; i++) {
        const std::array<float, 8> corsika_photon =
            {1.2, 3.4, 0.0, 0.0, 1e-9, 1e5, 15.5, 433};
        ml::EventIoPhotonFactory cpf(corsika_photon, 1337, &prng);
        CHECK(cpf.num_photons >= 15);
        CHECK(cpf.num_photons < 17);

        int num_photons_made = 0;
        while (cpf.has_still_photons_to_be_made()) {
            ml::Photon ph = cpf.make_photon();
            num_photons_made ++;
            CHECK(ph.simulation_truth_id == 1337);
        }
        CHECK(num_photons_made == cpf.num_photons_made);
        CHECK(num_photons_made <= cpf.num_photons);
        CHECK(num_photons_made >= 15);
    }
}

CASE("EventIoPhotonFactoryTest: weight < 0, only up to 1 photon") {
    ml::random::Mt19937 prng(0u);
    int num_photons_total = 0;
    for (int i = 0; i < 100; i++) {
        const float weight = prng.uniform();
        const std::array<float, 8> corsika_photon =
            {1.2, 3.4, 0.0, 0.0, 1e-9, 1e5, weight, 433};
        ml::EventIoPhotonFactory cpf(corsika_photon, 1337, &prng);
        int num_photons_made_from_bunch = 0;
        while (cpf.has_still_photons_to_be_made()) {
            ml::Photon ph = cpf.make_photon();
            num_photons_made_from_bunch ++;
            num_photons_total ++;
        }
        CHECK(num_photons_made_from_bunch <= 1);
    }
    CHECK(50 == Approx(num_photons_total).margin(10));
}

CASE("EventIoPhotonFactoryTest: merlict_rejects_photon_weight_below_0") {
    const unsigned int id = 1337;
    ml::random::Mt19937 prng(0u);
    const std::array<float, 8> corsika_photon =
        {1.2, 3.4, 0.0, 0.0, 1e-9, 1e5, -0.1, 433};

    CHECK_THROWS_AS(ml::EventIoPhotonFactory(corsika_photon, id, &prng),
ml::EventIoPhotonFactory::BadPhotonWeight);
}

CASE("EventIoPhotonFactoryTest: merlict_accepts_photon_weight_equal_1") {
    const unsigned int id = 1337;
    ml::random::Mt19937 prng(0u);
    const std::array<float, 8> corsika_photon =
        {1.2, 3.4, 0.0, 0.0, 1e-9, 1e5, 1.0, 433};

    CHECK_NOTHROW(ml::EventIoPhotonFactory(corsika_photon, id, &prng));
}

CASE("EventIoPhotonFactoryTest: merlict_rejects_photon_weight_above_1") {
    const unsigned int id = 1337;
    ml::random::Mt19937 prng(0u);
    const std::array<float, 8> corsika_photon =
        {1.2, 3.4, 0.0, 0.0, 1e-9, 1e5, 16.1, 433};

    CHECK_THROWS_AS(ml::EventIoPhotonFactory(corsika_photon, id, &prng),
ml::EventIoPhotonFactory::BadPhotonWeight);
}

CASE("EventIoPhotonFactoryTest: merlict_accepts_photon_weight_equal_0") {
    const unsigned int id = 1337;
    ml::random::Mt19937 prng(0u);
    const std::array<float, 8> corsika_photon =
        {1.2, 3.4, 0.0, 0.0, 1e-9, 1e5, 0.0, 433};
    CHECK_NOTHROW(ml::EventIoPhotonFactory(corsika_photon, id, &prng));
}

CASE("EventIoPhotonFactoryTest: merlict_accepts_photon_weight_btw_0_and_1") {
    const unsigned int id = 1337;
    ml::random::Mt19937 prng(0u);
    const std::array<float, 8>  corsika_photon =
        {1.2, 3.4, 0.0, 0.0, 1e-9, 1e5, 0.4455, 433};
    CHECK_NOTHROW(ml::EventIoPhotonFactory(corsika_photon, id, &prng));
}

CASE("EventIoPhotonFactoryTest: zero_weight_is_passed_on_zero_from_prng") {
    const std::array<float, 8>  corsika_photon =
    {1.2, 3.4, 0.0, 0.0, 1e-9, 1e5, 0.0, 433};
    const unsigned int id = 1337;
    ml::random::FakeConstant prng(0.0);
    ml::EventIoPhotonFactory cpf(corsika_photon, id, &prng);
    CHECK(cpf.has_still_photons_to_be_made());
}

CASE("EventIoPhotonFactoryTest: relative_arrival_time_on_ground") {
    float arrival_time_on_dround_in_ns = 1.0;
    const std::array<float, 8>  corsika_photon =
    {1.2, 3.4, 0.0, 0.0, arrival_time_on_dround_in_ns, 1e5, 0.4455, 433};
    const unsigned int id = 1337;
    ml::random::FakeConstant prng(0.0);
    ml::EventIoPhotonFactory cpf(corsika_photon, id, &prng);
    CHECK(cpf.relative_arrival_time_on_ground() ==
arrival_time_on_dround_in_ns*1e-9);
}

CASE("EventIoPhotonFactoryTest: correct_rel_time_when_intersecting_ground") {
    eventio::Run corsika_file("merlict_corsika/tests/resources/telescope.dat");
    while (corsika_file.has_still_events_left()) {
        // read in a corsika eventio event. we remember the relative arrival
        // times in the eventio file 'relative_arrival_times_in_corsika_file'
and
        // compare these to the actual arrival times of the merlict
        // photons on ground.

        eventio::Event event = corsika_file.next_event();

        std::vector<float> relative_arrival_times_in_corsika_file;

        std::vector<ml::Photon> photons;

        ml::random::Mt19937 prng(0u);

        for (unsigned int id = 0; id < event.photons.size(); id++) {
            ml::EventIoPhotonFactory factory(
                event.photons.at(id),
                id,
                &prng);
            relative_arrival_times_in_corsika_file.push_back(
                factory.relative_arrival_time_on_ground());
            photons.push_back(factory.make_photon());
        }

        // propagate merlict photons down to ground
        ml::Scenery scenery;
        ml::Disc* ground = scenery.root.add<ml::Disc>();
        ground->set_name_pos_rot("ground", ml::VEC3_ORIGIN, ml::ROT3_UNITY);
        scenery.colors.add("ground_color", ml::COLOR_GRAY);
        const unsigned int ground_sensor_id = 0;
        ground->outer_color = scenery.colors.get("ground_color");
        ground->inner_color = scenery.colors.get("ground_color");
        ground->set_radius(1e7);

        ml::sensor::Sensor sensor(ground_sensor_id, ground);
        std::vector<ml::sensor::Sensor*> sensor_vec = {&sensor};
        ml::sensor::Sensors sensor_list(sensor_vec);

        scenery.root.init_tree_based_on_mother_child_relations();

        // propagation settings
        ml::PropagationConfig settings;

        // photon propagation
        ml::propagate_photons_in_frame_with_config(
            &photons, &scenery.root, &settings, &prng);

        // detect photons in sensors
        sensor_list.clear_history();
        sensor_list.assign_photons(&photons);

        double mean_arrival_time = ml::sensor::arrival_time_mean(
            sensor.photon_arrival_history);

        for (
            unsigned int row = 0;
            row < sensor.photon_arrival_history.size();
            row++
        )
            sensor.photon_arrival_history[row].arrival_time -=
                mean_arrival_time;

        double mean_time_of_corsika_photons = std::accumulate(
            relative_arrival_times_in_corsika_file.begin(),
            relative_arrival_times_in_corsika_file.end(),
            0.0)/static_cast<double>(
                relative_arrival_times_in_corsika_file.size());

        for (unsigned int row = 0;
            row < sensor.photon_arrival_history.size();
            row++
        )
            sensor.photon_arrival_history[row].arrival_time +=
                mean_time_of_corsika_photons;

        // for each photon we compare the relative arrival
        // time written in the in the eventio file with the actual arrival time
        // of the merlict photon which ran down to the ground.
        for (
            unsigned int i = 0;
            i < sensor.photon_arrival_history.size();
            i++
        ) {
            unsigned int id =
                sensor.photon_arrival_history[i].simulation_truth_id;
            CHECK(relative_arrival_times_in_corsika_file.at(id) ==
Approx(sensor.photon_arrival_history[i].arrival_time).margin(1e-11));
        }

        // relative_arrival_times_in_corsika_file
    }
}
*/
