/*
** Project nobudget, 2022
**
** Author Francois Michaut
**
** Started on  Tue Mar  1 13:53:17 2022 Francois Michaut
** Last update Thu Mar  3 20:06:26 2022 Francois Michaut
**
** MainMenu.cpp : Main menu implementation
*/

#include "MainMenu.hpp"

#include <ncurses.h>

using ncurses::MenuItem;
using ncurses::Menu;

MainMenu::MainMenu() :
    login_menu("Login or Register", {
        std::make_shared<MenuItem>("Login", "Log into your account"),
        std::make_shared<MenuItem>("Register", "Create a new account"),
        std::make_shared<MenuItem>([](Menu &menu, void *){menu.stop();}, "Quit", "Quit nobudget")
    }),
    main_menu("NOBUDGET", {
        std::make_shared<MenuItem>("New Guest"),
        std::make_shared<MenuItem>("Manage Guests"),
        std::make_shared<MenuItem>("Help"),
        std::make_shared<MenuItem>("Logout"),
        std::make_shared<MenuItem>("Quit"),
    })
{}

void MainMenu::run()
{
    // bool connected = false;
    // int key = 0;
    login_menu.run();
    // // TODO check if connected
    // while (true) {
    //     // display login or main menu
    //     if (!connected) {
    //         main_menu.safe_post(false);
    //         login_menu.safe_post();
    //     } else {
    //         login_menu.safe_post(false);
    //         main_menu.safe_post();
    //     }
    //     // refresh(); // getch does refresh
    //     // handle input and take actions
    //     key = getch();
    //     if (connected) {
    //         main_menu.driver(key);
    //     } else {
    //         login_menu.driver(key);
    //     }
    // }
}
