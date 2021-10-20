#include "mli_random_generator.h"

uint32_t mliPrng_generate_uint32(struct mliPrng *prng)
{
        return prng->generate_uint32(&prng->_storage);
}

void mliPrng_reinit(struct mliPrng *prng, const uint32_t seed)
{
        prng->reinit(&prng->_storage, seed);
}

/**
 *      Mersenne Twister 19937
 *      ----------------------
 */

struct mliPrng mliPrng_init_MT19937(const uint32_t seed)
{
        struct mliPrng prng;
        prng._storage.mt19937 = mliMT19937_init(seed);
        prng.generate_uint32 = _mliPrng_mliMT19937_generate_uint32;
        prng.reinit = _mliPrng_mliMT19937_reinit;
        return prng;
}

uint32_t _mliPrng_mliMT19937_generate_uint32(void *mt)
{
        return mliMT19937_generate_uint32((struct mliMT19937 *)mt);
}

void _mliPrng_mliMT19937_reinit(void *mt, const uint32_t seed)
{
        _mliMT19937_reinit((struct mliMT19937 *)mt, seed);
}

/**
 *      PCG32
 *      -----
 */

struct mliPrng mliPrng_init_PCG32(const uint32_t seed)
{
        struct mliPrng prng;
        prng._storage.pcg32 = mliPCG32_init(seed);
        prng.generate_uint32 = _mliPrng_mliPCG32_generate_uint32;
        prng.reinit = _mliPrng_mliPCG32_reinit;
        return prng;
}

uint32_t _mliPrng_mliPCG32_generate_uint32(void *pcg)
{
        return mliPCG32_generate_uint32((struct mliPCG32 *)pcg);
}

void _mliPrng_mliPCG32_reinit(void *pcg, const uint32_t seed)
{
        mliPCG32_reinit((struct mliPCG32 *)pcg, seed);
}
