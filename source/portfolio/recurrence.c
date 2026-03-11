/* Determine the optimal policy for trading geometric mean-reversion */

#include "dynamicProgramming.h"
#include "kolmogorov.h"
#include <assert.h>

Float _invalidReturn = SMALLEST_FLOAT+1;
Float _invalidPolicy = SMALLEST_FLOAT+1;

/* Even though this problem has a return function f = f(x,y),
   I will still use utility functions that depend only on y. */

void __setTerminalCondition(MatrixFloat* futureReturns, Game* game,
			    UtilityFunction* utilityFunction);  // Set f(x,y,T) = u(y)
void __searchOverPolicies(Int logPriceIndex, Int logWealthIndex,
			  MatrixFloat* futureReturns, Game* game,
			  Float* bestPolicy, Float* bestKf);  // Maximize K_w f(x,y,t+dt)
Float __updateOptimalReturn(Int logPriceIndex, Int logWealthIndex,
			    MatrixFloat* futureReturns, Game* game,
			    Float* bestKf);  // Compute (1+dt K_w*)f(x,y,t+dt)

void portfolio_determineOptimalPolicy(VolumeFloat* policies, MatrixFloat* returns,
				      Game* game, Player* player) {
  assert(policies); assert(returns);
  assert(game); assert(player);

  Int numberOfRoundsPerGame = game->numberOfRoundsPerGame;
  Int sizeOfLogPriceGrid = game->sizeOfLogPriceGrid;
  assert(sizeOfLogPriceGrid == matrixFloat_getNumberOfRows(returns));
  Int sizeOfLogWealthGrid = game->sizeOfLogWealthGrid;
  assert(sizeOfLogWealthGrid == matrixFloat_getNumberOfColumns(returns));

  assert(volumeFloat_getWidth(policies) == sizeOfLogPriceGrid);
  assert(volumeFloat_getHeight(policies) == sizeOfLogWealthGrid);
  assert(volumeFloat_getDepth(policies) == numberOfRoundsPerGame);

  assert(matrixFloat_getNumberOfRows(returns) == sizeOfLogPriceGrid);
  assert(matrixFloat_getNumberOfColumns(returns) == sizeOfLogWealthGrid);
  
  // Scratch table for future returns
  MatrixFloat* futureReturns = matrixFloat_create(matrixFloat_getNumberOfRows(returns),
						  matrixFloat_getNumberOfColumns(returns));
  __setTerminalCondition(futureReturns, game, player->utilityFunction);

  matrixFloat_setAll(returns, _invalidReturn);
  volumeFloat_setAll(policies, _invalidPolicy);

  // Loop over stages
  for (Int timeIndex = 0; timeIndex < numberOfRoundsPerGame; timeIndex += 1) {

    // Loop over states
    for (Int logPriceIndex = 0; logPriceIndex < sizeOfLogPriceGrid; logPriceIndex += 1) {
      for (Int logWealthIndex = 0; logWealthIndex < sizeOfLogWealthGrid; logWealthIndex += 1) {
	// Temporary variables for the best policy and return for the current state and stage
	Float bestPolicy = _invalidPolicy;
	Float bestKf = _invalidReturn;  /* K_w f(x,y,t) has to be distinguished from (1+dt K_w)f(x,y,t+dt).
					   The best RETURN at the current state and stage is
					   f(x,y,t) = (1+dt K_w)f(x,y,t+dt). */

	// Search over conceivable policies
	__searchOverPolicies(logPriceIndex, logWealthIndex,  // current state
			     futureReturns,  /* The policy is defined to maximize the
						right-hand side of the recurrence relation */
			     game,  // Includes model parameters
			     &bestPolicy, &bestKf);  // outputs
	Float bestReturn = __updateOptimalReturn(logPriceIndex, logWealthIndex,
						 futureReturns,
						 game,
						 &bestKf);  // Compute (1+dt K_w*)f(x,y,t+dt)

	/* At this point, I know the best policy and return for the
	   current state and stage. Store them */
	volumeFloat_setAt(policies,
			  logPriceIndex, logWealthIndex, timeIndex,
			  bestPolicy);
	matrixFloat_setAt(returns,
			  logPriceIndex, logWealthIndex,
			  bestReturn);
      }  // End loop over y = ln(W/W_0)
    }  // End loop over x = ln(S/S_0)

    /* Overwrite the table of future returns with the updated returns,
       in preparation for the next stage */
    matrixFloat_copy(futureReturns,  // Destination
		     returns);  // Source
    
  }  // End loop over stages

  // Destroy the scratch table of future returns
  matrixFloat_destroy(futureReturns);
}


void __setTerminalCondition(MatrixFloat* futureReturns, Game* game,
			    UtilityFunction* utilityFunction) {
  assert(futureReturns);
  assert(game);
  assert(utilityFunction);

  Float dx = game->logPriceIncrement;
  Float dy = game->logWealthIncrement;

  for (Int i = 0; i < matrixFloat_getNumberOfRows(futureReturns); i += 1) {
    for (Int j = 0; j < matrixFloat_getNumberOfColumns(futureReturns); j += 1) {
      Float y = -game->maximumLogWealth + j*dy;
      matrixFloat_setAt(futureReturns, i, j,
			utilityFunction->function(y));
    }
  }
}

Float policyTolerance = 0.01;
void __searchOverPolicies(Int logPriceIndex, Int logWealthIndex,
			  MatrixFloat* futureReturns, Game* game,
			  Float* bestPolicy, Float* bestKf) {

  Float dw = 0.10;  // 0.01 is taking too long
  for (Float w = -0.25;  /* Give it some ability to short */
       w < 1 + policyTolerance; w += dw) {
    Float candidateKf = portfolio_kolmogorov(futureReturns,
					     logPriceIndex, logWealthIndex,
					     game, w);
    if (candidateKf >= *bestKf) {
      *bestKf = candidateKf;
      *bestPolicy = w;
    }
  }
}

Float __updateOptimalReturn(Int logPriceIndex, Int logWealthIndex,
			    MatrixFloat* futureReturns, Game* game,
			    Float* bestKf) {
  assert(futureReturns); assert(game);
  assert(logPriceIndex >= 0);
  assert(logPriceIndex < matrixFloat_getNumberOfRows(futureReturns));
  assert(logWealthIndex >= 0);
  assert(logWealthIndex < matrixFloat_getNumberOfColumns(futureReturns));

  Float dt = game->temporalIncrement;
  Float f = matrixFloat_getAt(futureReturns, logPriceIndex, logWealthIndex);
  ASSERT_FLOAT(f);

  Float result = f + dt * (*bestKf);
  REGULARIZE_FLOAT(result);

  return result;
}
