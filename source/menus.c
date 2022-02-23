/*
** Project nobudget, 2022
**
** Author Francois Michaut
**
** Started on  Fri Feb 18 16:23:34 2022 Francois Michaut
** Last update Wed Feb 23 19:11:28 2022 Francois Michaut
**
** menus.c : Menus implementation
*/

#include <ctype.h>
#include <fcntl.h>
#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "menus.h"
#include "nobudget_def.h"
#include "utils.h"

#define LOGIN_FILE "/tmp/.nobudget"

static int execute_command(const char *str);

static int logout_callback(menu_state *state);
static int login_callback(menu_state *state);

static const menu_t main_menu = {
   "Welcome to Nobudget !", {
      {'n', "New Guest", NULL, MENU_MAIN},
      {'m', "Manage Guests", NULL, MENU_MAIN},
      {'c', "Config", NULL, MENU_MAIN},
      {'h', "Help", NULL, MENU_MAIN},
      {'l', "Logout", logout_callback, MENU_LOGIN},
      {'q', "Quit", NULL, -1},
      {0},
   }
};

static const menu_t login_menu = {
   "Please log in", {
      {'l', "Login / Register", login_callback, MENU_MAIN},
      {'q', "Quit", NULL, -1},
      {0},
   }
};

static const menu_t *const menus[] = {
   [MENU_MAIN] = &main_menu,
   [MENU_LOGIN] = &login_menu,
};

static int execute_command(const char *str)
{
   if (str == NULL)
      return NOBUDGET_NO_ERROR;
   // TODO execute
   return NOBUDGET_NO_ERROR;
}

static int login_callback(menu_state *state)
{
   int fd = open(LOGIN_FILE, O_RDWR|O_CREAT, S_IRUSR | S_IRGRP | S_IROTH);

   close(fd);
   (void)execute_command;
   state->connected = TRUE;
   return NOBUDGET_NO_ERROR;
}

static int logout_callback(menu_state *state)
{
   remove(LOGIN_FILE);
   state->connected = FALSE;
   return NOBUDGET_NO_ERROR;
}

int handle_input_popup(menu_state *state)
{
   const char *items[] = {"Yes", "No"};
   const char *title = "Do you want to quit ?";
   const int len = strlen(title);
   const int size_y = 5;
   const int pos_y = (state->max_y / 2) - (size_y / 2);
   const int pos_x = (state->max_x / 2) - (len / 2);
   static bool selected = 1;
   int key = 0;
   int x = 0;

   if (state->popup == NULL) {
      state->popup = newwin(size_y, len + 4, pos_y, pos_x);
      selected = 1;
      keypad(state->popup, TRUE);
      //syncok(state->popup, TRUE);
   } else {
      mvwin(state->popup, pos_y, pos_x);
   }
   wclear(state->popup);
   box(state->popup, 0, 0);
   mvwprintw(state->popup, 1, 2, title);
   for (unsigned int i = 0; i < sizeof(items) / sizeof(char *); i++) {
      x = (i == 0 ? 2 : len - 4);
      if (selected == i) {
         wattr_on(state->popup, A_REVERSE, NULL);
         mvwprintw(state->popup, 3, x, items[i]);
         wattr_off(state->popup, A_REVERSE, NULL);
      } else {
         mvwprintw(state->popup, 3, x, items[i]);
      }
   }
   wrefresh(state->popup);
   key = tolower(wgetch(state->popup));
   if (key == KEY_LEFT || key == KEY_BTAB) {
      selected = 0;
      key = 0;
   } else if (key == KEY_RIGHT || key == KEY_STAB || key == '\t') {
      selected = 1;
      key = 0;
   } else if (key == KEY_ENTER || key == '\r' || key == '\n') {
      key = (selected == 1 ? 'n' : 'y');
   } else if (key == KEY_RESIZE) {
      key = 0;
      // Do nothing
   }

   if (key == 'y') {
      state->menu_idx = -1;
      delwin(state->popup);
      state->popup = NULL;
   } else if (key == 'n') {
      delwin(state->popup);
      state->popup = NULL;
      state->want_to_quit = FALSE;
   }
   return NOBUDGET_NO_ERROR;
}

int handle_input(const menu_t *menu, menu_state *state, int nb_items) {
   int key = tolower(getch());
   item_callback callback = NULL;

   state->unknown_shortcut = 0;
   if (state->too_small)
      return NOBUDGET_NO_ERROR;
   if (key == KEY_UP || key == KEY_BTAB) {
      state->item_idx--;
      key = 0;
   } else if (key == KEY_DOWN || key == KEY_STAB || key == '\t') {
      state->item_idx++;
      key = 0;
   } else if (key == KEY_ENTER || key == '\r' || key == '\n') {
      key = (unsigned char)menu->items[state->item_idx].shortcut;
   } else if (key == KEY_RESIZE) {
      key = 0;
      // Do nothing
   }
   state->item_idx = clamp(state->item_idx, 0, nb_items - 1);
   for (int i = 0; key != 0 && menu->items[i].shortcut != 0; i++) {
      if (menu->items[i].shortcut == key) {
         if (menu->items[i].next_menu != -1) {
            state->menu_idx = menu->items[i].next_menu;
         } else {
            state->want_to_quit = TRUE;
         }
         state->item_idx = 0;
         callback = menu->items[i].callback;
         if (callback != NULL)
            return callback(state);
         return NOBUDGET_NO_ERROR;
      }
   }
   // If key not found in list
   state->unknown_shortcut = key;
   return NOBUDGET_NO_ERROR;
}

static int get_border_height(int nb_items, int max_y)
{
   if (2 + nb_items * 2 >= max_y) {
      if (2 + nb_items > max_y) {
         return -1;
      }
   }
   return (max_y - (2 + nb_items * 2)) / 2;
}

bool print_str(const char *line, int pos_y, int max_x)
{
   int len = (int)strlen(line);

   if (len > max_x)
      return FALSE;
   if (pos_y < 0)
      pos_y = 0;
   mvprintw(pos_y, (max_x - len) / 2, line);
   return TRUE;
}

bool print_item(const menu_item *item, int pos_y, int max_x, bool selected)
{
   bool ret = TRUE;
   const unsigned long size = strlen(item->title) + 4;
   char *str = malloc(size + 1);

   if (!str)
      return FALSE; // TODO miss information
   str[0] = (char)toupper(item->shortcut);
   str[1] = ' ';
   str[2] = '-';
   str[3] = ' ';
   strcpy(str + 4, item->title); // NOLINT
   str[size] = 0;
   if (selected) {
      attr_on(A_REVERSE, NULL);
      print_str(str, pos_y, max_x);
      attr_off(A_REVERSE, NULL);
   } else {
      print_str(str, pos_y, max_x);
   }
   free(str);
   return ret;
}

int display_menu(const menu_t *menu, menu_state *state) {
   int nb_items = 0;
   int pos_y = 0;

   clear();
   getmaxyx(stdscr, state->max_y, state->max_x);
   while (menu->items[nb_items].shortcut != 0)
      nb_items++;
   pos_y = get_border_height(nb_items, state->max_y);
   if (pos_y == -1) {
      state->too_small = TRUE;
   } else  {
      state->too_small = !print_str(menu->title, pos_y - 1, state->max_x);
   }
   for (int i = 0; i < nb_items && !state->too_small; i++) {
      state->too_small = !print_item(&menu->items[i], pos_y + ((i + 1) * (pos_y >= 2 ? 2 : 1)),
                              state->max_x, state->item_idx == i);
   }
   if (state->unknown_shortcut != 0) {
      const char *msg = "Unknown shortcut '%s'.";
      const int len = strlen(msg);

      mvprintw(state->max_y - 1, (state->max_x - len) / 2, msg, keyname(state->unknown_shortcut));
      beep();
   }
   if (state->too_small) {
      clear();
      // TODO Display a better "Screen too swmall" message
      mvprintw(0, 0, "SCREEN TOO SMALL");
   }
   if (state->want_to_quit) {
      refresh();
      return handle_input_popup(state);
   }
   return handle_input(menu, state, nb_items);
}

static int init_color_pair()
{
   short default_pair[2];
   int ret = pair_content(0, &default_pair[0], &default_pair[1]);

   if (ret == ERR)
      return ret;
   return init_pair(1, default_pair[1], default_pair[0]);
}

int menu_loop() {
   int ret = NOBUDGET_NO_ERROR;
   menu_state state = {MENU_LOGIN, 0, 0, FALSE, FALSE, FALSE, NULL, 0, 0};
   const int nb_menus = sizeof(menus) / sizeof(menu_t *);

   if (init_color_pair() == ERR)
      return NOBUDGET_FATAL_ERROR; // TODO print error
   if (access(LOGIN_FILE, F_OK ) == 0) {
      state.connected = TRUE;
   }
   while (ret == NOBUDGET_NO_ERROR && state.menu_idx != -1) {
      if (!state.connected) {
         state.menu_idx = MENU_LOGIN;
      }
      if (state.menu_idx >= nb_menus)
         return NOBUDGET_FATAL_ERROR;
      ret = display_menu(menus[state.menu_idx], &state);
   }
   return ret;
}
