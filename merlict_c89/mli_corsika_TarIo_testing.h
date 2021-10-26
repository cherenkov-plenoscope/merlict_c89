/* Copyright 2020 Sebastian A. Mueller */
#ifndef MLI_CORSIKA_TARIO_TESTING_H_
#define MLI_CORSIKA_TARIO_TESTING_H_

#include <stdint.h>
#include "mli_corsika_TarIo_Writer.h"

void mliTarIo_testing_set_random_RUNH(float *runh, struct mliPrng *prng);
void mliTarIo_testing_set_random_EVTH(
        float *evth,
        const float event_number,
        struct mliPrng *prng);
void mliTarIo_testing_set_random_bunch(
        struct mliCorsikaPhotonBunch *bunch,
        struct mliPrng *prng);
int mliTarIo_testing_bunches_are_equal(
        struct mliCorsikaPhotonBunch *b1,
        struct mliCorsikaPhotonBunch *b2);
int mliTarIo_testing_corsika_headers_are_equal(
        const float *h1,
        const float *h2);
int mliTarIo_testing_write_and_read(
        const char *path,
        const uint64_t num_events,
        const uint64_t buffer_size,
        const float *event_numbers,
        const uint64_t *num_bunches,
        const uint32_t random_seed);

#endif
