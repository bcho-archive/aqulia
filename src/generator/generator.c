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
    20120901, 20160901,
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

void generate_personal(struct generate_limit limit,
                       struct personal_info *person)
{
    person->cardno = rand() % (limit.ce - limit.cb) + limit.cb;
    sprintf(person->expire, "%d", rand() % (limit.ee - limit.eb) + limit.eb);
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
