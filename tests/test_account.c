#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "test.h"
#include "../src/core/account.h"
#include "../src/core/record.h"

#define PRT(n) printf("%s %.2lf %s\n", n->consumed, n->balance, n->pos->name);
#define PRTC(b, e) for (;b != NULL && b != e;b = b->next) PRT(b);printf("end\n");

int main()
{
    int cardno;
    struct account *account;
    E_ACCOUNT_ACCESS_TYPE state;
    E_ACCOUNT_CONSUME_TYPE cstate;
    struct consume_record *b, *e;

    /* account read / destory */
    scanf("%d", &cardno);
    state = account_read(cardno, &account);
    printf("%d from %s\n", account->cardno, account->faculty);
    printf("state: %d\n", state);

    /* (un)freeze */
    /*
    if (state == E_FREEZE)
        state = account_unfreeze(account);
    else
        state = account_freeze(account);
    
    printf("state: %d\n", state);
    */
    
    /* save */
    account->balance += 50.0;
    state = account_save(account);
    if (state == E_FREEZE)
        printf("cannot save\n");
    printf("state: %d\n", state);

    /* query */
    account_query_by_date(account, "20130109", &b, &e);
    PRTC(b, e);
    account_query_by_date(account, "20130110", &b, &e);
    PRTC(b, e);
    account_query_by_date(account, "20130113", &b, &e);
    PRTC(b, e);
    account_query_by_date(account, "20130114", &b, &e);
    PRTC(b, e);

    account_query_by_date_range(account, "20130109", "20130114", &b, &e);
    PRTC(b, e);
    account_query_by_date_range(account, "20130109", "20130101", &b, &e);
    PRTC(b, e);

    account_query_by_sum(account, 0.1, &b, &e);
    PRTC(b, e);
    account_query_by_sum(account, 0.2, &b, &e);
    PRTC(b, e);
    account_query_by_sum(account, 100.1, &b, &e);
    PRTC(b, e);

    /* sort */
    printf("sorting\n");
    account_sort_by_date(account, 0);
    b = account->record;
    PRTC(b, NULL);
    account_sort_by_date(account, 1);
    b = account->record;
    PRTC(b, NULL);

    /*
    account_sort_by_sum(account, 0);
    b = account->record;
    PRTC(b, NULL);
    account_sort_by_sum(account, 1);
    b = account->record;
    PRTC(b, NULL);

    account_sort_by_transcation(account, 0);
    b = account->record;
    PRTC(b, NULL);
    account_sort_by_transcation(account, 1);
    b = account->record;
    PRTC(b, NULL);
    */

    /* consume */
    printf("consume\n");
    cstate = account_consume(account, "20130622", "20130622", 10.0, CONSUME_POS, "test machine");
    printf("consume state: %d\n", cstate);
    b = account->record;
    PRTC(b, NULL);
    /*
    account_query_by_date(account, "20130109", &b, &e);
    account_consume_delete(account, b);
    b = account->record;
    PRTC(b, NULL);
    */
    printf("sorting\n");
    account_sort_by_date(account, 0);
    b = account->record;
    PRTC(b, NULL);
    account_sort_by_date(account, 1);
    b = account->record;
    PRTC(b, NULL);


    account_save(account);
    account_destory(account);
    return 0;
}
