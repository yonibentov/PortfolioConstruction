/*****************************************************/
/* Implementation of array class attribute structure */
/*****************************************************/

#ifndef ARRAY_PRIVATE_H
#define ARRAY_PRIVATE_H

#include "common.h"

#define ASSERT_ARRAY(array) \
  assert(array); assert(array->elements); \
  assert(array->size > 0); assert(array->size < _array_maxSize);

#define ASSERT_ARRAY_INDEX(array, index) \
  assert(index >= 0); assert(index < array->size);

/* Array with integer-valued elements */
typedef struct ArrayInt {
  Int* elements;
  Int size;
} ArrayInt;

/* Array with floating-point-valued elements */
typedef struct ArrayFloat {
  Float* elements;
  Int size;
} ArrayFloat;

/* Array with string-valued elements */
typedef struct ArrayString {
  String** elements;  // Each element is a POINTER TO string
  Int size;
} ArrayString;


#endif /* ARRAY_PRIVATE_H */

