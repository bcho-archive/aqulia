#ifndef PERSONAL_INFO_H
#define PERSONAL_INFO_H

#define DATETIME_SIZE 8

/* card state */
typedef enum {
    CARD_USABLE,
    CARD_FREEZED,
    CARD_LOST
} card_state_t;

/* card information */
struct personal_info {
    int cardno;
    char expire[DATETIME_SIZE];
    double balance;
    card_state_t state;
    char faculty[150];
};

#endif
