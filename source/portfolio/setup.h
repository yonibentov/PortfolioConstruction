/* Game and player structures for portfolio construction */

#ifndef PORTFOLIO_2D_SETUP_H
#define PORTFOLIO_2D_SETUP_H

#include "common.h"
#include "utilityFunctions.h"

typedef struct Game {
  Int numberOfGames;

  /* Properties of each game */
  Float temporalIncrement;  // dt
  Float totalTime;  // T
  Int numberOfRoundsPerGame;  // T/dt
  Float interestRate;  // r
  
  /* Properties of the wealth process */
  Float logWealthIncrement;  // dy
  Float maximumLogWealth;  // y_cutoff
  Int sizeOfLogWealthGrid;  // 2y_cutoff/dy (I want the range to be [-y_cutoff,y_cutoff])
  ArrayFloat* logWealthGrid;

  /* Properties of the stock-price process */
  Float volatility;  // sigma
  Float asymptoticMean;  // sometimes called mu, but I reserve mu for the log-price drift
  Float meanReversionSpeed;  // theta
  
  Float logPriceIncrement;  // dx
  Float maximumLogPrice;  // x_cutoff
  Int sizeOfLogPriceGrid;  // 2x_cutoff/dx
  ArrayFloat* logPriceGrid;
} Game;

typedef struct Player {
  ArrayFloat* fractionInCash;  // Initial value = 1
  ArrayFloat* fractionInStock;  // Initial value = 0
  ArrayFloat* logWealth;  // y(t) on an actual game
  ArrayFloat* trades;  // dN(t), the actual buying/selling decision after each update
  UtilityFunction* utilityFunction;  // The input that will determine a policy
} Player;
  

#endif /* PORTFOLIO_1D_SETUP_H */
