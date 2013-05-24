#include "screen.h"
#include "../vendor/conio.h"
#include "../utils/str.h"

#include <string.h>

void screen_draw_border(char *help_msg)
{
    int px, py;
    int i, msg_len;

    trim(help_msg);

    px = wherex(); py = wherey();
    gotoxy(0, 0);
    for (i = 0;i <= WIN_WIDTH;i++)
        cprintf("%c", BORDER_HOR);

    for (i = 1;i < WIN_HEIGHT;i++) {
        gotoxy(0, i);
        cprintf("%c", BORDER_VER);
        gotoxy(WIN_WIDTH, i);
        cprintf("%c", BORDER_VER);
    }

    gotoxy(0, WIN_HEIGHT);
    for (msg_len = strlen(help_msg), i = 0;i < (WIN_WIDTH - msg_len) / 2;i++)
        cprintf("%c", BORDER_HOR);
    cprintf("%s", help_msg);
    for (i += msg_len;i <= WIN_WIDTH;i++)
        cprintf("%c", BORDER_HOR);

    gotoxy(px, py);
}
