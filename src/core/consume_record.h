#ifndef CONSUME_RECORD_H
#define CONSUME_RECORD_H

#define DATETIME_SIZE 8

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

#endif
