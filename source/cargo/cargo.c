/* The cargo-loading problem */

#include "common.h"
#include "array.h"
#include "matrix.h"
#include "recurrence.h"


/*****************************************************/
/* Example 1: Applied Dynamic Programming, pp. 28-29 */
// #define NUMBER_OF_STAGES 3
// Int VALUES[NUMBER_OF_STAGES] = {20, 75, 102};
// Int WEIGHTS[NUMBER_OF_STAGES] = {49, 50, 51};
// #define CAPACITY 100

/* Example 2: Applied Dynamic Programming, p. 31 */
#define NUMBER_OF_STAGES 8
Int VALUES[NUMBER_OF_STAGES] = {72, 60, 40, 27, 20, 50, 85, 96};
Int WEIGHTS[NUMBER_OF_STAGES] = {20, 18, 14, 12, 10, 16, 22, 24};
#define CAPACITY 100  // also 95, 91, 89, 87, 85, 83, 81

/*****************************************************/


int main(void) {

  /* Model parameters */
  Parameters parameters = {
    .numberOfStages = NUMBER_OF_STAGES,
    .values = arrayInt_create(NUMBER_OF_STAGES),
    .weights = arrayInt_create(NUMBER_OF_STAGES),
    .capacity = CAPACITY
  };
  
  arrayInt_setFromCArray(parameters.values, VALUES,
			 parameters.numberOfStages);
  arrayInt_setFromCArray(parameters.weights, WEIGHTS,
			 parameters.numberOfStages);

  
  /* Tables to hold optimal policies and returns */
  
  // The policy matrix has to store all capacities for all stages
  MatrixInt* policies = matrixInt_create(parameters.capacity + 1,
					 parameters.numberOfStages);

  // The return only needs to be stored for the final stage
  ArrayInt* returns = arrayInt_create(parameters.capacity + 1);
  

  /* Call the recurrence relation */
  recurrence(policies, returns,
	     &parameters);

  /* Print the results */
  output(policies, returns,
	 &parameters);


  /* Destroy everything */
  
  // Model parameters
  arrayInt_destroy(parameters.values);
  arrayInt_destroy(parameters.weights);

  // Tables
  matrixInt_destroy(policies);
  arrayInt_destroy(returns);

  DONE;
}
