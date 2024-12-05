#include "toggle_stdin.h"

#ifdef __unix__

#include <unistd.h> /* STDIN_FILENO */

struct termios mli_viewer_non_canonical_stdin(void)
{
        struct termios old_terminal;
        struct termios new_terminal;
        tcgetattr(STDIN_FILENO, &old_terminal);
        new_terminal = old_terminal;
        new_terminal.c_lflag &= ~(ICANON);
        tcsetattr(STDIN_FILENO, TCSANOW, &new_terminal);
        return old_terminal;
}

void mli_viewer_restore_stdin(struct termios *old_terminal)
{
        tcsetattr(STDIN_FILENO, TCSANOW, old_terminal);
}

#endif /* __unix__ */
