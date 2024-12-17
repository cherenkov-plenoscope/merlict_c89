/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLIP_HOTON_H_
#define MLIP_HOTON_H_

#include <stdint.h>
#include "../ray/ray.h"

struct mli_Photon {
        struct mli_Ray ray;
        double wavelength;
        int64_t id;
};

#endif
