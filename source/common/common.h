#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#define DONE printf("\n\ndone\n");

#define MILLION 1000000
#define BILLION 1000000000
#define PERCENT 1e-2

#include <stdint.h>
typedef int64_t Int;
#define LARGEST_INT BILLION
#define SMALLEST_INT -BILLION
#define ASSERT_INT(x) \
  assert(x > SMALLEST_INT); \
  assert(x < LARGEST_INT);
#define REGULARIZE_INT(x) \
  if (x <= SMALLEST_INT) { \
    x = SMALLEST_INT + 1; \
  } else if (x >= LARGEST_INT) { \
    x = LARGEST_INT - 1; \
  }

#include <math.h>
typedef double_t Float;
#define LARGEST_FLOAT 1.0*BILLION
#define SMALLEST_FLOAT -1.0*BILLION
#define ASSERT_FLOAT(x) \
  assert(x > SMALLEST_FLOAT); \
  assert(x < LARGEST_FLOAT);
#define REGULARIZE_FLOAT(x) \
  if (x <= SMALLEST_FLOAT) { \
    x = SMALLEST_FLOAT + 1; \
  } else if (x >= LARGEST_FLOAT) { \
    x = LARGEST_FLOAT - 1; \
  }

typedef int Bool;
#define TRUE 1
#define FALSE 0

// Largest linear dimension for an array, matrix, or volume
#define LARGEST_TABLE MILLION

#endif /* COMMON_H */
