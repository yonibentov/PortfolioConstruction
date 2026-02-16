/*****************************/
/* Interface of matrix class */
/*****************************/

#ifndef MATRIX_H
#define MATRIX_H

#include "common.h"


/*************************************************************/
/* Matrices with integer-valued elements (prefix: matrixInt) */
/*************************************************************/

/* Attribute structure */
typedef struct MatrixInt MatrixInt;

/* Class methods */

// Constructors
MatrixInt* matrixInt_create(Int rows, Int columns);
void matrixInt_destroy(MatrixInt* matrix);

void matrixInt_copy(MatrixInt* destination,
		    MatrixInt* source);

// Set and get
void matrixInt_setAt(MatrixInt* matrix, Int row, Int column, Int value);
Int matrixInt_getAt(MatrixInt* matrix, Int row, Int column);

Int matrixInt_getNumberOfRows(MatrixInt* matrix);
Int matrixInt_getNumberOfColumns(MatrixInt* matrix);

void matrixInt_setRow(MatrixInt* matrix, Int row, Int value);
void matrixInt_setColumn(MatrixInt* matrix, Int column, Int value);
void matrixInt_setAll(MatrixInt* matrix, Int value);

// Print to console
void matrixInt_printRowFromTo(MatrixInt* matrix, Int row,
			      Int from, Int to);  // Both inclusive
void matrixInt_printRow(MatrixInt* matrix, Int row);

void matrixInt_printColumnFromTo(MatrixInt* matrix, Int column,
				 Int from, Int to);  // Both inclusive
void matrixInt_printColumn(MatrixInt* matrix, Int column);

void matrixInt_print(MatrixInt* matrix);


/**********************************************************************/
/* Matrices with floating-point-valued elements (prefix: matrixFloat) */
/**********************************************************************/

/* Attribute structure */
typedef struct MatrixFloat MatrixFloat;

/* Class methods */

// Constructors
MatrixFloat* matrixFloat_create(Int rows, Int columns);
void matrixFloat_destroy(MatrixFloat* matrix);

void matrixFloat_copy(MatrixFloat* destination,
		      MatrixFloat* source);

// Set and get
void matrixFloat_setAt(MatrixFloat* matrix, Int row, Int column, Float value);
Float matrixFloat_getAt(MatrixFloat* matrix, Int row, Int column);

Int matrixFloat_getNumberOfRows(MatrixFloat* matrix);
Int matrixFloat_getNumberOfColumns(MatrixFloat* matrix);

void matrixFloat_setRow(MatrixFloat* matrix, Int row, Float value);
void matrixFloat_setColumn(MatrixFloat* matrix, Int column, Float value);
void matrixFloat_setAll(MatrixFloat* matrix, Float value);

// Print to console
void matrixFloat_printRowFromTo(MatrixFloat* matrix, Int row,
				Int from, Int to);  // Both inclusive
void matrixFloat_printRow(MatrixFloat* matrix, Int row);

void matrixFloat_printColumnFromTo(MatrixFloat* matrix, Int column,
				   Int from, Int to);  // Both inclusive
void matrixFloat_printColumn(MatrixFloat* matrix, Int column);

void matrixFloat_print(MatrixFloat* matrix);


#endif /* MATRIX_H */
