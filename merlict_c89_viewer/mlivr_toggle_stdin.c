#include "mlivr_toggle_stdin.h"
#include <unistd.h> /* STDIN_FILENO */

/* Thanks to Edwin Buck */
struct termios mlivr_disable_stdin_buffer(void)
{
        struct termios old_terminal;
        struct termios new_terminal;
        tcgetattr(STDIN_FILENO, &old_terminal);
        new_terminal = old_terminal;
        new_terminal.c_lflag &= ~(ICANON);
        tcsetattr(STDIN_FILENO, TCSANOW, &new_terminal);
        return old_terminal;
}

void mlivr_restore_stdin_buffer(struct termios *old_terminal)
{
        tcsetattr(STDIN_FILENO, TCSANOW, old_terminal);
}
