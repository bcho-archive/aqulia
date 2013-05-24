/* display unit */
#ifndef UNIT_H
#define UNIT_H

#include "screen.h"

struct unit_buf {
    int x, y, line;
    char *buf;

    struct unit_buf *next;
};

/* basic unit */
struct unit_buf *unit_menu(char *list[], int length);
struct unit_buf *unit_normal(char *headline, char *main);
struct unit_buf *unit_hero(char *main);

/* extra :) */
struct unit_buf *unit_center_menu(char *list[], int length);
struct unit_buf *unit_big_menu(char *list[], int length);
struct unit_buf *unit_select_menu(struct unit_buf *menu, char *select_msg);

void unit_destory(struct unit_buf *unit);

#endif
