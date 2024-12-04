/**
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

#include "random_pcg_variants_32bit_subset.h"

#define PCG_DEFAULT_MULTIPLIER_64 6364136223846793005U
#define PCG_DEFAULT_INCREMENT_64 1442695040888963407U

/**     Rotate helper functions.
 */

uint32_t pcg_rotr_32(uint32_t value, unsigned int rot)
{
        return (value >> rot) | (value << ((-rot) & 31));
}

/**     Output functions.  These are the core of the PCG generation scheme.
 *
 *      XSH RR
 */

uint32_t pcg_output_xsh_rr_64_32(uint64_t state)
{
        return pcg_rotr_32(((state >> 18u) ^ state) >> 27u, state >> 59u);
}

/**     Functions to advance the underlying LCG.
 *      These functions are considered semi-private.
 *      There is rarely a good reason to call them directly.
 */

void pcg_setseq_64_step_r(struct pcg_state_setseq_64 *rng)
{
        rng->state = rng->state * PCG_DEFAULT_MULTIPLIER_64 + rng->inc;
}

/**     Seed the RNG state.
 */

void pcg_setseq_64_srandom_r(
        struct pcg_state_setseq_64 *rng,
        uint64_t initstate,
        uint64_t initseq)
{
        rng->state = 0U;
        rng->inc = (initseq << 1u) | 1u;
        pcg_setseq_64_step_r(rng);
        rng->state += initstate;
        pcg_setseq_64_step_r(rng);
}

/**     Now, finally we provide
 *      a random_r function that provides a random number of the appropriate
 *      type (using the full range of the type).
 *
 *      Generation functions for XSH RR
 */

uint32_t pcg_setseq_64_xsh_rr_32_random_r(struct pcg_state_setseq_64 *rng)
{
        uint64_t oldstate = rng->state;
        pcg_setseq_64_step_r(rng);
        return pcg_output_xsh_rr_64_32(oldstate);
}
