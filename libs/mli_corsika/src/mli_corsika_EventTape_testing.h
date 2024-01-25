/* Copyright 2020 Sebastian A. Mueller */
#ifndef MLI_CORSIKA_EVENTTAPE_TESTING_H_
#define MLI_CORSIKA_EVENTTAPE_TESTING_H_

#include <stdint.h>
#include "mli_corsika_EventTape.h"
#include "../../mli_core/src/mli_random.h"
#include "../../mli_core/src/mli_random_generator.h"

void mliEventTape_testing_set_random_corsika_header(
        float *head,
        struct mliPrng *prng);
void mliEventTape_testing_set_random_RUNH(
        float *runh,
        const float run_number,
        struct mliPrng *prng);
void mliEventTape_testing_set_random_EVTH(
        float *evth,
        const float event_number,
        const float run_number,
        struct mliPrng *prng);
void mliEventTape_testing_set_random_bunch(float *bunch, struct mliPrng *prng);
int mliEventTape_testing_bunches_are_equal(float *b1, float *b2);
int mliEventTape_testing_corsika_headers_are_equal(
        const float *h1,
        const float *h2);
int mliEventTape_testing_write_and_read(
        const char *path,
        const uint64_t num_events,
        const uint64_t buffer_size,
        const float *event_numbers,
        const uint64_t *num_bunches,
        const uint32_t random_seed);

#endif
