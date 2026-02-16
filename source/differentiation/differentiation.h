/*******************************************/
/* Interface for differentiation functions */
/*******************************************/

#ifndef DIFFERENTIATION_H
#define DIFFERENTIATION_H

#include "common.h"
#include "array.h"
#include "matrix.h"


/**********************/
/* Analytic functions */
/**********************/

typedef Float Function1d(Float x);

/* Differentiation based on straightforward discretization */
Float differentiation_derivativeAt(Function1d* function, Float x, Float dx);
Float differentiation_secondDerivativeAt(Function1d* function, Float x, Float dx);


/*********************/
/* Tabular functions */
/*********************/

/* Functions of a single variable */
Float differentiation_arrayFloat_differenceAt(ArrayFloat* function, Int index);
Float differentiation_arrayFloat_secondDifferenceAt(ArrayFloat* function, Int index);

/* Functions of two variables */
Float differentiation_matrixFloat_partialDifferenceAt(MatrixFloat* function,
						      Int index1, Int index2,
						      Int direction);  // 1 or 2
Float differentiation_matrixFloat_secondPartialDifferenceAt(MatrixFloat* function,
							    Int index1, Int index2,
							    Int direction1,  // 1 or 2
							    Int direction2);  // 1 or 2

#endif /* DIFFERENTIATION_H */
