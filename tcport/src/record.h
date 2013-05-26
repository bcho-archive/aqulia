/* Account consume record
 */
#ifndef CONSUME_RECORD_H
#define CONSUME_RECORD_H

typedef enum {
    CONSUME_POS,
    CONSUME_RECHARGE
} CONSUME_TYPE_T;

struct POS {
    char *name;
    int transcation;

    struct POS *next;
};

struct consume_record {
    char *consumed, *received;
    double sum; double balance;
    CONSUME_TYPE_T type;
    struct POS *pos;

    struct consume_record *next;
};


struct consume_record *consume_record_read(char *fname);
void consume_record_save(char *fname, struct consume_record **rec);     /* record will be sorted after saving */
void consume_record_destory(struct consume_record *record);
struct consume_record *
consume_record_create(const char *consumed, const char *received,
                      double sum, double balance,
                      CONSUME_TYPE_T type, const char *pos_name,
                      struct consume_record *prev_record);


/* POS machine */
struct POS *POS_get(struct consume_record *record);                     /* copy all POS machines from consume record */
struct POS *POS_query_by_name(const char *name, struct POS *pos);
struct POS *POS_create(const char *name, int transcation);
struct POS *POS_dup(struct POS *pos);
void POS_destory(struct POS *pos);

#endif
