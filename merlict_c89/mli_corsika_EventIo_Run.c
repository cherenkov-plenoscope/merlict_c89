/* Copyright 2016 Sebastian A. Mueller, Dominik Neise */
#include "mli_corsika_EventIo_Run.h"

struct mliEventIoRun mliEventIoRun_init(void)
{
        struct mliEventIoRun run;
        run.f = NULL;
        run.next_block = mliEventIoHeader_init();
        memset(run.corsika_run_header, 0.0, 273);
        run.corsika_input_card = mliStr_init();
        run.telescope_positions = mliDynEventIoTelescopePosition_init();
        return run;
}

int mliEventIoRun_read_273float32_block(FILE *f, float *block)
{
        int32_t block_size;
        chk_fread(&block_size, sizeof(int32_t), 1, f);
        chk_msg(block_size == 273, "Expected block-size to be 273.");
        chk_fread(block, sizeof(float), (uint64_t)block_size, f);
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

int mliEventIoRun_read_input_card(
        FILE *f,
        struct mliStr *input_card,
        const uint64_t length)
{
        char _unknown[8];
        uint64_t input_card_length;

        chk_msg(mliStr_malloc_capacity(input_card, length + 1),
                "Failed to malloc cstr for input-card.");

        chk_fread(_unknown, sizeof(_unknown), 1, f);
        chk_msg(length >= sizeof(_unknown),
                "Expected at least 8bytes payload.");
        input_card_length = length - sizeof(_unknown);
        chk_fread(input_card->cstr, sizeof(char), input_card_length, f);
        return 1;
error:
        return 0;
}

int mliEventIoRun_read_telescope_positions(
        FILE *f,
        struct mliDynEventIoTelescopePosition *telescope_positions,
        const uint64_t length)
{
        int32_t ntel;
        int num_following_arrays;

        chk_fread(&ntel, sizeof(int32_t), 1, f);

        num_following_arrays = (int)((length - 4) / ntel / 4);

        chk_msg(num_following_arrays == 4, "Expected exactly four arrays.")

                chk(mliDynEventIoTelescopePosition_malloc_set_size(
                        telescope_positions, ntel));
        chk_fread(
                telescope_positions->array,
                sizeof(struct mliEventIoTelescopePosition),
                (uint64_t)ntel,
                f);
        return 1;
error:
        return 0;
}

int mliEventIoRun_next_block(struct mliEventIoRun *run, const int level)
{
        chk_msg(mliEventIoHeader_read(&run->next_block, run->f, level),
                "Failed to read EventIo-block-header.");
        return 1;
error:
        return 0;
}

int mliEventIoRun_open(struct mliEventIoRun *run, const char *path)
{
        run->f = fopen(path, "rb");
        chk_msg(run->f, "Can not open EventIo-file.");

        /* corsika_run_header */
        /* ------------------ */
        chk(mliEventIoRun_next_block(run, MLI_EVENTIO_TOP_LEVEL));
        chk_msg(run->next_block.type == 1200, "Expected type 1200.");
        chk_msg(mliEventIoRun_read_273float32_block(
                        run->f, run->corsika_run_header),
                "Failed to read corsika_run_header 273 float block.");

        /* corsika_input_card */
        /* ------------------ */
        chk(mliEventIoRun_next_block(run, MLI_EVENTIO_TOP_LEVEL));
        chk_msg(run->next_block.type == 1212, "Expected type 1212.");
        chk_msg(mliEventIoRun_read_input_card(
                        run->f,
                        &run->corsika_input_card,
                        run->next_block.length),
                "Failed to read corsika-input-card.");

        /* telescope_positions */
        /* ------------------- */
        chk(mliEventIoRun_next_block(run, MLI_EVENTIO_TOP_LEVEL));
        chk_msg(run->next_block.type == 1201, "Expected type 1201.");
        chk_msg(mliEventIoRun_read_telescope_positions(
                        run->f,
                        &run->telescope_positions,
                        run->next_block.length),
                "Failed to read telescope-positions.");

        /* next */
        /* ---- */
        chk(mliEventIoRun_next_block(run, MLI_EVENTIO_TOP_LEVEL));

        return 1;
error:
        return 0;
}

int mliEventIoRun_has_still_events_left(struct mliEventIoRun *run)
{
        const int32_t CORSIKA_RUN_END_TYPE = 1210;
        return run->next_block.type != CORSIKA_RUN_END_TYPE;
}

void mliEventIoRun_close(struct mliEventIoRun *run)
{
        mliStr_free(&run->corsika_input_card);
        mliDynEventIoTelescopePosition_free(&run->telescope_positions);
        fclose(run->f);
}
