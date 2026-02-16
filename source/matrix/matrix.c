/******************************************/
/* Implementation of matrix class methods */
/******************************************/

#include "matrix.h"
#include "matrix_private.h"
#include "tools.h"

#include <stdlib.h>  // malloc() and free()
#include <assert.h>
Int _matrix_maxRows = LARGEST_TABLE;
Int _matrix_maxColumns = LARGEST_TABLE;

#define ASSERT_MATRIX(matrix) \
  assert(matrix); assert(matrix->elements); \
  assert(matrix->rows > 0); assert(matrix->rows < _matrix_maxRows); \
  assert(matrix->columns > 0); assert(matrix->columns < _matrix_maxColumns);

#define ASSERT_MATRIX_ROW_INDEX(matrix, row) \
  assert(row >= 0); assert(row < matrix->rows);

#define ASSERT_MATRIX_COLUMN_INDEX(matrix, column) \
  assert(column >= 0); assert(column < matrix->columns);


/*************************************************************/
/* Matrices with integer-valued elements (prefix: matrixInt) */
/*************************************************************/
Int _matrixInt_maxValue = LARGEST_INT;
Int _matrixInt_minValue = SMALLEST_INT;

/* Constructors */

// Creation
MatrixInt* __matrixInt_allocate(void) {
  return malloc(sizeof(MatrixInt));
}

void __matrixInt_initialize(MatrixInt* matrix, Int rows, Int columns) {
  assert(matrix);
  assert(rows > 0); assert(rows < _matrix_maxRows);
  assert(columns > 0); assert(columns < _matrix_maxColumns);

  // Set size attributes
  matrix->rows = rows;
  matrix->columns = columns;
  
  // Create the elements and initialize their values to zero
  matrix->elements = malloc(sizeof(Int) * rows * columns);
  for (Int i = 0; i < rows * columns; i += 1) {
    *(matrix->elements + i) = 0;
  }
}

MatrixInt* matrixInt_create(Int rows, Int columns) {
  MatrixInt* matrix = __matrixInt_allocate();
  __matrixInt_initialize(matrix, rows, columns);  // Assertions on rows & columns are in there
  return matrix;
}


// Destruction
void __matrixInt_deinitialize(MatrixInt* matrix) {
  ASSERT_MATRIX(matrix);

  // Destroy elements
  assert(matrix->elements);
  free(matrix->elements);
}

void __matrixInt_deallocate(MatrixInt* matrix) {
  assert(matrix);  // matrix->elements should not exist
  free(matrix);
}

void matrixInt_destroy(MatrixInt* matrix) {
  __matrixInt_deinitialize(matrix);
  __matrixInt_deallocate(matrix);
}


// Copy
void matrixInt_copy(MatrixInt* destination,
		      MatrixInt* source) {
  ASSERT_MATRIX(destination);
  ASSERT_MATRIX(source);
  
  Int numberOfRows = minInt(destination->rows, source->rows);
  assert(numberOfRows > 0); assert(numberOfRows < _matrix_maxRows);
  
  Int numberOfColumns = minInt(destination->columns, source->columns);
  assert(numberOfColumns > 0); assert(numberOfColumns < _matrix_maxColumns);

  for (Int row = 0; row < numberOfRows; row += 1) {
    for (Int column = 0; column < numberOfColumns; column += 1) {
      matrixInt_setAt(destination, row, column,
		      matrixInt_getAt(source, row, column));
    }
  }
}


/* Set and get */

#define ASSERT_MATRIX_VALUE_INT(value) \
  assert(value > _matrixInt_minValue); \
  assert(value < _matrixInt_maxValue);

void matrixInt_setAt(MatrixInt* matrix, Int row, Int column, Int value) {
  ASSERT_MATRIX(matrix);

  ASSERT_MATRIX_ROW_INDEX(matrix, row);
  ASSERT_MATRIX_COLUMN_INDEX(matrix, column);

  Int stride = matrix->columns;
  Int offset = row * stride + column;

  ASSERT_MATRIX_VALUE_INT(value);
  
  *(matrix->elements + offset) = value;
}

Int matrixInt_getAt(MatrixInt* matrix, Int row, Int column) {
  ASSERT_MATRIX(matrix);

  ASSERT_MATRIX_ROW_INDEX(matrix, row);
  ASSERT_MATRIX_COLUMN_INDEX(matrix, column);

  Int stride = matrix->columns;
  Int offset = row * stride + column;

  return *(matrix->elements + offset);
}

Int matrixInt_getNumberOfRows(MatrixInt* matrix) {
  ASSERT_MATRIX(matrix);
  
  return matrix->rows;
}

Int matrixInt_getNumberOfColumns(MatrixInt* matrix) {
  ASSERT_MATRIX(matrix);

  return matrix->columns;
}

void matrixInt_setRow(MatrixInt* matrix, Int row, Int value) {
  ASSERT_MATRIX(matrix);
  ASSERT_MATRIX_ROW_INDEX(matrix, row);
  ASSERT_MATRIX_VALUE_INT(value);

  for (Int column = 0; column < matrix->columns; column += 1) {
    matrixInt_setAt(matrix, row, column, value);
  }
}

void matrixInt_setColumn(MatrixInt* matrix, Int column, Int value) {
  ASSERT_MATRIX(matrix);
  ASSERT_MATRIX_COLUMN_INDEX(matrix, column);
  ASSERT_MATRIX_VALUE_INT(value);
  
  for (Int row = 0; row < matrix->rows; row += 1) {
    matrixInt_setAt(matrix, row, column, value);
  }
}

void matrixInt_setAll(MatrixInt* matrix, Int value) {
  ASSERT_MATRIX(matrix);
  ASSERT_MATRIX_VALUE_INT(value);

  for (Int row = 0; row < matrix->rows; row += 1) {
    matrixInt_setRow(matrix, row, value);
  }
}


/* Print to console */

// Print a row
void matrixInt_printRowFromTo(MatrixInt* matrix, Int row,
			      Int from, Int to) {
  ASSERT_MATRIX(matrix);
  ASSERT_MATRIX_ROW_INDEX(matrix, row);
  ASSERT_MATRIX_COLUMN_INDEX(matrix, from);
  ASSERT_MATRIX_COLUMN_INDEX(matrix, to);
  assert(from <= to);
  
  #if defined __APPLE__
  printf("row %lld: ", row);
  #elif defined __linux__
  printf("row %ld: ", row);
  #else
  printf("Error: Failed to detect operating system.\n");
  assert(FALSE);
  #endif  /* Platform check */
  
  if (from > 0) { printf("..."); }
  #if defined __APPLE__
  printf("(%lld", matrixInt_getAt(matrix, row, from));
  #elif defined __linux__
  printf("(%ld", matrixInt_getAt(matrix, row, from));
  #else
  printf("Error: Failed to detect operating system.\n");
  assert(FALSE);
  #endif  /* Platform check */
  
  for (Int i = from + 1; i <= to; i += 1) {
    #if defined __APPLE__
    printf(", %lld", matrixInt_getAt(matrix, row, i));
    #elif defined __linux__
    printf(", %ld", matrixInt_getAt(matrix, row, i));
    #else
    printf("Error: Failed to detect operating system.\n");
    assert(FALSE);
    #endif  /* Platform check */
    
  }
  printf(")");
  if (to < matrix->columns - 1) { printf("..."); }
  printf("\n");
}

void matrixInt_printRow(MatrixInt* matrix, Int row) {
  matrixInt_printRowFromTo(matrix, row, 0, matrix->columns - 1);
}


// Print a column
void matrixInt_printColumnFromTo(MatrixInt* matrix, Int column,
				 Int from, Int to) {
  ASSERT_MATRIX(matrix);
  ASSERT_MATRIX_COLUMN_INDEX(matrix, column);
  ASSERT_MATRIX_ROW_INDEX(matrix, from);
  ASSERT_MATRIX_ROW_INDEX(matrix, to);
  assert(from <= to);

  #if defined __APPLE__
  printf("column %lld: ", column);
  #elif defined __linux__
  printf("column %ld: ", column);
  #else
  printf("Error: Failed to detect operating system.\n");
  assert(FALSE);
  #endif  /* Platform check */
  
  if (from > 0) { printf("..."); }
  #if defined __APPLE__
  printf("(%lld", matrixInt_getAt(matrix, from, column));
  #elif defined __linux__
  printf("(%ld", matrixInt_getAt(matrix, from, column));
  #else
  printf("Error: Failed to detect operating system.\n");
  assert(FALSE);
  #endif  /* Platform check */

  for (Int i = from + 1; i <= to; i += 1) {
    #if defined __APPLE__
    printf(", %lld", matrixInt_getAt(matrix, i, column));
    #elif defined __linux__
    printf(", %ld", matrixInt_getAt(matrix, i, column));
    #else
    printf("Error: Failed to detect operating system.\n");
    assert(FALSE);
    #endif  /* Platform check */
  }
  printf(")");
  if (to < matrix->rows - 1) { printf("..."); }
  printf("^T\n");
}

void matrixInt_printColumn(MatrixInt* matrix, Int column) {
  matrixInt_printColumnFromTo(matrix, column, 0, matrix->rows - 1);
}


// Print the whole matrix
void matrixInt_print(MatrixInt* matrix) {
  for (Int row = 0; row < matrix->rows; row += 1) {
    matrixInt_printRow(matrix, row);
  }
}


/**********************************************************************/
/* Matrices with floating-point-valued elements (prefix: matrixFloat) */
/**********************************************************************/
Int _matrixFloat_maxValue = LARGEST_FLOAT;
Int _matrixFloat_minValue = SMALLEST_FLOAT;

/* Constructors */

// Creation
MatrixFloat* __matrixFloat_allocate(void) {
  return malloc(sizeof(MatrixFloat));
}

void __matrixFloat_initialize(MatrixFloat* matrix, Int rows, Int columns) {
  assert(matrix);
  assert(rows > 0); assert(rows < _matrix_maxRows);
  assert(columns > 0); assert(columns < _matrix_maxColumns);

  // Set size attributes
  matrix->rows = rows;
  matrix->columns = columns;
  
  // Create the elements and initialize their values to zero
  matrix->elements = malloc(sizeof(Float) * rows * columns);
  for (Int i = 0; i < rows * columns; i += 1) {
    *(matrix->elements + i) = 0;
  }
}

MatrixFloat* matrixFloat_create(Int rows, Int columns) {
  MatrixFloat* matrix = __matrixFloat_allocate();
  __matrixFloat_initialize(matrix, rows, columns);  // Assertions on rows & columns are in there
  return matrix;
}


// Destruction
void __matrixFloat_deinitialize(MatrixFloat* matrix) {
  assert(matrix);

  // Destroy elements
  assert(matrix->elements);
  free(matrix->elements);
}

void __matrixFloat_deallocate(MatrixFloat* matrix) {
  assert(matrix);
  free(matrix);
}

void matrixFloat_destroy(MatrixFloat* matrix) {
  __matrixFloat_deinitialize(matrix);
  __matrixFloat_deallocate(matrix);
}

// Copy
void matrixFloat_copy(MatrixFloat* destination,
		      MatrixFloat* source) {
  ASSERT_MATRIX(destination);
  ASSERT_MATRIX(source);
  
  Int numberOfRows = minInt(destination->rows, source->rows);
  assert(numberOfRows > 0); assert(numberOfRows < _matrix_maxRows);
  
  Int numberOfColumns = minInt(destination->columns, source->columns);
  assert(numberOfColumns > 0); assert(numberOfColumns < _matrix_maxColumns);

  for (Int row = 0; row < numberOfRows; row += 1) {
    for (Int column = 0; column < numberOfColumns; column += 1) {
      matrixFloat_setAt(destination, row, column,
			matrixFloat_getAt(source, row, column));
    }
  }
}

  
/* Set and get */

#define ASSERT_MATRIX_VALUE_FLOAT(value) \
  assert(value > _matrixFloat_minValue); \
  assert(value < _matrixFloat_maxValue);

void matrixFloat_setAt(MatrixFloat* matrix, Int row, Int column, Float value) {
  ASSERT_MATRIX(matrix);

  ASSERT_MATRIX_ROW_INDEX(matrix, row);
  ASSERT_MATRIX_COLUMN_INDEX(matrix, column);

  Int stride = matrix->columns;
  Int offset = row * stride + column;

  ASSERT_MATRIX_VALUE_FLOAT(value);
  
  *(matrix->elements + offset) = value;
}

Float matrixFloat_getAt(MatrixFloat* matrix, Int row, Int column) {
  ASSERT_MATRIX(matrix);

  ASSERT_MATRIX_ROW_INDEX(matrix, row);
  ASSERT_MATRIX_COLUMN_INDEX(matrix, column);
  
  Int stride = matrix->columns;
  Int offset = row * stride + column;

  return *(matrix->elements + offset);
}

Int matrixFloat_getNumberOfRows(MatrixFloat* matrix) {
  ASSERT_MATRIX(matrix);
  
  return matrix->rows;
}

Int matrixFloat_getNumberOfColumns(MatrixFloat* matrix) {
  ASSERT_MATRIX(matrix);

  return matrix->columns;
}

void matrixFloat_setRow(MatrixFloat* matrix, Int row, Float value) {
  ASSERT_MATRIX(matrix);
  ASSERT_MATRIX_ROW_INDEX(matrix, row);
  ASSERT_MATRIX_VALUE_FLOAT(value);

  for (Int column = 0; column < matrix->columns; column += 1) {
    matrixFloat_setAt(matrix, row, column, value);
  }
}

void matrixFloat_setColumn(MatrixFloat* matrix, Int column, Float value) {
  ASSERT_MATRIX(matrix);
  ASSERT_MATRIX_COLUMN_INDEX(matrix, column);
  ASSERT_MATRIX_VALUE_FLOAT(value);
  
  for (Int row = 0; row < matrix->rows; row += 1) {
    matrixFloat_setAt(matrix, row, column, value);
  }
}

void matrixFloat_setAll(MatrixFloat* matrix, Float value) {
  ASSERT_MATRIX(matrix);
  ASSERT_MATRIX_VALUE_FLOAT(value);

  for (Int row = 0; row < matrix->rows; row += 1) {
    matrixFloat_setRow(matrix, row, value);
  }
}


/* Print to console */

// Print a row
void matrixFloat_printRowFromTo(MatrixFloat* matrix, Int row,
				Int from, Int to) {
  ASSERT_MATRIX(matrix);

  ASSERT_MATRIX_ROW_INDEX(matrix, row);
  ASSERT_MATRIX_COLUMN_INDEX(matrix, from);
  ASSERT_MATRIX_COLUMN_INDEX(matrix, to);
  assert(from <= to);

  #if defined __APPLE__
  printf("row %lld: ", row);
  #elif defined __linux__
  printf("row %ld: ", row);
  #else
  printf("Error: Failed to detect operating system.\n");
  assert(FALSE);
  #endif  /* Platform check */
  
  if (from > 0) { printf("..."); }
  printf("(%0.4f", matrixFloat_getAt(matrix, row, from));
  
  for (Int i = from + 1; i <= to; i += 1) {
    printf(", %0.4f", matrixFloat_getAt(matrix, row, i));
  }
  printf(")");
  if (to < matrix->columns - 1) { printf("..."); }
  printf("\n");
}

void matrixFloat_printRow(MatrixFloat* matrix, Int row) {
  matrixFloat_printRowFromTo(matrix, row, 0, matrix->columns - 1);
}


// Print a column
void matrixFloat_printColumnFromTo(MatrixFloat* matrix, Int column,
				   Int from, Int to) {
  ASSERT_MATRIX(matrix);

  ASSERT_MATRIX_COLUMN_INDEX(matrix, column);
  ASSERT_MATRIX_ROW_INDEX(matrix, from);
  ASSERT_MATRIX_ROW_INDEX(matrix, to);
  assert(from <= to);

  #if defined __APPLE__
  printf("column %lld: ", column);
  #elif defined __linux__
  printf("column %ld: ", column);
  #else
  printf("Error: Failed to detect operating system.\n");
  assert(FALSE);
  #endif  /* Platform check */
  
  if (from > 0) { printf("..."); }
  printf("(%0.4f", matrixFloat_getAt(matrix, from, column));

  for (Int i = from + 1; i <= to; i += 1) {
    printf(", %0.4f", matrixFloat_getAt(matrix, i, column));
  }
  printf(")");
  if (to < matrix->rows - 1) { printf("..."); }
  printf("^T\n");
}

void matrixFloat_printColumn(MatrixFloat* matrix, Int column) {
  matrixFloat_printColumnFromTo(matrix, column, 0, matrix->rows - 1);
}


// Print the whole matrix
void matrixFloat_print(MatrixFloat* matrix) {
  for (Int row = 0; row < matrix->rows; row += 1) {
    matrixFloat_printRow(matrix, row);
  }
}
