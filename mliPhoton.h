/* Copyright 2019 Sebastian Achim Mueller */
#ifndef MERLICT_MLIPHOTON_H_
#define MERLICT_MLIPHOTON_H_

struct mliPhoton {
    struct mliRay ray;
    double wavelength;
    int64_t simulation_truth_id;
};

#endif
