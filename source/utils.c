/*
** Project nobudget, 2022
**
** Author Francois Michaut
**
** Started on  Sat Feb 19 14:46:08 2022 Francois Michaut
** Last update Sun Feb 20 14:58:45 2022 Francois Michaut
**
** utils.c : Utility functions implementation
*/

#include "utils.h"

int clamp(int nb, int min, int max) {
    const int t = nb < min ? min : nb;
    return t > max ? max : t;
}
