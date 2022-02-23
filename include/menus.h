/*
** Project nobudget, 2022
**
** Author Francois Michaut
**
** Started on  Fri Feb 18 16:22:33 2022 Francois Michaut
** Last update Tue Feb 22 20:15:57 2022 Francois Michaut
**
** menus.h : Menu structures definitions
*/

#pragma once

typedef enum {
    MENU_MAIN,
    MENU_LOGIN,
    MENU_CONFIG,
    MENU_HELP,
} menu_id;

typedef struct {
    int menu_idx;
    int item_idx;
    int unknown_shortcut;
    char want_to_quit;
    char connected;
    char too_small;
    void *popup;
    int max_x;
    int max_y;
} menu_state;

typedef int (*item_callback)(menu_state *);

typedef struct {
    char shortcut;
    const char *title;
    item_callback callback;
    int next_menu;
} menu_item;

typedef struct {
    const char *title;
    menu_item items[];
} menu_t;
