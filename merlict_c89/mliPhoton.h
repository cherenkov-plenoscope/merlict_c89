/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MERLICT_C89_MLIPHOTON_H_
#define MERLICT_C89_MLIPHOTON_H_

struct mliPhoton {
        struct mliRay ray;
        double wavelength;
        int64_t id;
};

#endif
