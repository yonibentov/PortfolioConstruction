/* Implementation of reading from and writing to CSV files */

#include "csv.h"
#include "strings.h"
#include "strings_private.h"  // Strings will not be encapsulated for CSV operations

#include <assert.h>
#include <string.h>  // strtok_r()
#include <stdlib.h>  // malloc() and free()


Int _csv_maxNumberOfRows = 1000;
Int _csv_maxNumberOfColumns = 1000;
Int _csv_maxNumberOfFiles = 100;
Int _csv_maxNumberOfCharactersPerElement = 10;


#define ASSERT_DATA(source) \
  assert(source); \
  assert(source->file); \
  assert(source->numberOfRows > 0); \
  assert(source->numberOfRows < _csv_maxNumberOfRows); \
  assert(source->numberOfColumns > 0); \
  assert(source->numberOfColumns < _csv_maxNumberOfColumns);

FILE* fopenFromString(String* filepath, char* mode) {
  assert(filepath);
  assert(mode);
  /* Support read-only, overwrite-only, and append-only modes */
  assert(*mode == 'r' || *mode == 'w' || *mode == 'a');
  assert(*(mode+1) == '\0');
  FILE* result = fopen(filepath->characters, mode);
  assert(result);
  return result;
}


/********************************************/
/* Read a CSV file and turn it into numbers */
/********************************************/

/* Import a line of a file to an array */

// Integer-valued array elements
void csv_arrayIntFromData(ArrayInt* destination, Data* source) {
  assert(destination);
  ASSERT_DATA(source);
  
  assert(arrayInt_getSize(destination) == source->numberOfColumns);
  
  /* Read the next line into a C-string using fgets() */
  Int maxNumberOfCharactersPerLine = _csv_maxNumberOfCharactersPerElement * _csv_maxNumberOfColumns;
  char* line = malloc(maxNumberOfCharactersPerLine + 1);  // sizeof(char) is 1; +1 for '\0'
  fgets(line, maxNumberOfCharactersPerLine + 1, source->file);
  assert(line);
  
  /* Parse the string using strtok_r() */
  char* scratch = NULL;
  for (Int column = 0; column < arrayInt_getSize(destination); column += 1) {
    char* token = strtok_r(column == 0 ? line : NULL,
			   ",", &scratch);
    if (token == NULL) { break; }
    arrayInt_setAt(destination, column, atoi(token));
  }
  
  free(line);
}

// Floating-point-valued array elements
void csv_arrayFloatFromData(ArrayFloat* destination, Data* source) {
  assert(destination);
  ASSERT_DATA(source);

  assert(arrayFloat_getSize(destination) == source->numberOfColumns);

  /* Read the next line into a C-string using fgets() */
  Int maxNumberOfCharactersPerLine = _csv_maxNumberOfCharactersPerElement * _csv_maxNumberOfColumns;
  char* line = malloc(maxNumberOfCharactersPerLine + 1);  // sizeof(char) is 1; +1 for '\0'
  fgets(line, maxNumberOfCharactersPerLine + 1, source->file);
  assert(line);

  /* Parse the string using strtok_r() */
  char* scratch = NULL;
  for (Int column = 0; column < arrayFloat_getSize(destination); column += 1) {
    char* token = strtok_r(column == 0 ? line : NULL,
			   ",", &scratch);
    if (token == NULL) { break; }
    arrayFloat_setAt(destination, column, atof(token));
  }
  
  free(line);
}


/*********************************/
/* Write a CSV file from numbers */
/*********************************/

/* Write a preexisting array to a file for use
   outside the program */

// Integer-valued array elements
void csv_dataFromArrayInt(Data* destination, ArrayInt* source) {
  ASSERT_DATA(destination);
  assert(source);

  /* Write using fprintf() as I would print to the console using printf() */
  Int columns = arrayInt_getSize(source);
  for (Int column = 0; column < columns - 1; column += 1) {
    fprintf(destination->file,
	      #if defined __linux__
	      "%ld,"
	      #elif defined __APPLE__
	      "%lld,"
	      #endif
	      , arrayInt_getAt(source, column));
  }
  fprintf(destination->file,
	    #if defined __linux__
	    "%ld\n"
	    #elif defined __APPLE__
	    "%lld\n"
	    #endif
	    , arrayInt_getAt(source, columns-1));
}

void csv_dataFromMatrixIntColumn(Data* destination, MatrixInt* source, Int sourceColumn) {
  ASSERT_DATA(destination);
  assert(source);

  /* Write using fprintf() as I would print to the console using printf() */
  Int columns = matrixInt_getNumberOfRows(source);  /* Row entries of the source will be
						       column entries of the .csv file */
  for (Int column = 0; column < columns - 1; column += 1) {
    fprintf(destination->file,
	      #if defined __linux__
	      "%ld,"
	      #elif defined __APPLE__
	      "%lld,"
	      #endif
	    , matrixInt_getAt(source, column, sourceColumn));
  }
  fprintf(destination->file,
	    #if defined __linux__
	    "%ld\n"
	    #elif defined __APPLE__
	    "%lld\n"
	    #endif
	  , matrixInt_getAt(source, columns-1, sourceColumn));
}


// Floating-point-valued array elements
void csv_dataFromArrayFloat(Data* destination, ArrayFloat* source) {
  ASSERT_DATA(destination);
  assert(source);

  /* Write using fprintf() as I would print to the console using printf() */
  Int columns = arrayFloat_getSize(source);
  for (Int column = 0; column < columns - 1; column += 1) {
    fprintf(destination->file,
	    "%0.4f, ",
	    arrayFloat_getAt(source, column));
  }
  fprintf(destination->file,
	  "%0.4f\n",
	  arrayFloat_getAt(source, columns-1));
}
