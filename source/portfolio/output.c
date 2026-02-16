/* Implementation of the output phase, or backtest, of the dynamic-programming problem */

#include "dynamicProgramming.h"
#include <assert.h>
#include <math.h>

void portfolio_backtest(VolumeFloat* policies,  // Input
			Game* game,  // Input
			ArrayFloat* priceHistory,  // Input
			Player* player) {  /* Input: Initial positions
					      Outputs: - Fraction in cash,
						       - Fraction in stock,
						       - Log-wealth (i.e., performance),
						       - Trades */
  assert(game);
  assert(game->logPriceGrid);
  assert(game->logWealthGrid);
  // to-do: more assertions
  Int sizeOfLogPriceGrid = game->sizeOfLogPriceGrid;
  Int sizeOfLogWealthGrid = game->sizeOfLogWealthGrid;
  
  assert(policies);
  assert(volumeFloat_getWidth(policies) == sizeOfLogPriceGrid);
  assert(volumeFloat_getHeight(policies) == sizeOfLogWealthGrid);

  Int numberOfRoundsPerGame = game->numberOfRoundsPerGame;
  assert(volumeFloat_getDepth(policies) == numberOfRoundsPerGame);
  
  assert(priceHistory);
  assert(arrayFloat_getSize(priceHistory) == numberOfRoundsPerGame+1);

  assert(player);
  assert(player->fractionInCash);
  assert(arrayFloat_getSize(player->fractionInCash) == numberOfRoundsPerGame+1);

  assert(player->fractionInStock);
  assert(arrayFloat_getSize(player->fractionInStock) == numberOfRoundsPerGame+1);

  assert(player->logWealth);
  assert(arrayFloat_getSize(player->logWealth) == numberOfRoundsPerGame+1);

  assert(player->trades);
  assert(arrayFloat_getSize(player->trades) == numberOfRoundsPerGame);

  assert(player->utilityFunction);

  arrayFloat_setAll(player->fractionInCash, 1);
  arrayFloat_setAll(player->fractionInStock, 0);
  arrayFloat_setAll(player->logWealth, 0);  // y := ln(W/W_0) => y_0 = ln(1) = 0
  arrayFloat_setAll(player->trades, 0);
  Float originalPrice = arrayFloat_getAt(priceHistory, 0);
  Float oldNumberOfShares = 0;
  Float wealthScale = 1;
  for (Int round = 1; round <= numberOfRoundsPerGame; round += 1) {

    /* Old values */
    Float oldPrice = arrayFloat_getAt(priceHistory, round-1);
    Float oldLogPrice = log(oldPrice/originalPrice);
    Float oldLogWealth = arrayFloat_getAt(player->logWealth, round-1);
    Int oldPolicyLogPriceIndex = arrayFloat_indexFromValue(game->logPriceGrid,
							   oldLogPrice);
    Int oldPolicyLogWealthIndex = arrayFloat_indexFromValue(game->logWealthGrid,
							    oldLogWealth);
    Float oldPolicy = 0;
    if (round > 1) {
      oldPolicy = volumeFloat_getAt(policies,
				    oldPolicyLogPriceIndex,
				    oldPolicyLogWealthIndex,
				    round-2);  /* i think that should be round-2,
						      with index = -1 hardcoded */
    }
    assert(oldPolicy > -1);
    Float oldWealth = exp(oldLogWealth) * wealthScale;
    //    if (round > 1) {
    oldNumberOfShares = oldPolicy * oldWealth / oldPrice;
    //    }

    /* New values */
    Float newPrice = arrayFloat_getAt(priceHistory, round);
    Float newLogPrice = log(newPrice/originalPrice);

    Float NdS = oldNumberOfShares * (newPrice - oldPrice);
    Float rdt = (game->interestRate) * (game->temporalIncrement);
    Float NS = oldNumberOfShares * oldPrice;
    Float dW = NdS + (oldWealth - NS)*rdt;
    Float newWealth = oldWealth + dW;

    Float newLogWealth = log(newWealth/wealthScale);
    Int newPolicyLogPriceIndex = arrayFloat_indexFromValue(game->logPriceGrid,
							   newLogPrice);
    Int newPolicyLogWealthIndex = arrayFloat_indexFromValue(game->logWealthGrid,
							    newLogWealth);
    Float newPolicy = volumeFloat_getAt(policies,
					newPolicyLogPriceIndex,
					newPolicyLogWealthIndex,
					round-1);
    assert(newPolicy > -1); assert(newPolicy < 1 + 1e-2);

    Float newNumberOfShares = newPolicy * newWealth / newPrice;
    Float trade = newNumberOfShares - oldNumberOfShares;

    /* Store the new values */
    arrayFloat_setAt(player->trades, round-1,
		     trade);
    arrayFloat_setAt(player->logWealth, round,
		     newLogWealth);
    arrayFloat_setAt(player->fractionInStock, round,
		     newPolicy);
    arrayFloat_setAt(player->fractionInCash, round,
		     1.0-newPolicy);
  }
}
