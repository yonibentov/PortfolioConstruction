/* Interface for miscellaneous functions */

#ifndef COMMON_TOOLS_H
#define COMMON_TOOLS_H

#include "common.h"

// Check whether an integer is even or odd
Bool isEven(Int n);
Bool isOdd(Int n);


// Compare two numbers
Int minInt(Int x, Int y);
Float minFloat(Float x, Float y);
Int maxInt(Int x, Int y);
Float maxFloat(Float x, Float y);

Float relativeDifferenceFloat(Float new, Float old);


// Function types
typedef Int Function_IntToInt(Int);
typedef Float Function_IntToFloat(Int);
typedef Float Function_FloatToFloat(Float);


// Extremize a function
void function_IntToInt_minimize(Function_IntToInt* function,  // input
				Int domainMin,  // input
				Int domainMax,  // input
				Int stepSize,  // input
				Int* minimum,  // output
				Int* argument);  // output
void function_IntToInt_maximize(Function_IntToInt* function,  // input
				Int domainMin,  // input
				Int domainMax,  // input
				Int stepSize,  // input
				Int* maximum,  // output
				Int* argument);  // output

void function_IntToFloat_minimize(Function_IntToFloat* function,  // input
				  Int domainMin,  // input
				  Int domainMax,  // input
				  Int stepSize,  // input
				  Float* minimum,  // output
				  Int* argument);  // output
void function_IntToFloat_maximize(Function_IntToFloat* function,  // input
				  Int domainMin,  // input
				  Int domainMax,  // input
				  Int stepSize,  // input
				  Float* maximum,  // output
				  Int* argument);  // output

void function_FloatToFloat_minimize(Function_FloatToFloat* function,  // input
				    Float domainMin,  // input
				    Float domainMax,  // input
				    Float stepSize,  // input
				    Float* minimum,  // output
				    Float* argument);  // output
void function_FloatToFloat_maximize(Function_FloatToFloat* function,  // input
				    Float domainMin,  // input
				    Float domainMax,  // input
				    Float stepSize,  // input
				    Float* maximum,  // output
				    Float* argument);  // output



#endif /* COMMON_TOOLS_H */
