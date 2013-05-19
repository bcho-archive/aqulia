#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

#include "../core/account.h"

#define LIMIT_INPUT "limit.in"
#define OUTPUT_PREFIX "./data/"
#define PERSONAL_OUTPUT "personal.dat"
#define FEE_OUTPUT "fee.dat"

struct generate_limit {
    /* personal info */
    int cb, ce;                                             /* cardno */
    int eb, ee;                                             /* expire */
    double bb, be;                                          /* balance */
    card_state_t sb, se;                                    /* state */
    char school[5][50], faculty[5][50], major[5][50];       /* faculty */
    int sc, fc, mc;                                         /* selection count */
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
    1, 1, 3
};

void generate(struct generate_limit limit);

int main()
{
    srand(time(NULL));
    generate(limit);
    return 0;
}

inline int isleap(int year)
{
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

inline void generate_date(int yb, int ye, char *dest)
{
    int mdays[2][12] = {
        {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
        {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}
    };
    int year, month, day;

    year = rand() % (ye - yb) + yb;
    month = rand() % 12 + 1;
    day = mdays[isleap(year)][month - 1];

    sprintf(dest, "%d%02d%02d", year, month, day);
}

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

void prepeare_path(struct personal_info person)
{
    char command[30];

    sprintf(command, "mkdir -p "OUTPUT_PREFIX"%d", person.cardno);

    /* dangerous */
    system(command);
}

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

void generate(struct generate_limit limit)
{
    struct personal_info person;

    generate_personal(limit, &person);

    prepeare_path(person);
    save_personal(person);
}
