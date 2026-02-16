/*****************************************/
/* Details of string attribute structure */
/*****************************************/

#ifndef STRINGS_PRIVATE_H
#define STRINGS_PRIVATE_H

#include <stddef.h>  // size_t


typedef struct String {
  char* characters;  /* I considered making a new type Char so that characters
			would not be integers, but that seems over-engineered */
  size_t length;  /* The length attribute should behave in the same way as
		     the output from the C-standard function strlen() */
} String;



#endif /* STRINGS_PRIVATE_H */
