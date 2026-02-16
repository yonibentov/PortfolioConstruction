/* Interface for dynamic-programming functions for portfolio construction */

#ifndef PORTFOLIO_DYNAMIC_PROGRAMMING_H
#define PORTFOLIO_DYNAMIC_PROGRAMMING_H

#include "volume.h"
#include "matrix.h"
#include "array.h"
#include "setup.h"

void portfolio_determineOptimalPolicy(VolumeFloat* policies,  // Output, w(x,y,t)
				      MatrixFloat* returns,  // Output, f(x,y)
				      Game* game,  // Input 
				      Player* player);  // Input

void portfolio_backtest(VolumeFloat* policies,  // Input
			Game* game,  // Input
			ArrayFloat* priceHistory,  // Input
			Player* player);  /* Input: Initial positions (all cash, no stock)
					     Outputs:  - Fraction in cash,
					               - Fraction in stock,
						       - Log-wealth (i.e., performance) */

#endif /* PORTFOLIO_DYNAMIC_PROGRAMMING_H */
