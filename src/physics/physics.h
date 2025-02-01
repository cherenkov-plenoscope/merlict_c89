/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLI_PHYSICS_H_
#define MLI_PHYSICS_H_

#define MLI_PHYSICS_SPEED_OF_LIGHT_M_PER_S 299792458
#define MLI_PHYSICS_BOLTZMANN_J_PER_K 1.380649e-23
#define MLI_PHYSICS_PLANCK_KG_M2_PER_S 6.626e-34

double mli_physics_plancks_spectral_radiance_law_W_per_m2_per_sr_per_m(
        const double wavelength,
        const double temperature);

#endif
