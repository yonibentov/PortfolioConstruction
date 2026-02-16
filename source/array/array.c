/*****************************************/
/* Implementation of array class methods */
/*****************************************/

#include "array.h"
#include "array_private.h"
#include "tools.h"

#include <assert.h>
#include <stdlib.h>  // malloc() and free()

Int _array_maxSize = LARGEST_TABLE;
Int _arrayInt_maxValue = LARGEST_INT;
Int _arrayInt_minValue = SMALLEST_INT;
Float _arrayFloat_maxValue = LARGEST_FLOAT;
Float _arrayFloat_minValue = SMALLEST_FLOAT;


/**********************************************************/
/* Arrays with integer-valued elements (prefix: arrayInt) */
/**********************************************************/

/* Constructors */

// Creation
ArrayInt* __arrayInt_allocate(void) {
  return malloc(sizeof(ArrayInt));
}

void __arrayInt_initialize(ArrayInt* array, Int size) {
  assert(array);
  assert(size > 0); assert(size < _array_maxSize);

  // Set size attribute
  array->size = size;

  // Create elements
  array->elements = malloc(sizeof(Int) * size);
  for (Int i = 0; i < size; i += 1) {
    *(array->elements + i) = 0;  // Initialize element values to zero
  }
}

ArrayInt* arrayInt_create(Int size) {
  ArrayInt* array = __arrayInt_allocate();
  __arrayInt_initialize(array, size);
  return array;
}


// Destruction
void __arrayInt_deinitialize(ArrayInt* array) {
  ASSERT_ARRAY(array);
    
  // Destroy elements
  assert(array->elements);
  free(array->elements);
}

void __arrayInt_deallocate(ArrayInt* array) {
  assert(array);  // Only that, since array->elements should be gone
  free(array);
}

void arrayInt_destroy(ArrayInt* array) {
  __arrayInt_deinitialize(array);
  __arrayInt_deallocate(array);
}


// Copy
void arrayInt_copy(ArrayInt* destination, ArrayInt* source) {
  ASSERT_ARRAY(destination);
  ASSERT_ARRAY(source);
  
  Int size = minInt(destination->size, source->size);
  for(Int i = 0; i < size; i += 1) {
    arrayInt_setAt(destination, i,
		   arrayInt_getAt(source, i));
  }
}


/***************/
/* Set and get */
/***************/

#define ASSERT_ARRAY_VALUE_INT(value) \
  assert(value > _arrayInt_minValue); assert(value < _arrayInt_maxValue);

void arrayInt_setAt(ArrayInt* array, Int index, Int value) {
  ASSERT_ARRAY(array);
  ASSERT_ARRAY_INDEX(array, index);
  ASSERT_ARRAY_VALUE_INT(value);
  
  *(array->elements + index) = value;
}

Int arrayInt_getAt(ArrayInt* array, Int index) {
  ASSERT_ARRAY(array);
  ASSERT_ARRAY_INDEX(array, index);

  return *(array->elements + index);
}

void arrayInt_setFromCArray(ArrayInt* array,
			    Int* cArray, Int size) {
  ASSERT_ARRAY(array);

  assert(cArray);
  assert(size > 0); assert(size < _array_maxSize);
  
  /* If the C-array is smaller than array->size, the remaining
     elements of the ArrayInt will be unmodified.
     
     If the C-array is larger than array->size, the ArrayInt
     will be filled to capacity. */

  Int smallerSize = minInt(size, array->size);
  assert(smallerSize > 0); assert(smallerSize < _array_maxSize);
  
  for (Int i = 0; i < smallerSize; i += 1) {
    *(array->elements + i) = *(cArray + i);
  }
}

void arrayInt_setAll(ArrayInt* array, Int value) {
  ASSERT_ARRAY(array);
  ASSERT_ARRAY_VALUE_INT(value);
  
  for (Int i = 0; i < array->size; i += 1) {
    *(array->elements + i) = value;
  }
}

Int arrayInt_getSize(ArrayInt* array) {
  ASSERT_ARRAY(array);

  return array->size;
}


/********************/
/* Print to console */
/********************/

void arrayInt_printFromTo(ArrayInt* array, Int from, Int to) {
  ASSERT_ARRAY(array);
  ASSERT_ARRAY_INDEX(array, from);
  ASSERT_ARRAY_INDEX(array, to);
  assert(from <= to);

  /* Platform-dependent printing */
  #if defined __APPLE__
  printf("(%lld", *(array->elements + from));
  #elif defined __linux__
  printf("(%ld", *(array->elements + from));
  #else
  printf("Error: Failed to detect operating system.\n");
  assert(FALSE);
  #endif  /* End platform check */
  
  for (Int i = from + 1; i <= to; i += 1) {
    #if defined __APPLE__
    printf(", %lld", *(array->elements + i));
    #elif defined __linux__
    printf(", %ld", *(array->elements + i));
    #else
    printf("Error: Failed to detect operating system.\n");
    assert(FALSE);
    #endif  /* End platform check */
  }
  printf(")\n");
}

void arrayInt_print(ArrayInt* array) {
  arrayInt_printFromTo(array, 0, array->size - 1);
}


/***************************/
/* Mathematical operations */
/***************************/

Int arrayInt_innerProduct(ArrayInt* x, ArrayInt* y) {
  ASSERT_ARRAY(x);
  ASSERT_ARRAY(y);

  /* This will work even if the arrays have different sizes,
     under the convention that the missing values are
     treated as zero. */
  
  Int size = minInt(arrayInt_getSize(x), arrayInt_getSize(y));
  Int result = 0;
  
  for (Int i = 0; i < size; i += 1) {
    result += arrayInt_getAt(x, i) * arrayInt_getAt(y, i);
  }

  return result;
}


/*******************************************************************/
/* Arrays with floating-point-valued elements (prefix: arrayFloat) */
/*******************************************************************/

/* Constructors */

// Creation
ArrayFloat* __arrayFloat_allocate(void) {
  return malloc(sizeof(ArrayFloat));
}

void __arrayFloat_initialize(ArrayFloat* array, Int size) {
  assert(array);
  assert(size > 0); assert(size < _array_maxSize);

  // Set size attribute
  array->size = size;

  // Create elements
  array->elements = malloc(sizeof(Float) * size);
  for (Int i = 0; i < size; i += 1) {
    *(array->elements + i) = 0;  // Initialize element values to zero
  }
}

ArrayFloat* arrayFloat_create(Int size) {
  ArrayFloat* array = __arrayFloat_allocate();
  __arrayFloat_initialize(array, size);
  return array;
}


// Destruction
void __arrayFloat_deinitialize(ArrayFloat* array) {
  ASSERT_ARRAY(array);
    
  // Destroy elements
  assert(array->elements);
  free(array->elements);
}

void __arrayFloat_deallocate(ArrayFloat* array) {
  assert(array);  // array->elements should already be gone
  free(array);
}

void arrayFloat_destroy(ArrayFloat* array) {
  __arrayFloat_deinitialize(array);
  __arrayFloat_deallocate(array);
}


// Copy
void arrayFloat_copy(ArrayFloat* destination, ArrayFloat* source) {
  ASSERT_ARRAY(destination);
  ASSERT_ARRAY(source);
  
  Int size = minInt(destination->size, source->size);
  for(Int i = 0; i < size; i += 1) {
    arrayFloat_setAt(destination, i,
		     arrayFloat_getAt(source, i));
  }
}

/***************/
/* Set and get */
/***************/

#define ASSERT_ARRAY_VALUE_FLOAT(value) \
  assert(value > _arrayFloat_minValue); assert(value < _arrayFloat_maxValue);

void arrayFloat_setAt(ArrayFloat* array, Int index, Float value) {
  ASSERT_ARRAY(array);
  ASSERT_ARRAY_INDEX(array, index);
  ASSERT_ARRAY_VALUE_FLOAT(value);

  *(array->elements + index) = value;
}

Float arrayFloat_getAt(ArrayFloat* array, Int index) {
  ASSERT_ARRAY(array);
  ASSERT_ARRAY_INDEX(array, index);

  return *(array->elements + index);
}

void arrayFloat_setFromCArray(ArrayFloat* array,
			    Float* cArray, Int size) {
  ASSERT_ARRAY(array);

  assert(cArray);
  assert(size > 0); assert(size < _array_maxSize);
  
  /* If the C-array is smaller than array->size, the remaining
     elements of the ArrayFloat will be unmodified.
     
     If the C-array is larger than array->size, the ArrayFloat
     will be filled to capacity. */

  Int smallerSize = minInt(size, array->size);
  assert(smallerSize > 0); assert(smallerSize < _array_maxSize);
  
  for (Int i = 0; i < smallerSize; i += 1) {
    *(array->elements + i) = *(cArray + i);
  }
}

void arrayFloat_setAll(ArrayFloat* array, Float value) {
  ASSERT_ARRAY(array);
  ASSERT_ARRAY_VALUE_FLOAT(value);

  for (Int i = 0; i < array->size; i += 1) {
    *(array->elements + i) = value;
  }
}

Int arrayFloat_getSize(ArrayFloat* array) {
  ASSERT_ARRAY(array);

  return array->size;
}


/******************************/
/* Conversion from a function */
/******************************/

Float arrayFloat_intervalFromDomain(ArrayFloat* referenceArray,
				    Float domainMin, Float domainMax) {
  assert(referenceArray);
  assert(domainMin > SMALLEST_FLOAT);
  assert(domainMax < LARGEST_FLOAT);
  assert(domainMin < domainMax);

  Int N = arrayFloat_getSize(referenceArray);
  assert(N > 0); assert(N < _array_maxSize);

  Float dx = (domainMax-domainMin)/((Float)N);
  REGULARIZE_FLOAT(dx);

  return dx;
}

void arrayFloat_arrayFromFunction(ArrayFloat* output,
				  Function_FloatToFloat* input,
				  Float domainMin, Float domainMax) {
  assert(output);
  assert(input);
  assert(domainMin > SMALLEST_FLOAT);
  assert(domainMax < LARGEST_FLOAT);
  assert(domainMin < domainMax);

  Float dx = arrayFloat_intervalFromDomain(output,
					   domainMin, domainMax);
  Float x_i = LARGEST_FLOAT;  // Should give an error below if not reset
  Float f_i = LARGEST_FLOAT;  // That too
  for (Int i = 0; i < arrayFloat_getSize(output); i += 1) {
    x_i = domainMin + dx*(Float)i;
    ASSERT_FLOAT(x_i);
    f_i = input(x_i);
    ASSERT_FLOAT(x_i);
    arrayFloat_setAt(output, i,
		     f_i);
  }
}

Int arrayFloat_indexFromValue(ArrayFloat* samplingGrid, Float value) {
  assert(samplingGrid);
  Int N = arrayFloat_getSize(samplingGrid);
  assert(N > 0); assert(N < _array_maxSize);

  ASSERT_FLOAT(value);

  /* If the value is between x_i and x_{i+1}, I'll return i */
  Int i = 0;
  for (; i < N-1;  // I need to be able to return N-1 at the end
       i += 1) {
    Float x_i = arrayFloat_getAt(samplingGrid, i);
    if (value < x_i) {
      return i;
    }
  }
  return i;
}


/********************/
/* Print to console */
/********************/

void arrayFloat_printFromTo(ArrayFloat* array, Int from, Int to) {
  ASSERT_ARRAY(array);
  ASSERT_ARRAY_INDEX(array, from);
  ASSERT_ARRAY_INDEX(array, to);
  assert(from <= to);

  printf("(%0.4f", *(array->elements + from));
  for (Int i = from + 1; i <= to; i += 1) {
    printf(", %0.4f", *(array->elements + i));
  }
  printf(")\n");
}

void arrayFloat_print(ArrayFloat* array) {
  arrayFloat_printFromTo(array, 0, array->size - 1);
}


/***************************/
/* Mathematical operations */
/***************************/

Float arrayFloat_innerProduct(ArrayFloat* x, ArrayFloat* y) {
  ASSERT_ARRAY(x);
  ASSERT_ARRAY(y);

  /* This will work even if the arrays have different sizes,
     under the convention that the missing values are
     treated as zero. */
  
  Int size = minFloat(arrayFloat_getSize(x), arrayFloat_getSize(y));
  Float result = 0;
  
  for (Int i = 0; i < size; i += 1) {
    result += arrayFloat_getAt(x, i) * arrayFloat_getAt(y, i);
  }

  return result;
}


/************************************************************/
/* Arrays with string-valued elements (prefix: arrayString) */
/************************************************************/

/* Constructors */

ArrayString* __arrayString_allocate(void) {
  return malloc(sizeof(ArrayString));
}

void __arrayString_initialize(ArrayString* array, Int size) {
  assert(array);
  assert(size > 0); assert(size < _array_maxSize);

  /* Set the size attribute */
  array->size = size;

  /* Allocation of the elements requires more care
     than in previous cases */
  
  // Each element is a POINTER TO string:
  array->elements = malloc(size * sizeof(String*));

  for (Int i = 0; i < size; i += 1) {
    // Each element must POINT TO a region of memory that holds a string
    *(array->elements + i) = string_create();  // string_create() returns a POINTER to string
    assert(*(array->elements + i));
  }

}

ArrayString* arrayString_create(Int size) {
  ArrayString* array = __arrayString_allocate();
  assert(array);
  __arrayString_initialize(array, size);
  return array;
}

void __arrayString_deinitialize(ArrayString* array) {
  assert(array);
  assert(array->elements);
  assert(array->size > 0);
  assert(array->size < _array_maxSize);

  // Deallocate the string pointed to by each element
  for (Int i = 0; i < array->size; i += 1) {
    string_destroy(*(array->elements + i));
  }

  // Deallocate the list of pointers
  free(array->elements);
}

void __arrayString_deallocate(ArrayString* array) {
  assert(array);
  free(array);
}

void arrayString_destroy(ArrayString* array) {
  __arrayString_deinitialize(array);
  __arrayString_deallocate(array);
}


#define ASSERT_ELEMENTS_STRING(array) \
  assert(array->elements); \
  for (Int i = 0; i < array->size; i += 1) { \
    assert(*(array->elements + i)); \
    assert(string_getLength(*(array->elements + i)) >= 0); \
  }

/* Print to console */

void arrayString_printFromTo(ArrayString* array, Int from, Int to) {
  ASSERT_ARRAY(array);
  ASSERT_ELEMENTS_STRING(array);

  assert(from >= 0); assert(to < array->size);
  assert(from <= to);
  
  for (Int i = from; i <= to; i += 1) {
    printf("string %lld: ", i+1);
    string_print(*(array->elements + i));
  }
}

void arrayString_print(ArrayString* array) {
  arrayString_printFromTo(array, 0, array->size - 1);
}


/* Set and get */
Int arrayString_getSize(ArrayString* array) {
  ASSERT_ARRAY(array);
  return array->size;
}

void arrayString_setAtFromCString(ArrayString* array, Int index, char* characters) {
  ASSERT_ARRAY(array);
  ASSERT_ELEMENTS_STRING(array);
  assert(index >= 0); assert(index < array->size);
  assert(characters);
  string_overwriteWithCharacters(*(array->elements + index), characters);
}

void arrayString_appendAtFromCString(ArrayString* array, Int index, char* characters) {
  ASSERT_ARRAY(array);
  ASSERT_ELEMENTS_STRING(array);
  assert(index >= 0); assert(index < array->size);
  assert(characters);
  string_appendCharacters(*(array->elements + index), characters);
}

String* arrayString_getAt(ArrayString* array, Int index) {
  ASSERT_ARRAY(array);
  ASSERT_ARRAY_INDEX(array, index);

  return *(array->elements + index);
}
