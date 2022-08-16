/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "mliOOBB.h"
#include "mliMat.h"
#include "chk_debug.h"

int mli_estimate_covariant_transformation_from_vertices(
        const struct mliVec *vertices,
        const uint64_t num_vertices,
        struct mliHomTra *t)
{
        const struct mliVec mean = mliVec_mean(vertices, num_vertices);
        const struct mliMat cov =
                mliMat_covariance(vertices, num_vertices, mean);

        double cov_eigenvalues[3];
        struct mliVec cov_eigenvectors[3];
        const uint64_t MAX_ITERATIONS = 100;
        const double EIGENVALUE_MARGIN = 1e-4;
        int i;

        chk_msg(num_vertices >= 3, "Expected at least three vertices.");
        mliMat_find_eigenvalues(
                cov,
                &cov_eigenvalues[0],
                &cov_eigenvalues[1],
                &cov_eigenvalues[2],
                EIGENVALUE_MARGIN,
                MAX_ITERATIONS);

        for (i = 0; i < 3; i++) {
                chk_msg(mliMat_find_eigenvector_for_eigenvalue(
                                cov,
                                cov_eigenvalues[i],
                                &cov_eigenvectors[i],
                                1e-16),
                        "Failed to estimate eigenvector for eigenvalue");
        }

        /* set transformation */
        /* ------------------ */
        t->translation = mean;
        t->rotation = mliMat_init_columns(
                cov_eigenvectors[0], cov_eigenvectors[1], cov_eigenvectors[2]);
        t->rotation = mliMat_transpose(t->rotation); /* inverse */

        return 1;
error:
        return 0;
}
