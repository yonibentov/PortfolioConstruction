/****************************/
/* Interface of array class */
/****************************/

#ifndef ARRAY_H
#define ARRAY_H

#include "common.h"
#include "tools.h"
#include "strings.h"


/**********************************************************/
/* Arrays with integer-valued elements (prefix: arrayInt) */
/**********************************************************/

/* Attribute structure */
typedef struct ArrayInt ArrayInt;

/* Class methods */

// Constructors
ArrayInt* arrayInt_create(Int size);
void arrayInt_destroy(ArrayInt* array);

void arrayInt_copy(ArrayInt* destination,
		   ArrayInt* source);

// Set and get
void arrayInt_setAt(ArrayInt* array, Int index, Int value);
Int arrayInt_getAt(ArrayInt* array, Int index);

void arrayInt_setFromCArray(ArrayInt* array, Int* cArray, Int size);
void arrayInt_setAll(ArrayInt* array, Int value);

Int arrayInt_getSize(ArrayInt* array);

// Print to console
void arrayInt_printFromTo(ArrayInt* array, Int from, Int to);  // Both inclusive
void arrayInt_print(ArrayInt* array);


// Mathematical operations
Int arrayInt_innerProduct(ArrayInt* x, ArrayInt* y);


/*******************************************************************/
/* Arrays with floating-point-valued elements (prefix: arrayFloat) */
/*******************************************************************/

/* Attribute structure */
typedef struct ArrayFloat ArrayFloat;

/* Class methods */

// Constructors
ArrayFloat* arrayFloat_create(Int size);
void arrayFloat_destroy(ArrayFloat* array);

void arrayFloat_copy(ArrayFloat* destination,
		   ArrayFloat* source);

// Set and get
void arrayFloat_setAt(ArrayFloat* array, Int index, Float value);
Float arrayFloat_getAt(ArrayFloat* array, Int index);

void arrayFloat_setFromCArray(ArrayFloat* array, Float* cArray, Int size);
void arrayFloat_setAll(ArrayFloat* array, Float value);

Int arrayFloat_getSize(ArrayFloat* array);

// Print to console
void arrayFloat_printFromTo(ArrayFloat* array, Int from, Int to);  // Both inclusive
void arrayFloat_print(ArrayFloat* array);

// Mathematical operations
Float arrayFloat_innerProduct(ArrayFloat* x, ArrayFloat* y);


/* Conversion from a function */
Float arrayFloat_intervalFromDomain(ArrayFloat* referenceArray,
				    Float domainMin, Float domainMax);
void arrayFloat_arrayFromFunction(ArrayFloat* output,
				  Function_FloatToFloat* input,
				  Float domainMin, Float domainMax);
Int arrayFloat_indexFromValue(ArrayFloat* samplingGrid, Float value);


/************************************************************/
/* Arrays with string-valued elements (prefix: arrayString) */
/************************************************************/

/* Attribute structure */
typedef struct ArrayString ArrayString;

/* Class methods */

// Constructors
ArrayString* arrayString_create(Int size);
void arrayString_destroy(ArrayString* array);

// Set and get
void arrayString_setAtFromCString(ArrayString* array, Int index, char* characters);
void arrayString_appendAtFromCString(ArrayString* array, Int index, char* characters);

Int arrayString_getSize(ArrayString* array);
String* arrayString_getAt(ArrayString* array, Int index);


// Print to console
void arrayString_printFromTo(ArrayString* array, Int from, Int to);  // Both inclusive
void arrayString_print(ArrayString* array);




#endif /* ARRAY_H */


