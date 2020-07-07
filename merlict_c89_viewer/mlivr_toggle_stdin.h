/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MERLICT_C89_VIWER_MLIVR_TOGGLE_STDIN_H_
#define MERLICT_C89_VIWER_MLIVR_TOGGLE_STDIN_H_

#include <termios.h>

struct termios mlivr_disable_stdin_buffer(void);

void mlivr_restore_stdin_buffer(struct termios *old_terminal);

#endif
