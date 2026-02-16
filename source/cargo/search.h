/* Interface for search algorithms for the cargo-loading problem */

#ifndef CARGO_LOADING_SEARCH_H
#define CARGO_LOADING_SEARCH_H

/* Search over allowable allocations at fixed capacity at stage.
   
   Inputs:
     * An array of previous returns;
     * A value (v_n, of type Int) and a weight (w_n, of type Int);
     * The current capacity (c, of type Int) being considered.
     
   Outputs:
     * bestPolicy (of type Int);
     * bestReturn (of type Int). */

#include "common.h"
#include "array.h"

void cargoLoading_search(Int* bestPolicy,  // output (read-write)
			 Int* bestReturn,  // output (read-write)
			 ArrayInt* previousReturns,  // input (read only)
			 Int value,  // input (read only)
			 Int weight,  // input (read only)
			 Int capacity  // input (read only)
			 );

#endif /* CARGO_LOADING_SEARCH_H */
