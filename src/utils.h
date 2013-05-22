#ifndef UTILS_H
#define UTILS_H

/* trim the space */
char *trim(char *str);

/* 
 * split string into two part with given delimiter
 * 
 * return value: the right part or NULL if delimiter not found
 */
char *split(char *str, char delim);

/* touch file */
void touch_file(char *fname);

#endif
