/***********************************************/
/* Implementation of differentiation functions */
/***********************************************/

#include "differentiation.h"
#include "tools.h"
#include <assert.h>

Float _differentiation_tolerance = 1e-2;
Float _differentiation_smallestIncrement = 1e-4;
Float _differentiation_largestIncrement = 1e0;
Float _differentiation_incrementOfIncrement = 1e-3;
Float _differentiation_largestFirstDerivativeRatio = 1e1;
Float _differentiation_smallestFirstDerivativeRatio = 1e-1;

#define __ASSERT_INCREMENT(dx) \
  assert(dx >= _differentiation_smallestIncrement); \
  assert(dx <= _differentiation_largestIncrement);


/**********************/
/* Analytic functions */
/**********************/

Float differentiation_derivativeAt(Function1d* f, Float x, Float dx) {
  assert(f);
  ASSERT_FLOAT(x);
  __ASSERT_INCREMENT(dx);

  Float fLeft = f(x-dx);
  ASSERT_FLOAT(fLeft);
  
  Float fRight = f(x+dx);
  ASSERT_FLOAT(fRight);

  Float result = (fRight - fLeft)/(2.0*dx);
  REGULARIZE_FLOAT(result);
  ASSERT_FLOAT(result);

  return result;
}

Float differentiation_secondDerivativeAt(Function1d* function, Float x, Float dx) {
  assert(function);
  ASSERT_FLOAT(x);
  __ASSERT_INCREMENT(dx);

  Float f = function(x);
  ASSERT_FLOAT(f);
  
  Float fLeft = function(x-dx);
  ASSERT_FLOAT(fLeft);

  Float fRight = function(x+dx);
  ASSERT_FLOAT(fRight);

  Float result = (fRight - 2.0*f + fLeft)/(dx*dx);
  REGULARIZE_FLOAT(result);
  ASSERT_FLOAT(result);

  return result;
}


/*********************/
/* Tabular functions */
/*********************/

/* Functions of a single variable */

Float differentiation_arrayFloat_differenceAt(ArrayFloat* function, Int index) {
  assert(function);
  assert(index >= 0);
  assert(index < arrayFloat_getSize(function));

  Float fLeft = 0;
  Float fRight = 0;
  if (index > 0) {
    fLeft = arrayFloat_getAt(function, index-1);
  }
  if (index < arrayFloat_getSize(function)-1) {
    fRight = arrayFloat_getAt(function, index+1);
  }

  Float result = fRight - fLeft;
  REGULARIZE_FLOAT(result);

  return result;
}

Float __differentiation_arrayFloat_averageAt(ArrayFloat* function, Int index) {
  assert(function);
  assert(index >= 0);
  assert(index < arrayFloat_getSize(function));

  Float fLeft = 0;
  Float fRight = 0;
  if (index > 0) {
    fLeft = arrayFloat_getAt(function, index-1);
  }
  if (index < arrayFloat_getSize(function)-1) {
    fRight = arrayFloat_getAt(function, index+1);
  }

  Float result = 0.5*(fRight + fLeft);
  REGULARIZE_FLOAT(result);

  return result;
}

Float __differentiation_arrayFloat_differenceOverAverageAt(ArrayFloat* function, Int index) {
  Float result = differentiation_arrayFloat_differenceAt(function, index) /
                 __differentiation_arrayFloat_averageAt(function, index);
  REGULARIZE_FLOAT(result);
  return result;
}


Float differentiation_arrayFloat_secondDifferenceAt(ArrayFloat* function, Int index) {
  assert(function);
  assert(index >= 0);
  assert(index < arrayFloat_getSize(function));

  Float f = arrayFloat_getAt(function, index);

  Float fLeft = 0;
  Float fRight = 0;
  if (index > 0) {
    fLeft = arrayFloat_getAt(function, index-1);
  }
  if (index < arrayFloat_getSize(function)-1) {
    fRight = arrayFloat_getAt(function, index+1);
  }

  Float result = fRight - 2*f + fLeft;
  REGULARIZE_FLOAT(result);

  return result;
}


/* Functions of two variables */

Float differentiation_matrixFloat_partialDifferenceAt(MatrixFloat* function,
						      Int index1, Int index2,
						      Int direction) {
  assert(function);
  assert(index1 >= 0);
  assert(index1 < matrixFloat_getNumberOfRows(function));
  assert(index2 >= 0);
  assert(index2 < matrixFloat_getNumberOfColumns(function));
  assert(direction == 1 || direction == 2);
  
  Float result = 0;

  if (direction == 1) {
    Float fLeft = 0;
    Float fRight = 0;
    if (index1 > 0) {
      fLeft = matrixFloat_getAt(function, index1-1, index2);
    }
    if (index1 < matrixFloat_getNumberOfColumns(function)-1) {
      fRight = matrixFloat_getAt(function, index1+1, index2);
    }

    result = fRight - fLeft;
  }
  
  if (direction == 2) {
    Float fDown = 0;
    Float fUp = 0;
    if (index2 > 0) {
      fDown = matrixFloat_getAt(function, index1, index2-1);
    }
    if (index2 < matrixFloat_getNumberOfColumns(function)-1) {
      fUp = matrixFloat_getAt(function, index1, index2+1);
    }

    result = fUp - fDown;
  }
  
  REGULARIZE_FLOAT(result);

  return result;
}

Float differentiation_matrixFloat_secondPartialDifferenceAt(MatrixFloat* function,
							    Int index1, Int index2,
							    Int direction1,
							    Int direction2) {
  assert(function);
  assert(index1 >= 0);
  assert(index1 < matrixFloat_getNumberOfRows(function));
  assert(index2 >= 0);
  assert(index2 < matrixFloat_getNumberOfColumns(function));
  assert(direction1 == 1 || direction1 == 2);
  assert(direction2 == 1 || direction2 == 2);

  Float result = 0;

  if (direction1 == direction2) {
    Float f = matrixFloat_getAt(function, index1, index2);
    
    if (direction1 == 1) {  // partial_x^2
      Float fLeft = 0;
      Float fRight = 0;

      if (index1 > 0) {
	fLeft = matrixFloat_getAt(function, index1-1, index2);
      }
      if (index1 < matrixFloat_getNumberOfRows(function)-1) {
	fRight = matrixFloat_getAt(function, index1+1, index2);
      }

      result = fRight - 2.0*f + fLeft;

    } else {  // partial_y^2
      Float fDown = 0;
      Float fUp = 0;

      if (index2 > 0) {
	fDown = matrixFloat_getAt(function, index1, index2-1);
      }
      if (index2 < matrixFloat_getNumberOfRows(function)-1) {
	fUp = matrixFloat_getAt(function, index1, index2+1);
      }

      result = fUp - 2.0*f + fDown;
    }
  } else {  // partial_xy = partial_yx, assuming the function is continuous
    Float fLeftDown = 0;
    Float fLeftUp = 0;
    Float fRightDown = 0;
    Float fRightUp = 0;

    if (index1 > 0 && index2 > 0) {
      fLeftDown = matrixFloat_getAt(function, index1-1, index2-1);
    }
    if (index1 > 0 && index2 < matrixFloat_getNumberOfColumns(function)-1) {
      fLeftUp = matrixFloat_getAt(function, index1-1, index2+1);
    }
    if (index1 < matrixFloat_getNumberOfRows(function)-1 && index2 > 0) {
      fRightDown = matrixFloat_getAt(function, index1+1, index2-1);
    }
    if (index1 < matrixFloat_getNumberOfRows(function)-1 &&
	index2 < matrixFloat_getNumberOfColumns(function)-1) {
      fRightUp = matrixFloat_getAt(function, index1+1, index2+1);
    }

    result = 0.25 * (fLeftDown - fLeftUp - fRightDown + fRightUp);
  }

  REGULARIZE_FLOAT(result);

  return result;
}
