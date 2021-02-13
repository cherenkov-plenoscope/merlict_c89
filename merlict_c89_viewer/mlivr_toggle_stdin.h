/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MERLICT_C89_VIWER_MLIVR_TOGGLE_STDIN_H_
#define MERLICT_C89_VIWER_MLIVR_TOGGLE_STDIN_H_

#if defined(__unix__) || (defined(__APPLE__) && defined(__MACH__))
# define HAVE_TERMIOS_H 1
#endif

#ifdef HAVE_TERMIOS_H
#include <termios.h>
struct termios mlivr_non_canonical_stdin(void);
void mlivr_restore_stdin(struct termios *old_terminal);
#endif

#endif
