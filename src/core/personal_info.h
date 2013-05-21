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

struct personal_info *personal_read(char *fname);
void personal_save(char *fname, struct personal_info *info);
void personal_destory(struct personal_info *info);

#endif
