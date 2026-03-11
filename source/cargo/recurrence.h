/* Interface for the recurrence relation for the cargo-loading problem */

#ifndef CARGO_LOADING_RECURRENCE_H
#define CARGO_LOADING_RECURRENCE_H

#include "common.h"
#include "array.h"
#include "matrix.h"

/* Collect all of the model parameters into a structure.
   (This is overkill for this circumstance, but it is necessary
   for the POSIX threading library) */

typedef struct Parameters {
  Int numberOfStages;
  ArrayInt* values;
  ArrayInt* weights;
  Int capacity;
} Parameters;

void recurrence(MatrixInt* policies, ArrayInt* returns,
		Parameters* parameters);

void output(MatrixInt* policies, ArrayInt* returns,
	    Parameters* parameters);


#endif /* CARGO_LOADING_RECURRENCE_H */
