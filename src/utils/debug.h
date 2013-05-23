/* Debug utilities.
 */
#ifndef DEBUG_H
#define DEBUG_H

#include <stdio.h>


/* DEBUG suits */
#define LOC fprintf(stderr, "in %s:%d ", __FILE__, __LINE__)
void DEBUG(char *fmt, ...);
void ERROR(char *fmt, ...);

#endif
