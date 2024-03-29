/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLI_CORSIKA_RAY_VOXEL_OVERLAP_H_
#define MLI_CORSIKA_RAY_VOXEL_OVERLAP_H_

#include <stdint.h>
#include "../src/mli_corsika_CorsikaPhotonBunch.h"
#include "../../mli/src/mliDynUint32.h"
#include "../../mli/src/mliDynDouble.h"

void mli_corsika_overlap_of_ray_with_voxels(
        const struct mliCorsikaPhotonBunch *bunch,
        const struct mliDynDouble *x_bin_edges,
        const struct mliDynDouble *y_bin_edges,
        const struct mliDynDouble *z_bin_edges,
        struct mliDynUint32 *x_idxs,
        struct mliDynUint32 *y_idxs,
        struct mliDynUint32 *z_idxs,
        struct mliDynDouble *overlaps);

#endif
