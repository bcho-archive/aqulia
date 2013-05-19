#ifndef ACCOUNT_H
#define ACCOUNT_H

#define DATETIME_SIZE 8

/* card state */
typedef enum {
    CARD_USABLE,
    CARD_FREEZED,
    CARD_LOST
} card_state_t;

/* consume type */
typedef enum {
    CONSUME_POS;
} consume_type_t;

/* card information */
struct personal_info {
    char expr[DATETIME_SIZE];
    double balance;
    card_state_t state;
    char faculty[150];
};

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
};

#endif
