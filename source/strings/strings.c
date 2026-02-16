/******************************************/
/* Implementation of string class methods */
/******************************************/

#include "strings.h"
#include "strings_private.h"

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>


#define MAX_STRING_LENGTH 1000

/* Constructors */

// Creation
String* __string_allocate(void) {
  return malloc(sizeof(String));
}

/* "Initialization" will be interpreted as,
   "create an empty string" */
void __string_initialize(String* string) {
  assert(string);

  string->length = 0;
  
  // Create and initialize the characters attribute
  string->characters = malloc(1);  // sizeof(char) is 1
  *(string->characters) = '\0';

  /* An "empty" string will be considered to be a string
     with only one character, whose value is '\0'.

     The minimal "nonempty" string will be a string with
     two characters, the second of which is always '\0'.
     For C-standard strings, the first character could
     also be '\0'--in my string class, I'll disallow
     that. */
}

String* string_create(void) {
  String* result = __string_allocate();
  __string_initialize(result);
  return result;
}


// Destruction
void __string_deinitialize(String* string) {
  assert(string);
  assert(string->characters);
  free(string->characters);
}

void __string_deallocate(String* string) {
  assert(string);  // string->characters should be gone by now
  free(string);
}

void string_destroy(String* string) {
  __string_deinitialize(string);
  __string_deallocate(string);
}


#define ASSERT_STRING(string) \
  assert(string); \
  assert(string->characters); \
  assert(string->length >= 0); \
  assert(string->length < MAX_STRING_LENGTH);


/* Get properties */

size_t string_getLength(String* string) {
  ASSERT_STRING(string);
  return string->length;
}

char string_getAt(String* string, int c) {
  ASSERT_STRING(string);
  assert(c >= 0); assert(c < string->length);
  return *(string->characters + c);
}

void string_getAll(String* string,
		   char* destination,
		   size_t numberOfCharacters) {
  ASSERT_STRING(string);
  assert(destination);
  assert(numberOfCharacters > 0);
  assert(numberOfCharacters <= string_getLength(string));

  /* This assumes that the caller had allocated
     a character array with at least
     "numberOfCharacters+1" elements, the +1
     being for the terminating '\0' */

  for (int c = 0; c < string_getLength(string); c += 1) {
    *(destination + c) = string_getAt(string, c);
  }
  *(destination + string_getLength(string)) = '\0';
}

bool __string_isEmpty(String* string) {
  ASSERT_STRING(string);

  /* A string will be considered empty if and only if
     both of the following conditions hold:

     (1) The string has only one character;
     (2) That character is null.

     I should also make sure that an empty string has
     its length attribute set to zero, and a nonempty
     string has its length attribute set to something
     greater than zero. */

  int numberOfNonNullCharacters = string_getLength(string);
  if (numberOfNonNullCharacters == 0) {
    assert(*(string->characters) == '\0');
    return true;
  }
  assert(numberOfNonNullCharacters > 0);

  /* Let me work through the counting.

     Say the number of non-null characters is 1.
     For example, the string is "a". In that case,
     *string would be 'a', and *(string + 1) would be '\0'.
     
     So *(string + numberOfNonNullCharacters) should be '\0',
     and *(string + c) for all c < numberOfNonNullCharacters
     should be anything but '\0'. */

  for (int c = 0; c < numberOfNonNullCharacters; c += 1) {
    assert(*(string->characters + c) != '\0');
  }
  assert(*(string->characters + numberOfNonNullCharacters) == '\0');
  return false;
}


/* Modify strings */

void __string_grow(String* string, size_t numberOfCharactersToAdd) {
  ASSERT_STRING(string);
  assert(numberOfCharactersToAdd > 0);

  size_t newLength = string->length + numberOfCharactersToAdd;
  assert(newLength < MAX_STRING_LENGTH);
  string->length = newLength;

  // https://stackoverflow.com/questions/9142805/do-we-lose-data-in-a-buffer-after-reallocing
  char* newCharacters = realloc(string->characters, newLength + 1);  // +1 for '\0'
  assert(newCharacters);
  string->characters = newCharacters;

  /* At this stage, I have not ASSIGNED any new characters.
     So it would not make sense to assert that the string is
     not empty. */
  
  /* All of that includes space for the terminating null character. */
}

void string_overwriteWithCharacter(String* string, char character) {
  ASSERT_STRING(string);

  /* I will disallow overwriting with a null character:
     If I want to destroy the string, then I should
     destroy the string. */
  assert(character != '\0');
  
  char* newCharacters = realloc(string->characters, 2);  // Including '\0'
  assert(newCharacters);
  string->characters = newCharacters;
  string->length = 1;
  
  *(string->characters) = character;
  *(string->characters + 1) = '\0';
  assert(!__string_isEmpty(string));
}

void string_appendCharacter(String* string, char character) {
  ASSERT_STRING(string);
  assert(character != '\0');
  
  if (__string_isEmpty(string)) {
    string_overwriteWithCharacter(string, character);
  } else {
    __string_grow(string, 1);
    *(string->characters + string->length - 1) = character;
    *(string->characters + string->length) = '\0';
  }
}

void string_overwriteWithCharacters(String* string,
				    char* characters) {
  ASSERT_STRING(string);
  assert(characters);

  string_overwriteWithCharacter(string, *characters);
  assert(!__string_isEmpty(string));
  if (*(characters+1) != '\0') {
    for (int c = 1; *(characters+c) != '\0'; c += 1) {
      string_appendCharacter(string, *(characters + c));
    }
  }
  *(string->characters + string->length) = '\0';
}

void string_appendCharacters(String* string,
			     char* characters) {
  ASSERT_STRING(string);
  assert(characters);

  if (__string_isEmpty(string)) {
    string_overwriteWithCharacters(string, characters);
  } else {
    size_t oldLength = string_getLength(string);
    for (int c = 0; c < *(characters + c) != '\0'; c += 1) {
      __string_grow(string, 1);
      *(string->characters + oldLength + c) = *(characters + c);
    }
  }
  *(string->characters + string->length) = '\0';
}

void string_overwriteWithString(String* destination, String* source) {
  ASSERT_STRING(destination);
  ASSERT_STRING(source);

  string_overwriteWithCharacters(destination, source->characters);
}

void string_appendString(String* destination, String* source) {
  ASSERT_STRING(destination);
  ASSERT_STRING(source);

  string_appendCharacters(destination, source->characters);
}


/* Print to console */

void string_printFromTo(String* string,
			int from, int to) {
  ASSERT_STRING(string);
  
  if (__string_isEmpty(string)) {
    printf("()\n");
    return;
  }
  
  assert(from >= 0); assert(to < string->length);
  assert(from <= to);


  for (int c = from; c <= to; c += 1) {
    printf("%c", *(string->characters + c));
  }
  printf("\n");
}

void string_print(String* string) {
  string_printFromTo(string, 0, string->length-1);
}
