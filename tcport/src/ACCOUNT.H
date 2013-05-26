/* account */
#ifndef ACCOUNT_H
#define ACCOUNT_H

#include "record.h"

typedef enum {
    CARD_USABLE,
    CARD_FREEZED,
    CARD_LOST
} CARD_STATE_T;

typedef enum {
    E_OK,
    E_FREEZE
} E_ACCOUNT_ACCESS_TYPE;

typedef enum {
    E_CONSUME_OK,
    E_CONSUME_FREEZED,
    E_CONSUME_CANTPAY,
    E_CONSUME_NOTFOUND,
    E_CONSUME_FAILED
} E_ACCOUNT_CONSUME_TYPE;

/* default file name */
#define DATA_PREFIX "data\\"
#define ACCOUNT_LIST "accounts.dat"
#define FPERSONAL "personal.dat"
#define FFREEZE "freeze.dat"
#define FCONSUME_RECORD "fee.dat"
#define FCONSUME_UNDO "fee_undelete.dat"

struct account {
    /* data file name */
    char *fpersonal;
    char *ffreeze;
    char *fconsume_record;
    char *fconsume_undo;

    /* personal infomation */
    long int cardno;
    char *expire;
    double balance;
    CARD_STATE_T state;
    char *faculty;

    /* consume record */
    struct consume_record *record;
};

E_ACCOUNT_ACCESS_TYPE account_read(long int cardno, struct account **account_return);
E_ACCOUNT_ACCESS_TYPE account_save(struct account *account);
void account_save_force(struct account *account);
void account_destory(struct account *account);

/* freeze */
E_ACCOUNT_ACCESS_TYPE account_freeze(struct account *account);
E_ACCOUNT_ACCESS_TYPE account_unfreeze(struct account *account);

/* consume */
E_ACCOUNT_CONSUME_TYPE
account_consume(struct account *account,
                const char *consumed, const char *received,
                double sum, CONSUME_TYPE_T consume_type, const char *pos);
E_ACCOUNT_CONSUME_TYPE
account_consume_delete(struct account *account, struct consume_record *record);

/* validate */
int account_exists(long int cardno);
E_ACCOUNT_ACCESS_TYPE account_validate(struct account *account);

/* query */
E_ACCOUNT_ACCESS_TYPE
account_query_by_date(struct account *account, char *date,
                      struct consume_record **begin_return,
                      struct consume_record **end_return);
E_ACCOUNT_ACCESS_TYPE
account_query_by_date_range(struct account *account,
                            char *lower, char *upper,
                            struct consume_record **begin_return,
                            struct consume_record **end_return);
E_ACCOUNT_ACCESS_TYPE
account_query_by_sum(struct account *account, double sum,
                     struct consume_record **begin_return,
                     struct consume_record **end_return);

/* sort */
E_ACCOUNT_ACCESS_TYPE
account_sort_by_date(struct account *account, int reverse);
E_ACCOUNT_ACCESS_TYPE
account_sort_by_sum(struct account *account, int reverse);
E_ACCOUNT_ACCESS_TYPE
account_sort_by_transcation(struct account *account, int reverse);

#endif
