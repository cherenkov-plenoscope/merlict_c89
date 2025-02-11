/* Copyright 2019-2020 Sebastian Achim Mueller                                */

CASE("covariance")
{
        /* a crosscheck with numpy */
        struct mli_Vec vecs[9];
        struct mli_Vec vecs_mean;
        struct mli_Mat cov;

        /* random values */
        vecs[0] = mli_Vec_set(73.0, 7.0, 83.0);
        vecs[1] = mli_Vec_set(39.0, 97.0, 63.0);
        vecs[2] = mli_Vec_set(56.0, 95.0, 84.0);
        vecs[3] = mli_Vec_set(99.0, 21.0, 74.0);
        vecs[4] = mli_Vec_set(66.0, 50.0, 5.0);
        vecs[5] = mli_Vec_set(91.0, 29.0, 62.0);
        vecs[6] = mli_Vec_set(29.0, 1.0, 76.0);
        vecs[7] = mli_Vec_set(76.0, 42.0, 42.0);
        vecs[8] = mli_Vec_set(80.0, 46.0, 63.0);

        vecs_mean = mli_Vec_mean(vecs, 9);
        cov = mli_Mat_covariance(vecs, 9, vecs_mean);

        CHECK_MARGIN(cov.r00, 529.0, 1e-6);
        CHECK_MARGIN(cov.r01, -215.20833333, 1e-6);
        CHECK_MARGIN(cov.r02, -49.75, 1e-6);

        CHECK_MARGIN(cov.r10, -215.20833333, 1e-6);
        CHECK_MARGIN(cov.r11, 1177.36111111, 1e-6);
        CHECK_MARGIN(cov.r12, -98.16666667, 1e-6);

        CHECK_MARGIN(cov.r20, -49.75, 1e-6);
        CHECK_MARGIN(cov.r21, -98.16666667, 1e-6);
        CHECK_MARGIN(cov.r22, 614.0, 1e-6);
}

CASE("qr_decomposition")
{
        struct mli_Mat m = {
                529,
                -215.20833333,
                -49.75,
                -215.20833333,
                1177.36111111,
                -98.16666667,
                -49.75,
                -98.16666667,
                614.0};
        struct mli_Mat qr;
        struct mli_Mat q;
        struct mli_Mat r;
        double act_eval[3];
        struct mli_Vec act_evec[3];
        int i, j;

        /* expectation from numpy.linalg */
        double exp_eval[3] = {1252.33620323, 431.09407495, 636.93083293};
        struct mli_Vec exp_evec[3] = {
                {-0.27500715, 0.95326021, -0.12516409},
                {0.86629383, 0.30214904, 0.39779511},
                {-0.41702046, -0.00096762, 0.90889658},
        };

        mli_Mat_qr_decompose(m, &q, &r);
        qr = mli_Mat_multiply(q, r);

        CHECK_MARGIN(m.r00, qr.r00, 1e-6);
        CHECK_MARGIN(m.r01, qr.r01, 1e-6);
        CHECK_MARGIN(m.r02, qr.r02, 1e-6);
        CHECK_MARGIN(m.r10, qr.r10, 1e-6);
        CHECK_MARGIN(m.r11, qr.r11, 1e-6);
        CHECK_MARGIN(m.r12, qr.r12, 1e-6);
        CHECK_MARGIN(m.r20, qr.r20, 1e-6);
        CHECK_MARGIN(m.r21, qr.r21, 1e-6);
        CHECK_MARGIN(m.r22, qr.r22, 1e-6);

        mli_Mat_find_eigenvalues(
                m, &act_eval[0], &act_eval[1], &act_eval[2], 1e-4, 100);

        for (i = 0; i < 3; i++) {
                CHECK(mli_Mat_find_eigenvector_for_eigenvalue(
                        m, act_eval[i], &act_evec[i], 1e-16));
        }

        for (i = 0; i < 3; i++) {
                int match = 0;
                for (j = 0; j < 3; j++) {
                        if (mli_math_relative_ratio(exp_eval[i], act_eval[j]) <
                            1e-4) {
                                int eq;
                                match += 1;
                                eq = mli_Vec_equal_margin(
                                        exp_evec[i], act_evec[j], 1e-3);
                                if (eq == 0) {
                                        eq = mli_Vec_equal_margin(
                                                exp_evec[i],
                                                mli_Vec_multiply(
                                                        act_evec[j], -1.0),
                                                1e-3);
                                }
                                CHECK(eq);
                        }
                }
                CHECK(match == 1);
        }
}
