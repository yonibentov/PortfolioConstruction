#include "basePath.h"
#include "common.h"
#include "array.h"
#include "strings.h"
#include "csv.h"

#include <assert.h>
#include <math.h>

Float U(Int x) {
  Float xf = (Float)x;
  if (xf < 1.01) { return SMALLEST_FLOAT + 1; }
  else {
    Float x_L = 0.5;
    Float result = log(log(xf/x_L));
    //    Float result = log(xf);
    ASSERT_FLOAT(result);
    return result;
  }
}

Int maxNumberOfCoins = 100;
Float p = 0.75;

Int seedPolicy(Int x) {
  Int result = floor(2.0*p - 1) * x;
  ASSERT_INT(result);
  return result;
}

int main(void) {

  ArrayFloat* returns = arrayFloat_create(maxNumberOfCoins + 1);  // including 0
  ArrayInt* policies = arrayInt_create(maxNumberOfCoins + 1);

  String* path = string_create();
  //  char* basePath = "/Users/yoni/Library/CloudStorage/Dropbox/Finance/Programs 2025/files/bettingOnce";
  
  Data output = {
    .file = NULL,
    .numberOfRows = arrayFloat_getSize(returns),
    .numberOfColumns = 1
  };

  /* Record the simulation parameters */
  string_overwriteWithCharacters(path, basePathLiteral);
  string_appendCharacters(path, "parameters.txt");
  output.file = fopenFromString(path, "w");
  fprintf(output.file, "max number of coins: %lld\nprobability of heads: %0.2f\n",
	  maxNumberOfCoins, p);
  fclose(output.file);
  
  /******************************************/
  /* The exact dynamic-programming solution */
  /******************************************/
  
  for (Int x = 0; x <= maxNumberOfCoins; x += 1) {
    // Scratch variables for the current state
    Float bestReturn = SMALLEST_FLOAT;  // invalid value
    Int bestPolicy = -1;  // invalid value
    
    // Search over conceivable policies
    for (Int y = 0; y <= minInt(x,maxNumberOfCoins-x); y += 1) {
      Float candidateReturn = p * U(x+y) + (1-p) * U(x-y);
      ASSERT_FLOAT(candidateReturn);

      if (candidateReturn >= bestReturn) {
	bestReturn = candidateReturn;
	bestPolicy = y;
      }
    }  // end search over policies

    // Store the best policy and return for the current state
    arrayFloat_setAt(returns, x,
		     bestReturn);
    arrayInt_setAt(policies, x,
		   bestPolicy);
    
  }

  /* Output the exact solution */
  arrayInt_print(policies);
  string_overwriteWithCharacters(path, basePathLiteral);
  string_appendCharacters(path, "exactPolicy.csv");
  output.file = fopenFromString(path, "w");
  csv_dataFromArrayInt(&output, policies);
  fclose(output.file);
  
  
  /*********************************/
  /* Approximation in policy space */
  /*********************************/

  arrayFloat_setAll(returns, 0);
  arrayInt_setAll(policies, 0);

  // Define the zeroth-order return
  for (Int x = 0; x <= maxNumberOfCoins; x += 1) {
    Float result = p * U(x + seedPolicy(x)) + (1-p) * U(x - seedPolicy(x));
    ASSERT_FLOAT(result);
    arrayFloat_setAt(returns, x,
		     result);
  }
  
  for (Int x = 0; x <= maxNumberOfCoins; x += 1) {
    // Scratch variables for the current state
    Float bestReturn = SMALLEST_FLOAT;  // invalid value
    Int bestPolicy = -1;  // invalid value

    // Search over conceivable policies
    for (Int y = 0; y <= minInt(x,maxNumberOfCoins-x); y += 1) {
      Float f0Plus = arrayFloat_getAt(returns, x + y);  // right, the "expanding grid"
      Float f0Minus = arrayFloat_getAt(returns, x - y);
      Float candidateReturn = p * f0Plus + (1-p) * f0Minus;
      if (candidateReturn >= bestReturn) {
	bestReturn = candidateReturn;
	bestPolicy = y;
      }
    } // end search over policies

    // Store the best policy and return for the current state
    arrayFloat_setAt(returns, x,
		     bestReturn);
    arrayInt_setAt(policies, x,
		   bestPolicy);    
  }
  
  /* Output the approximate solution */
  arrayInt_print(policies);
  string_overwriteWithCharacters(path, basePathLiteral);
  string_appendCharacters(path, "approximatePolicy.csv");
  output.file = fopenFromString(path, "w");
  csv_dataFromArrayInt(&output, policies);
  fclose(output.file);

  
  /* Destroy everything */
  
  arrayFloat_destroy(returns);
  arrayInt_destroy(policies);
  string_destroy(path);

  DONE;
}
