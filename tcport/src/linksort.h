/* Sort a link list using merge sort.
 */
#ifndef LINK_SORT_H
#define LINK_SORT_H

void link_sort(void **list, void *(*next)(void *), void (*set_next)(void *, void *),
               int (*cmp)(void *, void *), int reverse);

#endif
