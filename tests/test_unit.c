#include "../src/vendor/conio.h"
#include "../src/cli/unit.h"

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
    getch();
    clrscr();
    
    unit_destory(b);

    b = unit_normal("hello", argv[argc - 1]);
    while (b != NULL) {
        gotoxy(b->x, b->y);
        cprintf("%s", b->buf);
        b = b->next;
    }
    getch();
    clrscr();

    unit_destory(b);

    b = unit_hero(argv[argc - 1]);
    while (b != NULL) {
        gotoxy(b->x, b->y);
        cprintf("%s", b->buf);
        b = b->next;
    }
    getch();
    clrscr();

    unit_destory(b);

    return 0;
}
