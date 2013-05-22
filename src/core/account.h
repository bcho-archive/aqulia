#ifndef ACCOUNT_H
#define ACCOUNT_H

#include "personal_info.h"
#include "consume_record.h"

#define PERSONAL_INFO_FILE "personal.dat"
#define CONSUME_RECORD_FILE "fee.dat"
#define FREEZE_FILE "freeze.dat"
#define CONSUME_UNDO_FILE "fee_undo.dat"

typedef enum {
    E_OK,
    E_FREEZE
} E_ACCOUNT_READ_TYPE;

struct account {
    struct personal_info *info;
    struct consume_record *record;

    char *personal_file;
    char *freeze_file;
    char *consume_record;
    char *consume_undo;
};

E_ACCOUNT_READ_TYPE account_read(int cardno, struct account **account);
void account_save(struct account *account);
void account_destory(struct account *account);

/* validate */
E_ACCOUNT_READ_TYPE account_validate(struct account *account);

/* query */
E_ACCOUNT_READ_TYPE account_query_by_date(struct account *account, char *date,
                                          struct consume_record **begin,
                                          struct consume_record **end);
E_ACCOUNT_READ_TYPE account_query_by_date_range(struct account *account,
                                                char *lower, char *upper,
                                                struct consume_record **begin,
                                                struct consume_record **end);
E_ACCOUNT_READ_TYPE account_query_by_sum(struct account *account, double sum,
                                         struct consume_record **begin,
                                         struct consume_record **end);

/* sort */
E_ACCOUNT_READ_TYPE account_sort_by_date(struct account *account, int reverse);
E_ACCOUNT_READ_TYPE account_sort_by_sum(struct account *account, int reverse);
E_ACCOUNT_READ_TYPE account_sort_by_transcation(struct account *account, int reverse);

/* freeze */
E_ACCOUNT_READ_TYPE account_freeze(struct account *account);
E_ACCOUNT_READ_TYPE account_unfreeze(struct account *account);

/* consume */

#define DAILY_CONSUME_LIMIT 50
#endif
