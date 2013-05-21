#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "test.h"
#include "../src/core/consume_record.h"

#define PRT(record) printf("consumed: %s received: %s\n"\
                           "sum: %.2lf balance: %.2lf\n"\
                           "type: %d\ntranscation: %d pos: %s\n\n",\
                           record->consumed, record->received, record->sum,\
                           record->balance, record->consume_type, record->pos->transcation,\
                           record->pos->name)

int main()
{
    struct consume_record *record, *p;
    char *data = TESTCASES_DIR"/fee.dat";

    record = consume_record_read(data);
    for (p = record;p != NULL;p = p->next)
        PRT(p);
    record->balance = 1000.0;
    consume_record_save(data, record);
    consume_record_destory(record);
    
    printf("---------------\n");
    record = consume_record_read(data);
    PRT(record);
    consume_record_destory(record);

    return 0;
}
