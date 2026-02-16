/******************************************************/
/* Implementation of matrix class attribute structure */
/******************************************************/

#ifndef MATRIX_PRIVATE_H
#define MATRIX_PRIVATE_H

#include "common.h"


/* Matrix with integer-valued elements */

typedef struct MatrixInt {
  Int* elements;  // Internally, the matrix will be a flattened array with strides
  Int rows;
  Int columns;
} MatrixInt;


/* Matrix with floating-point-valued elements */

typedef struct MatrixFloat {
  Float* elements;
  Int rows;
  Int columns;
} MatrixFloat;


#endif /* MATRIX_PRIVATE_H */
