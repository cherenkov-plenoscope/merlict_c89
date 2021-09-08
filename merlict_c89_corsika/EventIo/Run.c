/* Copyright 2016 Sebastian A. Mueller, Dominik Neise */
#include "../../merlict_c89_corsika/EventIo/Run.h"

struct mliEventIoRun mliEventIoRun_init(void)
{
        struct mliEventIoRun run;
        run._f = NULL;
        run._next_block = mliEventIoHeader_init();
        memset(run.corsika_run_header, 0.0, 273);
        run.corsika_input_card = mliDynStr_init();
        run.telescope_positions = mliDynEventIoTelescopePosition_init();
        return run;
}

int _read_273_block(FILE *f, float *block)
{
        int32_t block_size;
        mli_fread(&block_size, sizeof(int32_t), 1, f);
        mli_check(block_size == 273, "Expected block-size to be 273.");
        mli_fread(block, sizeof(float), (uint64_t)block_size, f);
        return 1;
error:
        return 0;
}

/*
 *      runh 1200
 *      rune 1210
 *      evth 1202
 *      evte 1209
 *      array_header 1204
 *      bunches 1205 (sub)
 */

int _read_input_card(FILE *f, struct mliDynStr *input_card, const uint64_t length)
{
        char _unknown[8];
        uint64_t input_card_length;

        mli_check(
                mliDynStr_malloc(input_card, length + 1),
                "Failed to malloc c_str for input-card.");

        mli_fread(_unknown, sizeof(_unknown), 1, f);
        mli_check(
                length >= sizeof(_unknown),
                "Expected at least 8bytes payload.");
        input_card_length = length - sizeof(_unknown);
        mli_fread(input_card->c_str, sizeof(char), input_card_length, f);
        return 1;
error:
        return 0;
}

int _read_telescope_positions(
        FILE *f,
        struct mliDynEventIoTelescopePosition *telescope_positions,
        const uint64_t length)
{
        int32_t ntel;
        int num_following_arrays;

        mli_fread(&ntel, sizeof(int32_t), 1, f);

        num_following_arrays = (int)((length - 4) / ntel / 4);

        mli_check(num_following_arrays == 4, "Expected exactly four arrays.")

                mli_c(mliDynEventIoTelescopePosition_malloc_set_size(
                        telescope_positions, ntel));
        mli_fread(
                telescope_positions->array,
                sizeof(struct mliEventIoTelescopePosition),
                (uint64_t)ntel,
                f);
        return 1;
error:
        return 0;
}

int _mliEventIoRun_next_block(struct mliEventIoRun *run, const int level)
{
        mli_check(
                mliEventIoHeader_read(
                        &run->_next_block, run->_f, level),
                "Failed to read EventIo-block-header.");
        return 1;
error:
        return 0;
}

int mliEventIoRun_open(struct mliEventIoRun *run, const char *path)
{
        run->_f = fopen(path, "rb");
        mli_check(run->_f, "Can not open EventIo-file.");

        /* corsika_run_header */
        /* ------------------ */
        mli_c(_mliEventIoRun_next_block(run, MLI_EVENTIO_TOP_LEVEL));
        mli_check(run->_next_block.type == 1200, "Expected type 1200.");
        mli_check(
                _read_273_block(run->_f, run->corsika_run_header),
                "Failed to read corsika_run_header 273 float block.");


        /* corsika_input_card */
        /* ------------------ */
        mli_c(_mliEventIoRun_next_block(run, MLI_EVENTIO_TOP_LEVEL));
        mli_check(run->_next_block.type == 1212, "Expected type 1212.");
        mli_check(
                _read_input_card(
                        run->_f, &run->corsika_input_card, run->_next_block.length),
                "Failed to read corsika-input-card.");

        /* telescope_positions */
        /* ------------------- */
        mli_c(_mliEventIoRun_next_block(run, MLI_EVENTIO_TOP_LEVEL));
        mli_check(run->_next_block.type == 1201, "Expected type 1201.");
        mli_check(
                _read_telescope_positions(
                        run->_f, &run->telescope_positions, run->_next_block.length),
                "Failed to read telescope-positions.");

        /* next */
        /* ---- */
        mli_c(_mliEventIoRun_next_block(run, MLI_EVENTIO_TOP_LEVEL));

        return 1;
error:
        return 0;
}

int mliEventIoRun_has_still_events_left(struct mliEventIoRun *run)
{
        const int32_t CORSIKA_RUN_END_TYPE = 1210;
        return run->_next_block.type != CORSIKA_RUN_END_TYPE;
}

void mliEventIoRun_close(struct mliEventIoRun *run)
{
        mliDynStr_free(&run->corsika_input_card);
        mliDynEventIoTelescopePosition_free(&run->telescope_positions);
        fclose(run->_f);
}
