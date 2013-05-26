/* String helper
 */
#ifndef STR_H
#define STR_H

/* Clean the string. */
char *trim(char *str);

/* Split string with given delimiter.
 * Return right part or NULL if delimiter not found.
 */
char *split(char *str, char delim);

#endif
