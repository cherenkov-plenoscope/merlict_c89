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

#define MLI_CORSIKA_HEADER_SIZE_BYTES (sizeof(float) * 273)
#define MLI_CORSIKA_BUNCH_SIZE_BYTES (sizeof(float) * 8)

#define MLI_CORSIKA_RUNH_RUN_NUMBER 1
#define MLI_CORSIKA_RUNH_SLOPE_OF_ENERGY_SPECTRUM 15
#define MLI_CORSIKA_RUNH_ENERGY_RANGE_START 16
#define MLI_CORSIKA_RUNH_ENERGY_RANGE_STOP 17
#define MLI_CORSIKA_RUNH_NUM_OBSERVATION_LEVELS 4

#define MLI_CORSIKA_EVTH_EVENT_NUMBER 1
#define MLI_CORSIKA_EVTH_RUN_NUMBER 43
#define MLI_CORSIKA_EVTH_PARTICLE_ID 2
#define MLI_CORSIKA_EVTH_ENERGY_GEV 3
#define MLI_CORSIKA_EVTH_ZENITH_RAD 10
#define MLI_CORSIKA_EVTH_AZIMUTH_RAD 11
#define MLI_CORSIKA_EVTH_FIRST_INTERACTION_HEIGHT_CM 6

#endif
