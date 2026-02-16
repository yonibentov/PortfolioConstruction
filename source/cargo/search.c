/* Implementation of search algorithms for the cargo-loading problem */

#include "search.h"
#include <assert.h>

/* Search over allowable allocations at fixed capacity and stage */

void cargoLoading_search(Int* bestPolicy, Int* bestReturn,
			 ArrayInt* previousReturns,
			 Int v_n, Int w_n, Int c) {
  assert(bestPolicy);
  assert(bestReturn);

  assert(v_n > 0); assert(v_n < LARGEST_INT);
  assert(w_n > 0); assert(w_n < LARGEST_INT);
  assert(c >= 0); assert(c < LARGEST_INT);
  
  for (Int x_n = 0; x_n <= c/w_n; x_n += 1) {
    Int previousReturn = arrayInt_getAt(previousReturns, c - w_n*x_n);
    Int incrementalReturn = v_n * x_n;

    Int candidateReturn = previousReturn + incrementalReturn;

    if (candidateReturn >= *bestReturn) {
      *bestReturn = candidateReturn;
      *bestPolicy = x_n;
    }
  }
}
