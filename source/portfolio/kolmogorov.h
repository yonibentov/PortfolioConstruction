/* Interface for the Kolmogorov operator for this problem */

#ifndef PORTFOLIO_KOLMOGOROV_H
#define PORTFOLIO_KOLMOGOROV_H


#include "array.h"
#include "matrix.h"
#include "setup.h"

Float portfolio_kolmogorov(MatrixFloat* returns,   // f(x,y)
			   Int logPriceIndex, Int logWealthIndex,
			   Game* game, Float policy);


#endif /* PORTFOLIO_KOLMOGOROV_H */
