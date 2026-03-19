/* Interface for recurrence relations for the coin-betting problem */

#ifndef COIN_RECURRENCE_H
#define COIN_RECURRENCE_H

#include "common.h"
#include "array.h"
#include "matrix.h"


/* Collect all of the model parameters into a structure */

typedef struct Parameters {
  Int maxNumberOfChips;
  Float probability;
  Int numberOfRounds;
  Float (*utilityFunction)(Int x);
} Parameters;


/* Determine the optimal policy */

void recurrenceExact(MatrixInt* policies, ArrayFloat* returns,
		     Parameters* parameters);

void recurrenceApproximate(MatrixInt* policies, ArrayFloat* returns,
			   Parameters* parameters,
			   Int numberOfIterations);


#endif /* COIN_RECURRENCE_H */
