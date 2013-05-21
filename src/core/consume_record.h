#ifndef CONSUME_RECORD_H
#define CONSUME_RECORD_H

#define DATETIME_SIZE 9

/* consume type */
typedef enum {
    CONSUME_POS
} consume_type_t;

/* pos machine information */
struct pos_info {
    char name[150];
    int transcation;
};

/* consume record */
struct consume_record {
    char consumed[DATETIME_SIZE], received[DATETIME_SIZE];
    double sum;
    double balance;
    consume_type_t consume_type;
    struct pos_info *pos;

    struct consume_record *next;
};

struct consume_record *consume_record_read(char *fname);
void consume_record_save(char *fname, struct consume_record *record);
void consume_record_destory(struct consume_record *record);
#endif
