#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "test.h"
#include "../src/utils/csv.h"
#include "../src/utils/debug.h"

int main()
{
    FILE *test_csv;
    struct csv_header *head, *h;
    struct csv_row *row, *r;

    test_csv = fopen(TESTCASES_DIR"/csv.csv", "r");
    if (!test_csv)
        ERROR("open");

    head = csv_read_header(test_csv);
    csv_write_header(stdout, head);

    head->type = CSV_INT;
    sprintf(head->fmt, "%%d");

    h = csv_find_header(head, "balance");
    assert(strcmp(h->name, "balance") == 0);
    h->type = CSV_DOUBLE;
    sprintf(h->fmt, "%%.22lf");

    row = csv_read_row(test_csv, head);
    csv_write_row(stdout, row);

    csv_destory_row(row);
    csv_destory_header(head);

    return 0;
}
