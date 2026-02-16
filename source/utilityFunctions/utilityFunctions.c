/* Implementations of utility functions for terminal-control problems */

#include "utilityFunctions.h"
#include <assert.h>
#include <math.h>


/* Log-utility of wealth is linear utility of log-wealth */
Float __logUtility(Float logWealth) {
  REGULARIZE_FLOAT(logWealth);
  return logWealth;
}

Float __sqrtUtility(Float logWealth) {
  REGULARIZE_FLOAT(logWealth);
  Float result = exp(0.5*logWealth);
  REGULARIZE_FLOAT(result);
  return result;
}

Float __logLogUtility(Float logWealth) {
  REGULARIZE_FLOAT(logWealth);
  if (logWealth < -0.68) {
    return -23.0;  /* Log is so slow that even ln(10^{-10})
		      is only about -23 */
  } else {
    Float result = log(logWealth + 0.69);
    ASSERT_FLOAT(result);
    return result;
  }
}

Float __kitaevUtility(Float logWealth) {
  REGULARIZE_FLOAT(logWealth);
  /* The parameter is W_0/W_E, the ratio of
     initial wealth to external wealth.
     The interesting regime is when
       W_E > W_0
     so that the player is more tolerant
     of risk than a log-maximizer, but
     less tolerant of risk than a linear
     maximizer. */

  Float ratio = 0.5;
  Float e = exp(logWealth);
  REGULARIZE_FLOAT(e);

  /* u(y) = ln(1 + ratio*e) */
  Float result = log(1.0 + ratio*e);
  ASSERT_FLOAT(result);
  return result;
}

Float __targetUtility(Float logWealth) {
  REGULARIZE_FLOAT(logWealth);
  Float benchmark = 0.10;  // log(110%) = 0.0953
  if (logWealth > benchmark) {
    return 1;
  } else {
    return 0;
  }
}

Float __reluUtility(Float logWealth) {
  REGULARIZE_FLOAT(logWealth);
  Float benchmark = 0.10;  // log(110%) = 0.0953
  if (logWealth > benchmark) {
    return logWealth - benchmark;
  } else {
    return 0;
  }
}


void setUtilityFunction(UtilityFunction* utilityFunction,
			AvailableUtilityFunctions choice) {
  assert(utilityFunction);
  
  switch(choice) {
  case LOG_UTILITY:
    utilityFunction->function = &__logUtility;
    utilityFunction->name = "log";
    break;
  case SQRT_UTILITY:
    utilityFunction->function = &__sqrtUtility;
    utilityFunction->name = "sqrt";
    break;
  case LOG_LOG_UTILITY:
    utilityFunction->function = &__logLogUtility;
    utilityFunction->name = "loglog";
    break;
  case KITAEV_UTILITY:
    utilityFunction->function = &__kitaevUtility;
    utilityFunction->name = "kitaev";
    break;
  case TARGET_UTILITY:
    utilityFunction->function = &__targetUtility;
    utilityFunction->name = "target";
    break;
  case RELU_UTILITY:
    utilityFunction->function = &__reluUtility;
    utilityFunction->name = "relu";
    break;
  default:
    printf("Warning: Unsupported utility function. Defaulting to log(x).\n");
    utilityFunction->function = &__logUtility;
    utilityFunction->name = "log";
  }
}
