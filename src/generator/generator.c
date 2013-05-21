#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

#include "../core/account.h"

#define YEAR 2013
#define LIMIT_INPUT "limit.in"
#define OUTPUT_PREFIX "./data/"
#define CARDS_OUPUT "cards.dat"
#define PERSONAL_OUTPUT "personal.dat"
#define FEE_OUTPUT "fee.dat"

int mdays[2][12] = {
    {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
    {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}
};

struct generate_limit {
    /* personal info */
    int cb, ce;                                             /* cardno */
    int eb, ee;                                             /* expire */
    double bb, be;                                          /* balance */
    card_state_t sb, se;                                    /* state */
    char school[5][50], faculty[5][50], major[5][50];       /* faculty */
    int sc, fc, mc;                                         /* faculty selection count */
    int ml, cl;                                             /* consume history */
    char pos[5][50];                                        /* pos machine */
    int pc;                                                 /* pos machine count */
    int tb, te;                                             /* pos transcation */
} limit = {
    14000000, 15000000,
    2012, 2016,
    0.0, 300.0,
    CARD_USABLE, CARD_FREEZED,
    {
        "GDUT"
    },
    {
        "computer"
    },
    {
        "CS", "SE", "NE"
    },
    1, 1, 3,
    3, 10,
    {
        "canteenA", "canteenB", "storeA", "storeB"
    },
    4,
    150, 850
};

void generate(struct generate_limit limit, int count);

int main(int argc, char *argv[])
{
    int count;

    count = 5;
    if (argc > 1)
        count = atoi(argv[argc - 1]);
    srand(time(NULL));
    generate(limit, count);
    return 0;
}

/* generate date */
inline int isleap(int year)
{
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

inline void generate_date(int yb, int ye, char *dest)
{
    int year, month, day;

    year = rand() % (ye - yb) + yb;
    month = rand() % 12 + 1;
    day = mdays[isleap(year)][month - 1];

    sprintf(dest, "%d%02d%02d", year, month, day);
}
/* generate date */

/* generate personal */
void generate_personal(struct generate_limit limit,
                       struct personal_info *person)
{
    person->cardno = rand() % (limit.ce - limit.cb) + limit.cb;
    generate_date(limit.eb, limit.ee, person->expire);
    person->balance = rand() % ((int) (limit.be - limit.bb)) + limit.bb;
    person->state = CARD_USABLE;
    sprintf(person->faculty, "/%s/%s/%s",
            limit.school[rand() % limit.sc],
            limit.faculty[rand() % limit.fc],
            limit.major[rand() % limit.mc]);
}
/* generate personal */

void save_personal(struct personal_info person)
{
    char fname[50];
    FILE *stream;

    sprintf(fname, OUTPUT_PREFIX"%d/"PERSONAL_OUTPUT, person.cardno);
    stream = fopen(fname, "w");
    fprintf(stream, "cardno,expire,balance,state,faculty\n");
    fprintf(stream, "%d,%s,%.2lf,%d,%s",
            person.cardno, person.expire, person.balance,
            (int) person.state, person.faculty);
    fclose(stream);
}

/* generate pos */
void generate_pos(struct generate_limit limit, struct pos_info *pos) {
    int no, num;

    no = rand() % 100;
    num = rand() % 10000000;

    sprintf(pos->name, "%s%02dpos%08d", limit.pos[rand() % limit.pc], no, num);
    pos->transcation = rand() % (limit.te - limit.tb) + limit.tb;
}
/* generate pos */

/* generate consume history */
char *generate_history(int year, int month, int day, double cost,
                       double balance, struct pos_info *pos)
{
    char *history;
    char buf[150];

    sprintf(buf, "%d%02d%02d,%d%02d%02d,%.2lf,%.2lf,%d,%d,%s",
            year, month, day, year, month, day,
            cost, balance, (int) CONSUME_POS, pos->transcation++, pos->name);
    history = malloc(sizeof(char) * (strlen(buf) + 1));
    strcpy(history, buf);
    return history;
}

void save_consume(struct generate_limit limit, struct personal_info *person,
                  struct pos_info *pos)
{
    int i, j, month, day;
    double cost;
    char *history;
    char fname[50];
    FILE *stream;

    sprintf(fname, OUTPUT_PREFIX"%d/"FEE_OUTPUT, person->cardno);
    stream = fopen(fname, "w");
    fprintf(stream, "consumed,received,sum,balance,consume_type,transcation,pos\n");

    month = rand() % 12 + 1;
    day = rand() % mdays[isleap(YEAR)][month - 1] + 1;
    for (i = 0;i < limit.ml && person->balance > 0.1;i++) {
        for (j = 0;j < limit.cl && person->balance > 0.1;j++) {
            do {
                cost = rand() % 25;

                if (rand() % 100 > 65)
                    cost /= 10;
            } while (person->balance - cost <= 0 || cost <= 0);
            person->balance -= cost;

            history = generate_history(YEAR, month, day, cost, person->balance,
                                       &pos[rand() % limit.pc]);

            fprintf(stream, "%s\n", history);
            free(history);
            /* printf("%s\n", history); */

            if (rand() % 100 > 45)
                day = (day + 1 > mdays[isleap(YEAR)][month - 1])\
                      ? (day) : (day + 1);
        }
        if (rand() % 100 > 5)
            if (month + 1 < 12) {
                month++;
                day = rand() % mdays[isleap(YEAR)][month - 1] + 1;
            }
    }

    fclose(stream);
}
/* generate history */

void prepeare_path(struct personal_info person)
{
    char command[30];

    sprintf(command, "mkdir -p "OUTPUT_PREFIX"%d", person.cardno);
    system(command);
}

void generate(struct generate_limit limit, int count)
{
    int i;
    struct pos_info *pos;
    struct personal_info person;
    FILE *stream;

    system("rm -rf "OUTPUT_PREFIX);
    system("mkdir -p "OUTPUT_PREFIX);

    /* generate pos */
    pos = malloc(sizeof(struct pos_info) * limit.pc);
    for (i = 0;i < limit.pc;i++)
        generate_pos(limit, pos + i);

    stream = fopen(OUTPUT_PREFIX CARDS_OUPUT, "w");
    for (i = 0;i < count;i++) {
        generate_personal(limit, &person);

        prepeare_path(person);
        save_consume(limit, &person, pos);
        save_personal(person);
        fprintf(stream, "%d\n", person.cardno);
    }

    free(pos);
    fclose(stream);
}
