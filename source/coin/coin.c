#include "basePath.h"
#include "common.h"
#include "tools.h"
#include "array.h"
#include "matrix.h"
#include "strings.h"
#include "csv.h"
#include "recurrence.h"

#include <assert.h>
#include <math.h>

/* Utility function */
Float U(Int x) {
  Float xf = (Float)x;
  Float result = tanh(0.1*(xf-50.0));
  ASSERT_FLOAT(result);
  return result;
}

/* Main */
int main(void) {

  /*********/
  /* Setup */
  /*********/
  
  Parameters parameters = {
    .maxNumberOfChips = 100,
    .probability = 0.75,
    .numberOfRounds = 10,
    .utilityFunction = &U
  };
  
  ArrayFloat* returns = arrayFloat_create(parameters.maxNumberOfChips+1);  // Including 0
  MatrixInt* policies = matrixInt_create(parameters.maxNumberOfChips+1,
					 parameters.numberOfRounds);

  // I/O Setup
  String* path = string_create();
  
  Data output = {
    .file = NULL,
    .numberOfRows = arrayFloat_getSize(returns),
    .numberOfColumns = 1
  };

  
  /* Write the simulation parameters to storage */
  
  string_overwriteWithCharacters(path, basePathLiteral);
  string_appendCharacters(path, "parameters.txt");
  output.file = fopenFromString(path, "w");
  fprintf(output.file, "Max number of chips: %lld\nProbability of heads: %0.2f\n",
	  parameters.maxNumberOfChips, parameters.probability);
  fclose(output.file);

  
  /******************************************/
  /* The exact dynamic-programming solution */
  /******************************************/

  recurrenceExact(policies, returns, &parameters);

  
  /* Output the exact solution to the console */

  printf("Final decision (exact):\n");
  matrixInt_printColumn(policies, parameters.numberOfRounds-1);
  printf("Penultimate decision (exact):\n");
  matrixInt_printColumn(policies, parameters.numberOfRounds-2);
  printf("First decision (exact):\n");
  matrixInt_printColumn(policies, 0);


  /* Write the exact solution to storage */
  
  // Final decision
  string_overwriteWithCharacters(path, basePathLiteral);
  string_appendCharacters(path, "finalDecision_exact.csv");
  output.file = fopenFromString(path, "w");
  csv_dataFromMatrixIntColumn(&output, policies, parameters.numberOfRounds-1);
  fclose(output.file);
  
  // Penultimate decision
  string_overwriteWithCharacters(path, basePathLiteral);
  string_appendCharacters(path, "penultimateDecision_exact.csv");
  output.file = fopenFromString(path, "w");
  csv_dataFromMatrixIntColumn(&output, policies, parameters.numberOfRounds-2);
  fclose(output.file);

  // First decision
  string_overwriteWithCharacters(path, basePathLiteral);
  string_appendCharacters(path, "firstDecision_exact.csv");
  output.file = fopenFromString(path, "w");
  csv_dataFromMatrixIntColumn(&output, policies, 0);
  fclose(output.file);
  

  printf("\n");
  
  
  /*********************************/
  /* Approximation in policy space */
  /*********************************/

  Int numberOfIterations = 1; /* The paper is concerned with one iteration,
			         but this program can do more */
  recurrenceApproximate(policies, returns, &parameters,
			numberOfIterations);

  
  /* Output the approximate solution to the console */
  
  printf("Final decision (approximate):\n");
  matrixInt_printColumn(policies, parameters.numberOfRounds-1);
  printf("Penultimate decision (approximate):\n");
  matrixInt_printColumn(policies, parameters.numberOfRounds-2);
  printf("First decision (approximate):\n");
  matrixInt_printColumn(policies, 0);

  
  /* Write the approximate solution to storage */
  
  // Final decision
  string_overwriteWithCharacters(path, basePathLiteral);
  string_appendCharacters(path, "finalDecision_approximate.csv");
  output.file = fopenFromString(path, "w");
  csv_dataFromMatrixIntColumn(&output, policies, parameters.numberOfRounds-1);
  fclose(output.file);

  // Penultimate decision
  string_overwriteWithCharacters(path, basePathLiteral);
  string_appendCharacters(path, "penultimateDecision_approximate.csv");
  output.file = fopenFromString(path, "w");
  csv_dataFromMatrixIntColumn(&output, policies, parameters.numberOfRounds-2);
  fclose(output.file);
  
  // First decision
  string_overwriteWithCharacters(path, basePathLiteral);
  string_appendCharacters(path, "firstDecision_approximate.csv");
  output.file = fopenFromString(path, "w");
  csv_dataFromMatrixIntColumn(&output, policies, 0);
  fclose(output.file);
    
  
  /* Destroy everything */
  
  arrayFloat_destroy(returns);
  matrixInt_destroy(policies);
  string_destroy(path);

  DONE;
}

