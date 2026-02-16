/* Implementation of the Kolmogorov operator for this problem,
   in terms of log-price,
   
     x := ln(S/S_0),

   and log-wealth,

     y := ln(W/W_0) .

   The operator is:

     K_w(x,d/dx;y,d/dy) = sigma^2[(1/2) d^2/dx^2 + w d/dx d/dy + (1/2) w^2 d^2/dy^2]
                          + mu(x) d/dx + nu(x,y) d/dy,

   with

     mu(x) = -theta * (x-m),
     nu(x,y) = w * mu + (1-w) * r + w(1-w) * sigma^2/2. */

#include "kolmogorov.h"
#include "differentiation.h"
#include <assert.h>

extern Float policyTolerance;

Float portfolio_kolmogorov(MatrixFloat* returns,   // f(x,y)
			   Int logPriceIndex, Int logWealthIndex,
			   Game* game, Float policy) {
  assert(game);

  Float sigma = game->volatility;
  assert(sigma > 0); assert(sigma < LARGEST_FLOAT);

  Float m = game->asymptoticMean;
  assert(m > -game->maximumLogPrice);
  assert(m < game->maximumLogPrice);

  Float theta = game->meanReversionSpeed;
  assert(theta > 0);  // theta = 0 reduces to Brownian motion
  assert(theta < LARGEST_FLOAT);

  Float r = game->interestRate;
  assert(r > 0); assert(r < LARGEST_FLOAT);

  assert(returns);
  Int logPriceGridSize = matrixFloat_getNumberOfRows(returns);
  assert(logPriceIndex >= 0); assert(logPriceIndex < logPriceGridSize);
  
  Int logWealthGridSize = matrixFloat_getNumberOfColumns(returns);
  assert(logWealthIndex >= 0); assert(logWealthIndex < logWealthGridSize);

  /* Out-of-bounds checks are included in my differentiation functions */

  // Log-price-dependent drift
  Float x = arrayFloat_getAt(game->logPriceGrid, logPriceIndex);
  ASSERT_FLOAT(x);
  Float mu = -theta * (x-m);
  ASSERT_FLOAT(mu);
  
  // First-derivative terms
  Float coefficient_x = mu;
  ASSERT_FLOAT(coefficient_x);
  Float f_x = differentiation_matrixFloat_partialDifferenceAt(returns,
							      logPriceIndex,
							      logWealthIndex,
							      1);
  ASSERT_FLOAT(f_x);

  Float coefficient_y = policy*mu + (1.0-policy)*r + 0.5*policy*(1.0-policy)*sigma*sigma;
  ASSERT_FLOAT(coefficient_y);
  Float f_y = differentiation_matrixFloat_partialDifferenceAt(returns,
							      logPriceIndex,
							      logWealthIndex,
							      2);
  ASSERT_FLOAT(f_y);

  Float firstDerivativeTerms = coefficient_x * f_x + coefficient_y * f_y;
  ASSERT_FLOAT(firstDerivativeTerms);
  
  // Second-derivative terms
  Float f_xx = differentiation_matrixFloat_secondPartialDifferenceAt(returns,
								     logPriceIndex,
								     logWealthIndex,
								     1,1);
  ASSERT_FLOAT(f_xx);

  Float f_yy = differentiation_matrixFloat_secondPartialDifferenceAt(returns,
								     logPriceIndex,
								     logWealthIndex,
								     2,2);
  ASSERT_FLOAT(f_yy);

  Float f_xy = differentiation_matrixFloat_secondPartialDifferenceAt(returns,
								     logPriceIndex,
								     logWealthIndex,
								     1,2);
  ASSERT_FLOAT(f_xy);

  Float secondDerivativeTerms = sigma*sigma*(0.5*f_xx
					     + policy*f_xy
					     + 0.5*policy*policy*f_yy);
  ASSERT_FLOAT(secondDerivativeTerms);

  // Put everything together
  Float result = firstDerivativeTerms + secondDerivativeTerms;
  REGULARIZE_FLOAT(result);

  return result;
}
