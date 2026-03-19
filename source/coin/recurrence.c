/* Implementation of recurrence relations for the coin-betting problem */

#include "recurrence.h"
#include "tools.h"
#include <assert.h>

#define ASSERT_PARAMETERS(parameters) \
  assert(parameters->maxNumberOfChips > 0); \
  assert(parameters->maxNumberOfChips < LARGEST_INT); \
  assert(parameters->probability > 0); \
  assert(parameters->probability < 1); \
  assert(parameters->numberOfRounds > 0); \
  assert(parameters->numberOfRounds < 20); \
  assert(parameters->utilityFunction);


void __setTerminalCondition(ArrayFloat* returns,
			    Parameters* parameters) {
  assert(returns);
  ASSERT_PARAMETERS(parameters);
  
  for (Int x = 0; x <= parameters->maxNumberOfChips; x += 1) {
    Float U = parameters->utilityFunction(x);
    ASSERT_FLOAT(U);
    arrayFloat_setAt(returns, x,
		     U);
  }
}


void recurrenceExact(MatrixInt* policies, ArrayFloat* returns,
		     Parameters* parameters) {
  assert(policies);
  assert(returns);
  assert(matrixInt_getNumberOfRows(policies) == arrayFloat_getSize(returns));
  ASSERT_PARAMETERS(parameters);

  Int maxNumberOfChips = parameters->maxNumberOfChips;
  Int numberOfRounds = parameters->numberOfRounds;
  Float p = parameters->probability;
  
  /* Don't rely on the caller to have initialized the policies and returns */
  matrixInt_setAll(policies, 0);
  arrayFloat_setAll(returns, 0);

  /* Scratch table of future returns */
  ArrayFloat* futureReturns = arrayFloat_create(maxNumberOfChips+1);
  __setTerminalCondition(futureReturns, parameters);

  // Loop over rounds ("stages")
  for (Int i = numberOfRounds-1; i >= 0; i -= 1) {

    // Loop over number of chips ("states")
    for (Int x = 0; x <= maxNumberOfChips; x += 1) {

      // Scratch variables within the current state
      Float bestReturn = SMALLEST_FLOAT;  // Invalid value
      Int bestPolicy = -1;  // Invalid value

      // Search over conceivable policies
      for (Int y = 0; y <= minInt(x, maxNumberOfChips-x); y += 1) {

	Float fWin = arrayFloat_getAt(futureReturns, x+y);
	ASSERT_FLOAT(fWin);
	Float fLose = arrayFloat_getAt(futureReturns, x-y);
	ASSERT_FLOAT(fLose);

	Float candidateReturn = p * fWin + (1-p) * fLose;
	ASSERT_FLOAT(candidateReturn);

	if (candidateReturn >= bestReturn) {
	  bestReturn = candidateReturn;
	  bestPolicy = y;
	}
	
      }  // End search over policies

      /* Store the best policy for the current state */
      arrayFloat_setAt(returns, x,
		       bestReturn);
      matrixInt_setAt(policies, x, i,  // i = N-1, then N-2, then... till 0
		      bestPolicy);
    }  // End loop over states

    /* Overwrite the future returns in preparation for the next stage */
    arrayFloat_copy(futureReturns,  // Destination
		    returns);  // Source
    
  }  // End loop over stages

  /* Destroy the scratch table */
  arrayFloat_destroy(futureReturns);
}


Int __seedPolicy(Int x, Int i,  // The seed policy may depend on state and stage
		 Parameters* parameters) {
  assert(x >= 0);
  assert(x <= parameters->maxNumberOfChips);

  assert(i >= 0);
  assert(i < parameters->numberOfRounds);

  ASSERT_PARAMETERS(parameters);

  /* Try the stage-independent policy that maximizes logarithmic utility */
  Float p = parameters->probability;
  Int result = floor((2.0*p - 1.0) * x);
  assert(result >= 0);
  assert(result <= parameters->maxNumberOfChips);
  
  return result;
}

void __initializePolicyTableFromSeed(MatrixInt* policies,
				     Parameters* parameters) {
  assert(policies);
  ASSERT_PARAMETERS(parameters);
  
  assert(matrixInt_getNumberOfRows(policies) == parameters->maxNumberOfChips+1);
  assert(matrixInt_getNumberOfColumns(policies) == parameters->numberOfRounds);

  for (Int i = parameters->numberOfRounds-1; i >= 0; i -= 1) {
    for (Int x = 0; x <= parameters->maxNumberOfChips; x += 1) {
      
      /* Take care to impose the cutoff */
      Int maxPolicy = minInt(x, parameters->maxNumberOfChips - x);
      Int policy = minInt(__seedPolicy(x, i, parameters),
			  maxPolicy);
      
      matrixInt_setAt(policies, x, i,
		      policy);
    }
  }
}


void recurrenceApproximate(MatrixInt* policies, ArrayFloat* returns,
			   Parameters* parameters, Int numberOfIterations) {  
  assert(policies);
  assert(returns);
  assert(matrixInt_getNumberOfRows(policies) == arrayFloat_getSize(returns));
  ASSERT_PARAMETERS(parameters);

  Int maxNumberOfChips = parameters->maxNumberOfChips;
  Int numberOfRounds = parameters->numberOfRounds;
  Float p = parameters->probability;
  
  /* Don't rely on the caller to have initialized the policies and returns */
  matrixInt_setAll(policies, 0);
  arrayFloat_setAll(returns, 0);

  /* Scratch table of future returns */
  ArrayFloat* futureReturns = arrayFloat_create(maxNumberOfChips+1);

  __initializePolicyTableFromSeed(policies, parameters);  // New
  
  // Loop over policy iterations
  for (Int I = 0; I < numberOfIterations; I += 1) {

    printf("POLICY ITERATION %lld\n", I);
    /* Reset the terminal condition at the beginning of each new
       policy iteration */
    __setTerminalCondition(futureReturns, parameters);
    
    // Loop over rounds ("stages")
    for (Int i = numberOfRounds-1; i >= 0; i -= 1) {
      
      // Loop over chips ("states")
      for (Int x = 0; x <= maxNumberOfChips; x += 1) {

	/* Given a policy table

             {y_I(x,i)}_{x = 0,1,...,X; i = 0,1,...,N-1},

	   compute, for the current i and for all x,

             f_I(x,i) = p f_I(x+y_I(x,i),i+1) + (1-p) f_I(x-y_I(x,i),i+1). */

	/* Take care to impose the cutoff */
	Int maxPolicy = minInt(x, maxNumberOfChips-x);
	Int y = minInt(matrixInt_getAt(policies, x, i),
		       maxPolicy);
	
	Float fWin = arrayFloat_getAt(futureReturns, x+y);
	ASSERT_FLOAT(fWin);
	Float fLose = arrayFloat_getAt(futureReturns, x-y);
	ASSERT_FLOAT(fLose);

	Float currentReturn = p * fWin + (1.0-p) * fLose;
	ASSERT_FLOAT(currentReturn);

	arrayFloat_setAt(returns, x,
			 currentReturn);
	

	/* Now that the policy at the current state and stage
	   has served its purpose, update that entry of the
	   policy table according to the definition of policy
	   improvement: 

           y_{I+1}(x,i) := argmax_{y} [p f_I(x+y,i+1) + (1-p) f_I(x-y,i+1)].

         On the right-hand side are FUTURE returns (stage i+1) */

	Float bestReturn = SMALLEST_FLOAT;
        Int bestPolicy = -1;
	for (Int y = 0; y <= minInt(x,maxNumberOfChips-x); y += 1) {
	  fWin = arrayFloat_getAt(futureReturns, x+y);
	  ASSERT_FLOAT(fWin);
	  fLose = arrayFloat_getAt(futureReturns, x-y);
	  ASSERT_FLOAT(fLose);
	  
	  Float candidateReturn = p * fWin + (1.0-p) * fLose;
	  ASSERT_FLOAT(candidateReturn);
	  
	  if (candidateReturn >= bestReturn) {
	    bestReturn = candidateReturn;
	    bestPolicy = y;
	  }
	}  // End search over policies

	matrixInt_setAt(policies, x, i, 
			bestPolicy);  // Improvement of y(x,i)

      }  // End loop over states

      /* Overwrite future returns */
      arrayFloat_copy(futureReturns,  // Destination
		      returns);  // Source
      
    }  // End loop over stages

  }  // End loop over policy iterations

  arrayFloat_destroy(futureReturns);
}
