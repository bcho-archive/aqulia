/* display unit */
#ifndef UNIT_H
#define UNIT_H

#include "screen.h"

struct unit_buf {
    int x, y;
    char *buf;

    struct unit_buf *next;
};

struct unit_buf *unit_menu(char *list[], int length);
struct unit_buf *unit_normal(char *headline, char *main);
struct unit_buf *unit_hero(char *main);
void unit_destory(struct unit_buf *unit);

#endif
