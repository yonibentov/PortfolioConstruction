/* Available utility functions for terminal-control problems */

#ifndef UTILITY_FUNCTIONS_H
#define UTILITY_FUNCTIONS_H


#include "common.h"

/* These utility functions depend on only a single variable, log-wealth.
   For example, log-utility of wealth is linear utility of log-wealth. */

typedef struct UtilityFunction {
  Float (*function)(Float);  // u(y) with y := ln(W/W_0)
  char* name;
} UtilityFunction;

#define NUMBER_OF_AVAILABLE_UTILITY_FUNCTIONS 6
typedef enum AvailableUtilityFunctions {
  LOG_UTILITY,  // linear utility of log-wealth
  SQRT_UTILITY,  // U(W) = (W/W_0)^{1/2} = e^{y/2} = u(y)
  LOG_LOG_UTILITY,  /* U(W) = ln(ln(W/W_0) + ln(W_0/W_L))
		       = ln(y + 0.69) := u(y),
		       for W_L = W_0/2 */
  KITAEV_UTILITY,  /* U(W) = ln(1+W/W_E) = ln(1 + (W_0/W_E)(W/W_0))
		           = ln(1 + (W_0/W_E)e^y) = u(y),
		      where W_E is the "external wealth" */
  TARGET_UTILITY,  /* U(W) = Step(W-W_B) = Step(y-y_B) = u(y),
		      where W_B or y_B = ln(W_B/W_0) is the
		      performance target, or benchmark */
  RELU_UTILITY  /* U(W) = ln(W/W_B) Step(W-W_B)
		        = (y-y_B) Step(y-y_B) = u(y) */
} AvailableUtilityFunctions;

/* Note that exponential utility might be a problem this way,
   since U(W) := -e^{-kW/W_0} = -e^{-ke^y}, a double exponential. */

void setUtilityFunction(UtilityFunction* utilityFunction,
			AvailableUtilityFunctions choice);



#endif /* UTILITY_FUNCTIONS_H */
