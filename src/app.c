#include "core/account.h"

#include "cli/screen.h"
#include "cli/unit.h"
#include "utils/misc.h"
#include "vendor/conio.h"

#include <stdlib.h>
#include <string.h>

#define LINE_MAX 1024

/* account */
// oh yes, global is evil, but don't panic :)
// yo, how's your day my VIP
struct account *potato;

/* helper message */
char BORDER_HELPER[] = "j/k: skip page, q: go upper level, x: exit";
char CHOICES[] = "Your choices: ";

/* basic routine */
void display(struct unit_buf *buf);
void app_exit(int errno);

/* screen */

/* parent */
void login_menu();
void main_menu();

/* first level children */
void query_menu();
void sort_menu();
void stat_page();
void remove_page();
void freeze_page();
void pos_page();
void help_page();

int main()
{
    login_menu();
    return 0;
}

void display(struct unit_buf *buf)
{
    clrscr();
    /* display unit */
    while (buf != NULL) {
        gotoxy(buf->x, buf->y);
        cprintf("%s", buf->buf);
        buf = buf->next;
    }
    /* display border */
    screen_draw_border(BORDER_HELPER);
}

void login_menu()
{
    int loop, cardno;
    char _cardno[19];
    char *msg = "Please tell me your cardno: ";
    struct unit_buf *frame;
    E_ACCOUNT_ACCESS_TYPE state;

    loop = 1;
    while (loop) {
        frame = unit_normal("Login Required", msg);
        display(frame);

        /* TODO use getch for the first char */
        cscanf("%s", _cardno);

        if (strcmp(_cardno, "x") == 0)                      /* exit eariler */
            app_exit(0);
        cardno = atoi(_cardno);
        if (cardno == 0) {
           msg = "Your cardno is wrong! Please check your input: ";
           continue;
        }

        if (!account_exists(cardno)) {
           msg = "Cardno doesn't exists! Please retype: ";
           continue;
        }

        state = account_read(cardno, &potato);

        if (state == E_FREEZE)
            cprintf("goto unfreeze page");
        else if (state == E_OK)
            main_menu();
        app_exit(0);                                        /* ensure our app will terminate here */
    }
}

void main_menu()
{
    int loop;
    char op;
    char *options[] = {
        "1. Query",
        "2. Sort",
        "3. Statistics",
        "4. Remove Record",
        "5. Freeze Card",
        "6. POS",
        "7. Help!"
    };
    struct unit_buf *menu;

    menu = unit_center_menu(options, sizeof(options) / sizeof(options[0]));
    menu = unit_select_menu(menu, CHOICES);
    loop = 1;
    while (loop) {
        display(menu);

        op = getche();

        switch (op) {
            case '1':
                query_menu();
                break;
            case '2':
                sort_menu();
                break;
            case '3':
                stat_page();
                break;
            case '4':
                cprintf("goto remove record");
                break;
            case '5':
                cprintf("goto freeze record");
                break;
            case '6':
                cprintf("goto pos");
                break;
            case '7':
                cprintf("goto help");
                break;
            case 'x' :
                app_exit(0);
                break;
            default:
                break;
        }
    }
}

void query_menu()
{
    int loop;
    char op;
    char *options[] = {
        "1. Query by date",
        "2. Query by date range",
        "3. Query by sum"
    };
    struct unit_buf *menu;

    menu = unit_center_menu(options, sizeof(options) / sizeof(options[0]));
    menu = unit_select_menu(menu, CHOICES);
    loop = 1;
    while (loop) {
        display(menu);

        op = getche();
        switch (op) {
            case '1':
                cprintf("goto query by date");
                break;
            case '2':
                cprintf("goto query by date range");
                break;
            case '3':
                cprintf("goto query by sum");
                break;
            case 'x':
                app_exit(0);
                break;
            case 'q':
                return;
        }
    }
}

void sort_menu()
{
    int loop;
    char op;
    char *options[] = {
        "1. Sort by date",
        "2. Sort by date (reverse)",
        "3. Sort by sum",
        "4. Sort by sum (reverse)",
        "5. Sort by transcation",
        "6. Sort by transcation (reverse)"
    };
    struct unit_buf *menu;

    menu = unit_center_menu(options, sizeof(options) / sizeof(options[0]));
    menu = unit_select_menu(menu, CHOICES);
    loop = 1;
    while (loop) {
        display(menu);

        op = getche();

        switch (op) {
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
                break;
            case 'x':
                app_exit(0);
                break;
            case 'q':
                return;
        }
    }
}

void stat_page()
{
    int loop, bf, ef;
    char op, b[LINE_MAX], e[LINE_MAX], *cur;
    struct unit_buf *unit;
    char *msg;

    msg = "Which date you want to start with? (e.g. 20130230) ";
    loop = 1;
    bf = 0; ef = 0;
    
    while (loop) {
        unit = unit_normal("Statistics", msg);
        display(unit);

        if (!bf || !ef) {
            if (!bf)
                cur = b;
            else
                cur = e;

            cscanf("%s", cur);
            if (strcmp(cur, "q") == 0)
                return;
            if (strcmp(cur, "x") == 0)
                app_exit(0);

            if (!date_validate(b)) {
                msg = "Oops, your input format is incorrect, "
                      "please make sure your input looks like 20130230: ";
                continue;
            } else {
                bf = 1;
                msg = "Now tell me the end date: ";
                if (!date_validate(e))
                    continue;
            }
            if (!date_validate(e)) {
                msg = "Oops, it seems that your input format is incorrect, "
                      "make sure your input looks like 20130230";
                continue;
            } else {
                if (strcmp(e, b) < 0) {
                    msg = "Oops, the end date is before the start date, try again: ";
                    continue;
                }
                ef = 1;
            }
        } else {
            cprintf("display sum");
            break;
        }
    }
}

void remove_page()
{
}

void freeze_page()
{

}

void pos_page()
{
}

void help_page()
{
}

void app_exit(int errno)
{
    clrscr();
    gotoxy(WIN_WIDTH / 2, WIN_HEIGHT / 2);
    cprintf("Bye!\n");

    exit(errno);
}
