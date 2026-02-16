/* Implementation of miscellaneous functions */

#include "tools.h"
#include <assert.h>


/* Check whether an integer is even or odd */

Bool isEven(Int n) {
  assert(n > SMALLEST_INT);
  assert(n < LARGEST_INT);

  if (n == 0) {
    printf("Warning: Asked whether 0 is even\n");
  } else if (n == 1) {
    printf("Warning: Asked whether -1 is even\n");
  }

  return n % 2 == 0 ? TRUE : FALSE;
}

Bool isOdd(Int n) {
  assert(n > SMALLEST_INT);
  assert(n < LARGEST_INT);

  if (n == 0) {
    printf("Warning: Asked whether 0 is even\n");
  } else if (n == 1) {
    printf("Warning: Asked whether -1 is even\n");
  }
  
  return n % 2 != 0 ? TRUE : FALSE;
}


/* Compare two numbers */

// min(x,y)

Int minInt(Int x, Int y) {
  assert(x > SMALLEST_INT); assert(x < LARGEST_INT);
  assert(y > SMALLEST_INT); assert(y < LARGEST_INT);

  return x < y ? x : y;
}

Float minFloat(Float x, Float y) {
  assert(x > SMALLEST_FLOAT); assert(x < LARGEST_FLOAT);
  assert(y > SMALLEST_FLOAT); assert(y < LARGEST_FLOAT);

  return x < y ? x : y;
}


// max(x,y)

Int maxInt(Int x, Int y) {
  assert(x > SMALLEST_INT); assert(x < LARGEST_INT);
  assert(y > SMALLEST_INT); assert(y < LARGEST_INT);

  return x > y ? x : y;
}

Float maxFloat(Float x, Float y) {
  assert(x > SMALLEST_FLOAT); assert(x < LARGEST_FLOAT);
  assert(y > SMALLEST_FLOAT); assert(y < LARGEST_FLOAT);

  return x > y ? x : y;
}

// Compute the relative difference between "new" and "old" values
Float relativeDifferenceFloat(Float new, Float old) {
  assert(old < LARGEST_FLOAT);
  if (old <= SMALLEST_FLOAT) {
    printf("[relativeDifference()] Warning: Dividing by zero...\n");
    return LARGEST_FLOAT - 1;
  }
  assert(new > SMALLEST_FLOAT);
  assert(new < LARGEST_FLOAT);

  Float result = new/old - 1;
  REGULARIZE_FLOAT(result);
  return result;

}


/* Extremize a function */

#define TOOLS_ASSERT_FUNCTION(function, value, argument) \
  assert(f); \
  assert(value); \
  assert(argument);

// Functions of type Int to Int
#define TOOLS_ASSERT_INTEGER_DOMAIN(domainMin, domainMax, stepSize) \
  assert(domainMin > SMALLEST_INT); \
  assert(domainMax < LARGEST_INT); \
  assert(domainMin < domainMax); \
  assert(stepSize >= 1); assert(stepSize < LARGEST_INT);


void function_IntToInt_minimize(Function_IntToInt* f,  // input
				Int domainMin,  // input
				Int domainMax,  // input
				Int stepSize,  // input
				Int* minimum,  // output
				Int* argument  // output
				) {
  TOOLS_ASSERT_INTEGER_DOMAIN(domainMin, domainMax, stepSize);
  TOOLS_ASSERT_FUNCTION(f, minimum, argument);

  /* Assume that the function itself is defined to be "safe"
     over all conceivable arguments */

  *minimum = LARGEST_INT-1;  // The largest integer supported by my programs
  for (Int x = domainMin; x <= domainMax; x += stepSize) {
    if (f(x) <= *minimum) {
      *minimum = f(x);
      *argument = x;
    }
  }
}

void function_IntToInt_maximize(Function_IntToInt* f,  // input
				Int domainMin,  // input
				Int domainMax,  // input
				Int stepSize,  // input
				Int* maximum,  // output
				Int* argument  // output
				) {

  TOOLS_ASSERT_INTEGER_DOMAIN(domainMin, domainMax, stepSize);
  TOOLS_ASSERT_FUNCTION(f, maximum, argument);

  /* Assume that the function itself is defined to be "safe"
     over all conceivable arguments */

  *maximum = SMALLEST_INT+1;  // The smallest integer supported by my programs
  for (Int x = domainMin; x <= domainMax; x += stepSize) {
    if (f(x) >= *maximum) {
      *maximum = f(x);
      *argument = x;
    }
  }
}

// Functions of type Int to Float

void function_IntToFloat_minimize(Function_IntToFloat* f,  // input
				  Int domainMin,  // input
				  Int domainMax,  // input
				  Int stepSize,  // input
				  Float* minimum,  // output
				  Int* argument  // output
				) {
  TOOLS_ASSERT_INTEGER_DOMAIN(domainMin, domainMax, stepSize);
  TOOLS_ASSERT_FUNCTION(f, minimum, argument);

  /* Assume that the function itself is defined to be "safe"
     over all conceivable arguments */

  *minimum = LARGEST_FLOAT-1;  // The largest floating-point number supported by my programs
  for (Int x = domainMin; x <= domainMax; x += stepSize) {
    if (f(x) <= *minimum) {
      *minimum = f(x);
      *argument = x;
    }
  }
}

void function_IntToFloat_maximize(Function_IntToFloat* f,  // input
				  Int domainMin,  // input
				  Int domainMax,  // inpunt
				  Int stepSize,  // input
				  Float* maximum,  // output
				  Int* argument  // output
				  ) {

  TOOLS_ASSERT_INTEGER_DOMAIN(domainMin, domainMax, stepSize);
  TOOLS_ASSERT_FUNCTION(f, maximum, argument);

  /* Assume that the function itself is defined to be "safe"
     over all conceivable arguments */

  *maximum = SMALLEST_FLOAT+1;  // The smallest floating-point number supported by my programs
  for (Int x = domainMin; x <= domainMax; x += stepSize) {
    if (f(x) >= *maximum) {
      *maximum = f(x);
      *argument = x;
    }
  }
}


// Functions of type Float to Float
#define TOOLS_ASSERT_FLOAT_DOMAIN(domainMin, domainMax, stepSize) \
  assert(domainMin > SMALLEST_FLOAT); \
  assert(domainMax < LARGEST_FLOAT); \
  assert(domainMin < domainMax); \
  assert(stepSize > 0); assert(stepSize < LARGEST_FLOAT);

void function_FloatToFloat_minimize(Function_FloatToFloat* f,  // input
				    Float domainMin,  // input
				    Float domainMax,  // input
				    Float stepSize,  // input
				    Float* minimum,  // output
				    Float* argument  // output
				    ) {
  TOOLS_ASSERT_FLOAT_DOMAIN(domainMin, domainMax, stepSize);
  TOOLS_ASSERT_FUNCTION(f, minimum, argument);

  /* Assume that the function itself is defined to be "safe"
     over all conceivable arguments */

  *minimum = LARGEST_FLOAT-1;  /* The largest floating-point number
				  supported by my programs */
  for (Float x = domainMin; x <= domainMax; x += stepSize) {
    if (f(x) <= *minimum) {
      *minimum = f(x);
      *argument = x;
    }
  }
}

void function_FloatToFloat_maximize(Function_FloatToFloat* f,  // input
				    Float domainMin,  // input
				    Float domainMax,  // input
				    Float stepSize,  // input
				    Float* maximum,  // output
				    Float* argument  // output
				    ) {
  TOOLS_ASSERT_FLOAT_DOMAIN(domainMin, domainMax, stepSize);
  TOOLS_ASSERT_FUNCTION(f, maximum, argument);

  /* Assume that the function itself is defined to be "safe"
     over all conceivable arguments */

  *maximum = SMALLEST_FLOAT+1;  /* The smallest floating-point number
				   supported by my programs */
  for (Float x = domainMin; x <= domainMax; x += stepSize) {
    if (f(x) >= *maximum) {
      *maximum = f(x);
      *argument = x;
    }
  }
}
