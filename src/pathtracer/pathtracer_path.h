/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLI_PATHTRACER_PATH_H_
#define MLI_PATHTRACER_PATH_H_

#include <stdint.h>

struct mli_pathtracer_Path {
        double weight;
        uint64_t num_interactions;
};

struct mli_pathtracer_Path mli_pathtracer_Path_init(void);

#endif
