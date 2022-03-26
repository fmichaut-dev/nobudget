/*
** Project nobudget, 2022
**
** Author Francois Michaut
**
** Started on  Wed Feb 23 21:51:36 2022 Francois Michaut
** Last update Wed Mar 16 23:46:22 2022 Francois Michaut
**
** main.cpp : Main entry point of the program
*/

#include "cpp-ncurses/Kernel.hpp"
#include "cpp-ncurses/Window.hpp"

#include "MainMenu.hpp"

#include <iostream>

int main()
{
    try {
        ncurses::Kernel k;
        auto stdscr = ncurses::Window::stdscr();
        MainMenu main;

        // TODO ease options setup
        k.curs_set(0);
        k.cbreak(true);
        k.echo(false);
        k.nl(false);
        stdscr.intrflush(false);
        stdscr.keypad(true);
        stdscr.meta(true);
        stdscr.leaveok(true);

        main.run();
    } catch (std::exception &e) {
        std::cout << e.what() << std::endl;
    }
    return 0;
}
