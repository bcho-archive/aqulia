#include "screen.h"
#include "unit.h"
#include <conio.h>
#include "debug.h"
#include "str.h"

#include <stdlib.h>
#include <string.h>

#define NORMAL_UNIT_PADDING 5

/* create a text block which width is shorter than line_width */
static char *
create_block(char *src, int line_width, int leftpadding, int *line)
{
    int msg_width, total, i, j;
    char *block, *p, *s;

    msg_width = strlen(src);
    if (msg_width == 0)
        return NULL;
    *line = msg_width / line_width + 1;
    total = msg_width * *line + leftpadding * (*line - 1) + 1;
    if ((block = malloc(sizeof(char) * total)) == NULL)
        ERROR("malloc");
    memset(block, 0, total);
    for (p = block, s = src, i = *line;i > 1;i--) {
        strncpy(p, s, line_width);
        *(p + line_width) = '\n';
        p += line_width + 1;
        s += line_width; 
        for (j = 0;j < leftpadding;j++, p++)
            *p = ' ';
    }
    msg_width -= (*line - 1) * line_width;
    strncpy(p, s, msg_width);
    *(p + msg_width) = '\0';
    return block;
}

static struct unit_buf *
menu(char *list[], int length, int line_width, int xoffset, int yoffset)
{
    int line, x, y, i;
    struct unit_buf *head, *prev, *p;

    head = NULL;
    x = xoffset; y = yoffset;
    for (i = 0;i < length;i++) {
        if ((p = malloc(sizeof(struct unit_buf))) == NULL)
            ERROR("malloc");
        p->next = NULL;
        p->buf = create_block(list[i], line_width, xoffset, &line);
        p->x = x;
        p->y = y;
        p->line = line;
        y += line + PADDING;

        if (head == NULL)
            head = p;
        else
            prev->next = p;
        prev = p;
    }

    return head;
}

struct unit_buf *
unit_menu(char *list[], int length)
{
    return menu(list, length, INNER_WIDTH / 2 - PADDING * 2,
                BORDER_WIDTH + PADDING, BORDER_WIDTH + PADDING);
}

struct unit_buf *
unit_normal(char *headline, char *main)
{
    int line, line_width;
    struct unit_buf *_headline, *_main;

    if ((_headline = malloc(sizeof(struct unit_buf))) == NULL)
        ERROR("malloc");
    if ((_main = malloc(sizeof(struct unit_buf))) == NULL)
        ERROR("malloc");
    _headline->next = _main;
    _main->next = NULL;

    _headline->x = PADDING + BORDER_WIDTH;
    _headline->y = PADDING + BORDER_WIDTH;
    _headline->buf = create_block(headline, INNER_WIDTH,
                                  BORDER_WIDTH + PADDING, &line);
    _headline->line = line;

    _main->x = _headline->x + NORMAL_UNIT_PADDING;
    _main->y = _headline->y + line + PADDING;
    line_width = INNER_WIDTH - 2 * NORMAL_UNIT_PADDING;
    _main->buf = create_block(main, line_width,
                              BORDER_WIDTH + PADDING + NORMAL_UNIT_PADDING,
                              &line);
    _main->line = line;

    return _headline;
}

struct unit_buf *
unit_hero(char *main)
{
    int line, line_width;
    struct unit_buf *_main;
    
    if ((_main = malloc(sizeof(struct unit_buf))) == NULL)
        ERROR("malloc");
    _main->next = NULL;
    
    line_width = INNER_WIDTH - 2 * NORMAL_UNIT_PADDING;
    _main->buf = create_block(main, line_width,
                              BORDER_WIDTH + PADDING + NORMAL_UNIT_PADDING,
                              &line);
    _main->x = BORDER_WIDTH + PADDING + NORMAL_UNIT_PADDING;
    _main->y = (INNER_HEIGHT - line) / 2;

    return _main;
}

void
unit_destory(struct unit_buf *unit)
{
    struct unit_buf *prev;

    while (unit != NULL) {
        prev = unit->next;
        free(unit->buf);
        free(unit);
        unit = prev;
    }
}

struct unit_buf *
unit_center_menu(char *list[], int length)
{
    return menu(list, length, INNER_WIDTH - PADDING * 2,
                BORDER_WIDTH + PADDING + NORMAL_UNIT_PADDING,
                BORDER_WIDTH + PADDING + NORMAL_UNIT_PADDING);
}

struct unit_buf *
unit_big_menu(char *list[], int length)
{
    return menu(list, length, INNER_WIDTH - PADDING * 2,
                BORDER_WIDTH + PADDING, BORDER_WIDTH + PADDING);
}

struct unit_buf *
unit_select_menu(struct unit_buf *menu, char *select_msg)
{
    struct unit_buf *_select, *p;

    for (p = menu;p != NULL && p->next != NULL;p = p->next)
        ;
    if (p != NULL) {
        _select = unit_hero(select_msg);
        _select->y = p->y + p->line + PADDING;
        p->next = _select;
    }
    return menu;
}
