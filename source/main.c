/*
** Project nobudget, 2022
**
** Author Francois Michaut
**
** Started on  Fri Feb 18 15:16:01 2022 Francois Michaut
** Last update Tue Feb 22 21:14:55 2022 Francois Michaut
**
** main.c : Main entry point
*/

#include "nobudget_def.h"

#include <ncurses.h>

int menu_loop();

int main()
{
    int ret = 0;

    if (!initscr() || start_color() == ERR) {
        dprintf(2, "Failed to initialize ncurses with colors.\n");
        return NOBUDGET_FATAL_ERROR;
    }
    if (cbreak() == ERR || noecho() == ERR || intrflush(stdscr, FALSE) == ERR ||
        keypad(stdscr, TRUE) == ERR || meta(stdscr, TRUE) == ERR ||
        leaveok(stdscr, TRUE) == ERR || nonl() == ERR || curs_set(0) == ERR) {
        endwin();
        dprintf(2, "Failed to setup terminal for interactive input.\n");
        return NOBUDGET_FATAL_ERROR;
    }
    ret = menu_loop();
    endwin();
    return ret;
}
