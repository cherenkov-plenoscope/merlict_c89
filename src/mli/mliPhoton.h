/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLIPHOTON_H_
#define MLIPHOTON_H_

#include <stdint.h>
#include "../ray/ray.h"

struct mliPhoton {
        struct mliRay ray;
        double wavelength;
        int64_t id;
};

#endif
