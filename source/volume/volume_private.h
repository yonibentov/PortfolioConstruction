/******************************************************/
/* Implementation of volume class attribute structure */
/******************************************************/

#ifndef VOLUME_PRIVATE_H
#define VOLUME_PRIVATE_H

#include "common.h"


/***************************************/
/* Volume with integer-valued elements */
/***************************************/

typedef struct VolumeInt {
  Int* elements;  // Internally, the volume will be a flattened array with strides
  Int width;
  Int height;
  Int depth;
} VolumeInt;


/**********************************************/
/* Volume with floating-point-valued elements */
/**********************************************/

typedef struct VolumeFloat {
  Float* elements;
  Int width;
  Int height;
  Int depth;
} VolumeFloat;


#endif /* VOLUME_PRIVATE_H */
