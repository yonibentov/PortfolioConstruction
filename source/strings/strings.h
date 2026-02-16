/********************************/
/* Interface for string library */
/********************************/

#ifndef STRINGS_H
#define STRINGS_H

#include <stddef.h>  // size_t


/* Attribute structure */
typedef struct String String;


/* Class methods (prefix: string) */

// Constructors
String* string_create(void);
void string_destroy(String* string);

// Get properties
size_t string_getLength(String* string);
char string_getAt(String* string, int c);
void string_getAll(String* string,
		   char* destination,
		   size_t numberOfCharacters);

// Modify strings

void string_overwriteWithCharacter(String* string,
				   char character);
void string_appendCharacter(String* string,
			    char character);

void string_overwriteWithCharacters(String* string,
				    char* CStringLiteral);
void string_appendCharacters(String* string,
			     char* CStringLiteral);

void string_overwriteWithString(String* destination,
				String* source);
void string_appendString(String* destination,
			 String* source);

// Print to console
void string_printFromTo(String* string,
			int from, int to);  // Both inclusive
void string_print(String* string);


#endif /* STRINGS_H */
