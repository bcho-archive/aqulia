#include "../vendor/conio.h"

#define WIN_WIDTH 80
#define WIN_HEIGHT 20
#define BORDER '*'

void draw_border(int width, int height, char border);
void draw_p();

int main()
{
    int c;
foo:
    draw_border(WIN_WIDTH, WIN_HEIGHT, BORDER);
    c = getch();

    if (c == 'a')
        draw_p();
    goto foo;
    return 0;
}

void draw_border(int width, int height, char border)
{
    int i;

    gotoxy(0, 0);
    for (i = 0;i < width;i++)
        cprintf("%c", border);
    gotoxy(0, height);
    for (i = 0;i < width;i++)
        cprintf("%c", border);
    gotoxy(0, 0);
}

void draw_p()
{
    return;
}
