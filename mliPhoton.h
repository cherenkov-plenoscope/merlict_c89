/* Copyright 2019 Sebastian Achim Mueller */
#ifndef MERLICT_MLIPHOTON_H_
#define MERLICT_MLIPHOTON_H_

struct mliPhoton {
    struct mliRay ray;
    /* mliVec electric_field_polarization; */
    double wavelength;
};

struct mliPhoton mliPhoton_set(
    const struct mliRay ray,
    const double wavelength)
{
    struct mliPhoton ph;
    ph.ray = ray;
    ph.wavelength = wavelength;
    return ph;
}

#endif
