#include "physics.h"
#include <math.h>

double mli_physics_plancks_spectral_radiance_law_W_per_m2_per_sr_per_m(
        const double wavelength_m,
        const double temperature_K)
{
        const double c = MLI_PHYSICS_SPEED_OF_LIGHT_M_PER_S();
        const double k = MLI_PHYSICS_BOLTZMANN_J_PER_K();
        const double h = MLI_PHYSICS_PLANCK_KG_M2_PER_S();
        const double T = temperature_K;
        const double l = wavelength_m;
        const double A = (2.0 * h * pow(c, 2.0)) / pow(l, 5.0);
        const double _denominator = exp((h * c) / (l * k * T)) - 1.0;
        const double B = (1.0 / _denominator);
        return A * B;
}

double MLI_PHYSICS_SPEED_OF_LIGHT_M_PER_S(void) { return 299792458.0; }

double MLI_PHYSICS_BOLTZMANN_J_PER_K(void) { return 1.380649e-23; }

double MLI_PHYSICS_PLANCK_KG_M2_PER_S(void) { return 6.626e-34; }
