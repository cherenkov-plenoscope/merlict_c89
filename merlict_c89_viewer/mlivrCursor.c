/* Copyright 2019 Sebastian Achim Mueller                                     */
#include "mlivrCursor.h"

void mlivrCursor_move_up(struct mlivrCursor *cursor)
{
        if (cursor->row != 0)
                cursor->row -= 1;
}

void mlivrCursor_move_down(struct mlivrCursor *cursor)
{
        if (cursor->row + 1u < cursor->num_rows)
                cursor->row += 1;
}

void mlivrCursor_move_right(struct mlivrCursor *cursor)
{
        if (cursor->col != 0)
                cursor->col -= 1;
}

void mlivrCursor_move_left(struct mlivrCursor *cursor)
{
        if (cursor->col + 1u < cursor->num_cols)
                cursor->col += 1;
}
