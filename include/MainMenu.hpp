/*
** Project nobudget, 2022
**
** Author Francois Michaut
**
** Started on  Tue Mar  1 13:51:26 2022 Francois Michaut
** Last update Wed Mar 16 20:26:28 2022 Francois Michaut
**
** MainMenu.hpp : Main menu class definition
*/

#pragma once

#include "cpp-ncurses/Menu.hpp"

class MainMenu {
    public:
        MainMenu();
        ~MainMenu() = default;

        void run();
    private:
        ncurses::Menu login_menu;
        ncurses::Menu main_menu;
};
