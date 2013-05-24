#include "../src/vendor/conio.h"
#include "../src/cli/unit.h"
#include "../src/cli/screen.h"

int main(int argc, char *argv[])
{
    struct unit_buf *b;

    if (argc < 2)
        return 1;

    clrscr();
    b = unit_menu(argv, argc);
    while (b != NULL) {
        gotoxy(b->x, b->y);
        cprintf("%s", b->buf);
        b = b->next;
    }
    screen_draw_border(argv[0]);
    getch();
    clrscr();
    
    unit_destory(b);

    b = unit_normal("hello", argv[argc - 1]);
    while (b != NULL) {
        gotoxy(b->x, b->y);
        cprintf("%s", b->buf);
        b = b->next;
    }
    screen_draw_border(argv[0]);
    getch();
    clrscr();

    unit_destory(b);

    b = unit_hero(argv[argc - 1]);
    while (b != NULL) {
        gotoxy(b->x, b->y);
        cprintf("%s", b->buf);
        b = b->next;
    }
    screen_draw_border(argv[0]);
    getch();
    clrscr();

    unit_destory(b);

    return 0;
}
