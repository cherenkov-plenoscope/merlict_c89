/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLI_VIEWER_TOGGLE_STDIN_H_
#define MLI_VIEWER_TOGGLE_STDIN_H_

#if defined(__unix__) || (defined(__APPLE__) && defined(__MACH__))
#define HAVE_TERMIOS_H 1
#endif

#ifdef HAVE_TERMIOS_H
#include <termios.h>
struct termios mlivr_non_canonical_stdin(void);
void mlivr_restore_stdin(struct termios *old_terminal);
#endif

#endif
