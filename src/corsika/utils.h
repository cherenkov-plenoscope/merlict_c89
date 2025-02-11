/* Copyright 2020 Sebastian A. Mueller */
#ifndef MLI_CORSIKA_UTILS_H_
#define MLI_CORSIKA_UTILS_H_

float mli_chars_to_float(const char *four_char_word);

double mli_corsika_ux_to_cx(const double ux);
double mli_corsika_vy_to_cy(const double vy);
double mli_corsika_wz_to_cz(const double wz);

double mli_corsika_cx_to_ux(const double cx);
double mli_corsika_cy_to_vy(const double cy);
double mli_corsika_cz_to_wz(const double cz);

double mli_corsika_restore_direction_z_component(
        const double x,
        const double y);

enum mli_corsika_block_sizes {
        MLI_CORSIKA_HEADER_SIZE_BYTES = (sizeof(float) * 273),
        MLI_CORSIKA_BUNCH_SIZE_BYTES = (sizeof(float) * 8)
};

enum mli_corsika_runh_types {
        MLI_CORSIKA_RUNH_RUN_NUMBER = 1,
        MLI_CORSIKA_RUNH_SLOPE_OF_ENERGY_SPECTRUM = 15,
        MLI_CORSIKA_RUNH_ENERGY_RANGE_START = 16,
        MLI_CORSIKA_RUNH_ENERGY_RANGE_STOP = 17,
        MLI_CORSIKA_RUNH_NUM_OBSERVATION_LEVELS = 4
};

enum mli_corsika_evth_types {
        MLI_CORSIKA_EVTH_EVENT_NUMBER = 1,
        MLI_CORSIKA_EVTH_RUN_NUMBER = 43,
        MLI_CORSIKA_EVTH_PARTICLE_ID = 2,
        MLI_CORSIKA_EVTH_ENERGY_GEV = 3,
        MLI_CORSIKA_EVTH_ZENITH_RAD = 10,
        MLI_CORSIKA_EVTH_AZIMUTH_RAD = 11,
        MLI_CORSIKA_EVTH_FIRST_INTERACTION_HEIGHT_CM = 6
};

#endif
