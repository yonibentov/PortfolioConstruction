/* Implementation of the recurrence relation for the cargo-loading problem */

#include "recurrence.h"
#include <assert.h>
#include "search.h"

#define CARGO_LOADING_ASSERTIONS \
  assert(policies); \
  assert(returns); \
  assert(parameters); \
  assert(parameters->values); \
  assert(parameters->weights);

void recurrence(MatrixInt* policies, ArrayInt* returns,
		Parameters* parameters) {
  CARGO_LOADING_ASSERTIONS;

  /* Don't rely on the caller to have initialized the policies and returns */
  matrixInt_setAll(policies, 0);
  arrayInt_setAll(returns, 0);
  
  /* The recurrence relation for this problem is a one-term recurrence:
     The function f_n(c) is determined solely by the function f_{n-1}(c).
     That means that, internally, I'll need only one more array--to store
     the optimal return at the immediately preceding stage--instead of
     a whole matrix of returns. */

  ArrayInt* previousReturns = arrayInt_create(parameters->capacity+1);  // Initialized to zero

  // Loop over stages
  for (Int n = 1; n <= parameters->numberOfStages; n += 1) {

    Int v_n = arrayInt_getAt(parameters->values, n-1);  // Indexed from 0
    assert(v_n > 0);
    Int w_n = arrayInt_getAt(parameters->weights, n-1);  // Indexed from 0
    assert(w_n > 0);

    // Loop over all possible capacities
    for (Int c = 0; c <= parameters->capacity; c += 1) {

      Int bestPolicy = -1;
      Int bestReturn = -1;

      // Search over allowable allocations at fixed capacity and stage
      cargoLoading_search(&bestPolicy, &bestReturn,
			  previousReturns, v_n, w_n, c);

      // I now have an optimal allocation for capacity c at stage n
      matrixInt_setAt(policies, c, n-1,
		      bestPolicy);

      // Store the optimal return for capacity c at stage n
      arrayInt_setAt(returns, c,
		     bestReturn);
      
    }  // End loop over capacities at fixed stage

    // Set all previous returns to all current returns
    arrayInt_copy(previousReturns,  // Destination
		  returns);  // Source

  }  // End loop over stages

  // Destroy the internal lookup table
  arrayInt_destroy(previousReturns);
}

void output(MatrixInt* policies, ArrayInt* returns,
	    Parameters* parameters) {
  CARGO_LOADING_ASSERTIONS;

  Int numberOfStages = parameters->numberOfStages;
  Int capacity = parameters->capacity;

  /* Note that here I formatted the output slightly differently
     from how I formatted it in Sec. 3.6.5 in the paper. In the
     paper I used the notation f(state,stage), whereas here I
     use the notation f_stage(state). */
  
  printf("Optimal return:\n");
  #if defined __APPLE__
  printf("\tf_%lld(%lld) = %lld\n",
  #elif defined __linux__
  printf("\tf_%ld(%ld) = %ld\n",
  #endif /* platform check */
	 numberOfStages, capacity,
	 arrayInt_getAt(returns, capacity));

  printf("Optimal policy:\n");
  Int remainingCapacity = capacity;
  for (Int n = numberOfStages; n > 0; n -= 1) {
    Int x_n = matrixInt_getAt(policies, remainingCapacity, n-1);
    
    printf("\tx_%lld = %lld\n",
	   n, x_n);
    
    Int w_n = arrayInt_getAt(parameters->weights, n-1);
    remainingCapacity -= w_n * x_n;
  }

  printf("Capacity left unused: %lld\n",
	 remainingCapacity);
}
