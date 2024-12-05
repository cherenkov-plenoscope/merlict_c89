/**
 *  2021 March 23, Sebastian Achim Mueller
 *
 *  Based on 'pcg_variants.h' written by Melissa O'Neill.
 *
 *  I only kept the version with the 64bit sequence state to generate
 *  32bit numbers.
 *  I dropped 'advance', and 'boundedrand'.
 *  I only kept the seeding and the generation.
 *  I split the original header-only into a source.c and a header.h.
 */

/*
 * PCG Random Number Generation for C.
 *
 * Copyright 2014 Melissa O'Neill <oneill@pcg-random.org>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * For additional information about the PCG random number generation scheme,
 * including its license and other licensing options, visit
 *
 *     http://www.pcg-random.org
 */

#ifndef MLI_PRNG_PCG_VARIANTS_32BIT_SUBSET_H_INCLUDED
#define MLI_PRNG_PCG_VARIANTS_32BIT_SUBSET_H_INCLUDED

#include <stdint.h>

struct mli_prng_pcg_state_setseq_64 {
        uint64_t state;
        uint64_t inc;
};

void mli_prng_pcg_setseq_64_srandom_r(
        struct mli_prng_pcg_state_setseq_64 *rng,
        uint64_t initstate,
        uint64_t initseq);

uint32_t mli_prng_pcg_setseq_64_xsh_rr_32_random_r(
        struct mli_prng_pcg_state_setseq_64 *rng);

#endif
