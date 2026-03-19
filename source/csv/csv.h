/* Interface for reading from and writing to CSV files */

#ifndef READ_WRITE_CSV_H
#define READ_WRITE_CSV_H

#include "common.h"
#include <stdlib.h>
#include "array.h"
#include "strings.h"
#include "matrix.h"


typedef struct Data {
  FILE* file;
  Int numberOfRows;
  Int numberOfColumns;
} Data;

FILE* fopenFromString(String* filepath, char* mode);


/********************************************/
/* Read a CSV file and turn it into numbers */
/********************************************/

/* Import a line of a file to an array */

// Integer-valued array elements
void csv_arrayIntFromData(ArrayInt* destination,
			  Data* source);
// Floating-point-valued array elements
void csv_arrayFloatFromData(ArrayFloat* destination,
			    Data* source);


/*********************************/
/* Write a CSV file from numbers */
/*********************************/

/* Write a preexisting array to a file for use
   outside the program */

// Integer-valued array elements
void csv_dataFromArrayInt(Data* destination,
			  ArrayInt* source);
void csv_dataFromMatrixIntColumn(Data* destination,
				 MatrixInt* source,
				 Int column);

// Floating-point-valued array elements
void csv_dataFromArrayFloat(Data* destination,
			    ArrayFloat* source);


#endif /* READ_WRITE_CSV_H */
