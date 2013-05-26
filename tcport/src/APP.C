#include "account.h"

#include "screen.h"
#include "unit.h"
#include "misc.h"
#include "debug.h"
#include <conio.h>

#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define LINE_MAX 1024
#define RECORD_MAX 100
#define RECORD_DISPLAY_MAX 7
#define APP_NAME "GDUT_POS"

/* account */
/* oh yes, global is evil, but don't panic :) */
/* yo, how's your day my VIP */
struct account *potato;

/* helper message */
char BORDER_HELPER[] = "j/k: skip page, q: go upper level, x: exit";
char CHOICES[] = "Your choices: ";

/* basic routine */
void display(struct unit_buf *buf);
void app_exit(int errno);
/* display consume record with j/k scrolling and detail viewing */
int display_consume_record(struct consume_record *b, struct consume_record *e);
/* convert consume records to string */
int record_str(char *buf[], struct consume_record **d, struct consume_record *e);

/* screen */

/* parent */
void login_menu();
void main_menu();
void unfreeze_page();
void relogin_page();

/* first level children */
void personal_page();
void query_menu();
void sort_menu();
void stat_page();
void remove_page();
void freeze_page();
void pos_page();
void help_page();

/* second level children */
void query_by_date();
void query_by_date_range();
void query_by_sum();

void sort_by_date(int reverse);
void sort_by_sum(int reverse);
void sort_by_transcation(int reverse);

/* third level children */
void delete_record(struct consume_record *record);

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
    long int loop, cardno;
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
	   cardno = atol(_cardno);
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
            unfreeze_page();
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
        "0. Personal infomation",
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
            case '0':
                personal_page();
                break;
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
                remove_page();
                break;
            case '5':
                freeze_page();
                break;
            case '6':
                pos_page();
                break;
            case '7':
                help_page();
                break;
            case 'x' :
                app_exit(0);
                break;
            default:
                break;
        }
    }
}

void unfreeze_page()
{
    int loop;
    char msg[LINE_MAX], op;
    struct unit_buf *unit;

    E_ACCOUNT_ACCESS_TYPE state;

    sprintf(msg, "Your account (%ld) had been frozen, "
                 "do you want to unfreeze it?(y or n) ", potato->cardno);
    unit = unit_hero(msg);

    if ((state = account_validate(potato)) == E_OK)
        return;

    loop = 1;
    while (loop) {
        display(unit);

        op = getche();

        switch (op) {
            case 'y':
            case 'Y':
                state = account_unfreeze(potato);
                if (state == E_OK)
                    relogin_page();
            case 'n':
            case 'N':
                login_menu();
            case 'q':
                return;
            case 'x':
                app_exit(0);
                break;
        }
    }
}

void relogin_page()
{
    struct unit_buf *unit;

    unit = unit_hero("Now please login again. ");
    display(unit);
    getch();
    login_menu();
}

void personal_page()
{
    int loop;
    char buf[LINE_MAX];
    struct unit_buf *unit;

    sprintf(buf, "Hi %ld from %s, your remain balance is %.2lf. "
                 "And your card will expire in %s",
                 potato->cardno, potato->faculty, potato->balance,
                 potato->expire);
    unit = unit_hero(buf);
    display(unit);
    getch();
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
                query_by_date();
                break;
            case '2':
                query_by_date_range();
                break;
            case '3':
                query_by_sum();
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
                sort_by_date(0);
                break;
            case '2':
                sort_by_date(1);
                break;
            case '3':
                sort_by_sum(0);
                break;
            case '4':
                sort_by_sum(1);
                break;
            case '5':
                sort_by_transcation(0);
                break;
            case '6':
                sort_by_transcation(1);
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
    char op[LINE_MAX], b[LINE_MAX], e[LINE_MAX], buf[LINE_MAX], *cur;
    struct unit_buf *unit;
    char *msg;
    double sum;
    E_ACCOUNT_ACCESS_TYPE state;
    struct consume_record *be, *ee;

    msg = "Which date you want to start with? (e.g. 20130230) ";
    loop = 1;
    bf = 0; ef = 0;
    memset(b, LINE_MAX, 0);
    memset(e, LINE_MAX, 0);

    while (loop) {
        unit = unit_normal("Statistics", msg);
        display(unit);

        if (!bf || !ef) {
            if (!bf)
                cur = b;
            else
                cur = e;
		  memset(cur, LINE_MAX, 0);
            cscanf("%s", cur);
            if (strcmp(cur, "q") == 0)
                return;
            if (strcmp(cur, "x") == 0)
                app_exit(0);

            if (!date_validate(b)) {
                bf = 0;
                msg = "Oops, your input format is incorrect, "
                      "please make sure your input looks like 20130230: ";
                continue;
            } else {
                bf = 1;
			 msg = "Now tell me the end date: ";
            }
		  if (!date_validate(e)) {
                ef = 0;
                continue;
            } else {
                if (strcmp(e, b) < 0) {
                    msg = "Oops, the end date is before the start date, try again: ";
                    continue;
                }
                ef = 1;
            }
        } else {
            if ((state = account_query_by_date_range(potato, b, e, &be, &ee)) != E_OK)
                unfreeze_page();
            for (sum = 0;be != NULL && be != ee;be = be->next)
                if (be->type == CONSUME_POS)
                    sum += be->sum;
            sprintf(buf, "From %s to %s, you had consumed %.2lf.", b, e, sum);
            unit = unit_normal("Statistics", buf);
            display(unit);
		  getche();
		  getche();
		  break;
        }
    }
}

void remove_page()
{
    int loop, length, select;
    char op, *options[RECORD_MAX];
    struct unit_buf *unit;
    E_ACCOUNT_ACCESS_TYPE state;
    struct consume_record *record, *b, *e;

    if ((state = account_sort_by_date(potato, 0)) != E_OK)
        unfreeze_page();

    b = potato->record; e = NULL;
    length = record_str(options, &b, e);
    unit = unit_center_menu(options, length);
    unit = unit_select_menu(unit, CHOICES);
    loop = 1;
    while (loop) {
        record = potato->record;
        display(unit);

        op = getche();

        switch (op) {
            case 'q':
                return;
            case 'x':
                app_exit(0);
                break;
            case 'j':
                length = record_str(options, &b, e);
                if (length)
                    unit = unit_center_menu(options, length);
                break;
            /* TODO j back */
            case 'k':
                length = record_str(options, &b, e);
                if (length)
                    unit = unit_center_menu(options, length);
                break;
            default:
                if (isalnum(op)) {
                    select = op - '0';
                    while (select-- > 1 && record != NULL)
                        record = record->next;
                    if (record != NULL) {
                        delete_record(record);
                        if ((state = account_sort_by_date(potato, 0)) != E_OK)
                            unfreeze_page();
                        b = potato->record;
                        length = record_str(options, &b, e);
                        unit = unit_center_menu(options, length);
                        unit = unit_select_menu(unit, CHOICES);
                    }
                }
        }
    }
}

void freeze_page()
{
    int loop;
    char op;
    struct unit_buf *unit;

    E_ACCOUNT_ACCESS_TYPE state;

    if ((state = account_validate(potato)) != E_OK)
        unfreeze_page();

    loop = 1;
    while (loop) {
        unit = unit_hero("This will freeze your card, are you sure?(y or n) ");
        display(unit);

        op = getche();

        switch (op) {
            case 'y':
            case 'Y':
                state = account_freeze(potato);
                if (state == E_FREEZE)
                    unfreeze_page();
                break;

            case 'n':
            case 'N':
            case 'q':
                return;
            case 'x':
                app_exit(0);
                break;
        }
    }
}

void pos_page()
{
    int loop, _type;
    unsigned int flag;
    struct unit_buf *unit;
    char buf[LINE_MAX];
    char *consumed, *pos, *msg;
    double sum;
    CONSUME_TYPE_T consume_type;
    E_ACCOUNT_CONSUME_TYPE state;

    loop = 1; flag = 8;
    msg = "Please input consumed date. (e.g. 20130230) ";
    pos = APP_NAME;

    while (loop) {
        if (flag == (1 | 2 | 4 | 8)) {
            state = account_consume(potato, consumed, consumed, sum, consume_type, pos);
            if (state == E_CONSUME_OK) {
                unit = unit_hero("OK!");
                display(unit);
			 getch();
			 getch();
                return;
            } else {
                unit = unit_hero("Failed! Please (don't) contact admin.");
                display(unit);
			 getch();
			 getch();
                flag = 0;
            }
        } else {
            unit = unit_normal("Create record", msg);
            display(unit);
            cscanf("%s", buf);

            if (strcmp(buf, "q") == 0)
                return;
            if (strcmp(buf, "x") == 0)
                app_exit(0);

            if ((flag & 1) == 0) {
                if (date_validate(buf)) {
                    consumed = strdup(buf);
                    flag |= 1;
                    msg = "Now tell me the sum: ";
                    continue;
                } else {
                    msg = "Oops... It seems that your date format is wrong! ";
                    continue;
                }
            }
            if ((flag & 2) == 0) {
                sum = atof(buf);
                if (sum < 0) {
                    msg = "Oops... The sum cannot be negative! ";
                    continue;
                } else {
                    flag |= 2;
                    msg = "... and type?(0 for pos or 1 for recharge) ";
                    continue;
                }
            }
            if ((flag & 4) == 0) {
                if (!(strlen(buf) == 1 && isalnum(buf[0]))) {
                    msg = "Oops... Please choose between 0 and 1 ";
                    continue;
                } else {
                    _type = atoi(buf);
                    if (_type == (int) CONSUME_POS) {
                        consume_type = CONSUME_POS;
                    } else if (_type == (int) CONSUME_RECHARGE) {
                        consume_type = CONSUME_RECHARGE;
                    } else {
                        msg = "Oops... Please choose between 0 and 1 ";
                        continue;
                    }
                    flag |= 4;
                    continue;
                }
            }
        }
    }
}

void help_page()
{
    int loop;
    char op;
    struct unit_buf *unit;

    loop = 1;
    while (loop) {
	   unit = unit_normal("GDUT POS", "To scroll page, press j/k."
					  "Press q to return parent level and x to terminate the program.");
        display(unit);

        op =  getche();

        switch (op) {
            case 'x':
                app_exit(0);
                break;
		  case 'q':
		  default:
                return;
        }
    }
}

void app_exit(int errno)
{
    account_save(potato);
    clrscr();
    gotoxy(WIN_WIDTH / 2, WIN_HEIGHT / 2);
    cprintf("Bye!\n");

    exit(errno);
}

void query_by_date()
{
    int loop;
    char date[LINE_MAX];
    struct unit_buf *unit;
    char *msg;

    E_ACCOUNT_ACCESS_TYPE state;
    struct consume_record *b, *e;

    msg = "Which date you want to check? (e.g. 20130230) ";
    loop = 1;

    while (loop) {
        unit = unit_normal("Query by date", msg);
        display(unit);

        cscanf("%s", date);
        if (strcmp(date, "q") == 0)
            return;
        if (strcmp(date, "x") == 0)
            app_exit(0);

        if (!date_validate(date)) {
                msg = "Oops, your input format is incorrect, "
                      "please make sure your input looks like 20130230: ";
                continue;
        } else {
            state = account_query_by_date(potato, date, &b, &e);
            if (state == E_OK) {
                display_consume_record(b, e);
                break;
            } else {
                unfreeze_page();
                break;
            }
        }
    }
}

void query_by_date_range()
{
    int loop, bf, ef;
    char bd[LINE_MAX], ed[LINE_MAX], *cur;
    struct unit_buf *unit;
    char *msg;

    E_ACCOUNT_ACCESS_TYPE state;
    struct consume_record *b, *e;

    msg = "Which date you want to start with? (e.g. 20130230) ";
    loop = 1;
    bf = 0; ef = 0;

    while (loop) {
        unit = unit_normal("Query by date range", msg);
        display(unit);

        if (!bf || !ef) {
            cur = (bf) ? ed : bd;

            cscanf("%s", cur);
            if (strcmp(cur, "q") == 0)
                return;
            if (strcmp(cur, "x") == 0)
                app_exit(0);

            if (!date_validate(bd)) {
                bf = 0;
                msg = "Oops, your input format is incorrect, "
                      "please make sure your input looks like 20130230: ";
                continue;
            } else {
                bf = 1;
                msg = "Now tell me the end date: ";
                if (!date_validate(ed))
                    continue;
            }
            if (!date_validate(ed)) {
                ef = 0;
                msg = "Oops, it seems that your input format is incorrect, "
                      "make sure your input looks like 20130230";
                continue;
            } else {
                if (strcmp(ed, bd) < 0) {
                    msg = "Oops, the end date is before the start date, try again: ";
                    continue;
                }
                ef = 1;
            }
        } else {
            state = account_query_by_date_range(potato, bd, ed, &b, &e);
            if (state == E_OK) {
                display_consume_record(b, e);
                break;
            } else {
                unfreeze_page();
            }
        }
    }
}

void query_by_sum()
{
    int loop;
    double sum;
    char _sum[LINE_MAX];
    struct unit_buf *unit;
    char *msg;

    E_ACCOUNT_ACCESS_TYPE state;
    struct consume_record *b, *e;

    msg = "Tell me the borderline sum: ";
    loop = 1;

    while (loop) {
        unit = unit_normal("Query by sum", msg);
        display(unit);

        cscanf("%s", _sum);
        if (strcmp(_sum, "q") == 0)
            return;
        if (strcmp(_sum, "x") == 0)
            app_exit(0);
        sum = atof(_sum);
        if (sum <= 0) {
            msg = "Oops, your input seems wrong, try again? ";
            continue;
        }

        state = account_query_by_sum(potato, sum, &b, &e);
        if (state == E_OK) {
            display_consume_record(b, e);
            break;
        } else {
            unfreeze_page();
            break;
        }
    }
}

void sort_by_date(int reverse)
{
    E_ACCOUNT_ACCESS_TYPE state;

    if ((state = account_sort_by_date(potato, reverse)) != E_OK)
        unfreeze_page();

    display_consume_record(potato->record, NULL);
}

void sort_by_sum(int reverse)
{
    E_ACCOUNT_ACCESS_TYPE state;

    if ((state = account_sort_by_sum(potato, reverse)) != E_OK)
        unfreeze_page();

    display_consume_record(potato->record, NULL);
}

void sort_by_transcation(int reverse)
{
    E_ACCOUNT_ACCESS_TYPE state;

    if ((state = account_sort_by_transcation(potato, reverse)) != E_OK)
        unfreeze_page();

    display_consume_record(potato->record, NULL);
}

int record_str(char *buf[], struct consume_record **d, struct consume_record *e)
{
    char str[LINE_MAX], *type;
    int c;
    struct consume_record *b;

    for (b = *d,c = 0;
         b != NULL && b != e && c < RECORD_DISPLAY_MAX;
         b = b->next, c++) {
        if (b->type == CONSUME_POS)
            type = "pos";
        else if (b->type == CONSUME_RECHARGE)
            type = "recharge";
        sprintf(str, "%d. %s sum: %.2lf balance: %.2lf in %s(%d, %s)",
                c + 1, b->consumed, b->sum, b->balance,
                b->pos->name, b->pos->transcation, type);
        buf[c] = strdup(str);
    }
    *d = b;

    return c;
}

int display_consume_record(struct consume_record *b, struct consume_record *e)
{
    int loop, length;
    char op;
    char *options[RECORD_MAX];
    struct unit_buf *menu;

    length = record_str(options, &b, e);
    menu = unit_center_menu(options, length);

    loop = 1;
    while (loop) {
        display(menu);

        op = getche();

        switch (op) {
            case 'q':
                return 0;
            case 'x':
                app_exit(0);
                break;
            case 'j':
                length = record_str(options, &b, e);
                if (length)
                    menu = unit_center_menu(options, length);
                break;
            /* TODO j back */
            case 'k':
                length = record_str(options, &b, e);
                if (length)
                    menu = unit_center_menu(options, length);
                break;
            default:
                if (isalnum(op))
                    return op - '0';
        }
    }
    return 0;
}

void delete_record(struct consume_record *record)
{
    char op, msg[LINE_MAX];
    struct unit_buf *unit;
    E_ACCOUNT_CONSUME_TYPE state;

    if (record == NULL)
        return;
    sprintf(msg, "Are you sure to remove record %s (%s)?(y or n) ",
            record->consumed, record->pos->name);
    unit = unit_hero(msg);
    display(unit);
    op = getche();

    switch (op) {
        case 'y':
        case 'Y':
            if ((state = account_consume_delete(potato, record)) != E_CONSUME_OK) {
                unit = unit_hero("Error! Please (don't) contact admin!");
                display(unit);
                getch();
                app_exit(1);
            } else {
                unit = unit_hero("Removed!");
                display(unit);
                getch();
            }
            return;
        case 'n':
        case 'N':
        case 'q':
            return;
        case 'x':
            app_exit(0);
    }
}
