/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLI_CORSIKA_RAY_VOXEL_OVERLAP_H_
#define MLI_CORSIKA_RAY_VOXEL_OVERLAP_H_

#include <stdint.h>
#include "CorsikaPhotonBunch.h"
#include "../vector/uint32_vector.h"
#include "../vector/double_vector.h"

void mli_corsika_overlap_of_ray_with_voxels(
        const struct mli_corsika_PhotonBunch *bunch,
        const struct mli_DoubleVector *x_bin_edges,
        const struct mli_DoubleVector *y_bin_edges,
        const struct mli_DoubleVector *z_bin_edges,
        struct mli_Uint32Vector *x_idxs,
        struct mli_Uint32Vector *y_idxs,
        struct mli_Uint32Vector *z_idxs,
        struct mli_DoubleVector *overlaps);

#endif
