/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLI_PHYSICS_H_
#define MLI_PHYSICS_H_

double mli_physics_plancks_spectral_radiance_law_W_per_m2_per_sr_per_m(
        const double wavelength,
        const double temperature);

double MLI_PHYSICS_SPEED_OF_LIGHT_M_PER_S(void);
double MLI_PHYSICS_BOLTZMANN_J_PER_K(void);
double MLI_PHYSICS_PLANCK_KG_M2_PER_S(void);

#endif
