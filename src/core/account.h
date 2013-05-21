#ifndef ACCOUNT_H
#define ACCOUNT_H

#include "personal_info.h"
#include "consume_record.h"

struct account {
    struct personal_info *info;
    struct consume_record *record;

    char *personal_file;
    char *freeze_file;
    char *consume_record;
    char *consume_undo;
};

#define DAILY_CONSUME_LIMIT 50
#endif
