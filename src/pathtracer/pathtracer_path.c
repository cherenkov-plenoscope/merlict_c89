/* Copyright 2018-2024 Sebastian Achim Mueller */
#include "pathtracer_path.h"

struct mli_pathtracer_Path mli_pathtracer_Path_init(void)
{
        struct mli_pathtracer_Path out;
        out.weight = 1.0;
        out.num_interactions = 0u;
        return out;
}
