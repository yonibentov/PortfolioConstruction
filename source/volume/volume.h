/*****************************/
/* Interface of volume class */
/*****************************/

#ifndef VOLUME_H
#define VOLUME_H

#include "common.h"


/***********************************************************/
/* Volume with integer-valued elements (prefix: volumeInt) */
/***********************************************************/

/* Attribute structure */
typedef struct VolumeInt VolumeInt;

/* Class methods */

// Constructors
VolumeInt* volumeInt_create(Int width, Int height, Int depth);
void volumeInt_destroy(VolumeInt* volume);

void volumeInt_copy(VolumeInt* destination, VolumeInt* source);

// Set and get
void volumeInt_setAt(VolumeInt* volume, Int x, Int y, Int z, Int value);
Int volumeInt_getAt(VolumeInt* volume, Int x, Int y, Int z);

void volumeInt_setAll(VolumeInt* volume, Int value);

Int volumeInt_getWidth(VolumeInt* volume);
Int volumeInt_getHeight(VolumeInt* volume);
Int volumeInt_getDepth(VolumeInt* volume);

// Print to console
void volumeInt_printAt(VolumeInt* volume, Int x, Int y, Int z);


/********************************************************************/
/* Volume with floating-point-valued elements (prefix: volumeFloat) */
/********************************************************************/

/* Attribute structure */
typedef struct VolumeFloat VolumeFloat;

/* Class methods */

// Constructors
VolumeFloat* volumeFloat_create(Int width, Int height, Int depth);
void volumeFloat_destroy(VolumeFloat* volume);

void volumeFloat_copy(VolumeFloat* destination, VolumeFloat* source);

// Set and get
void volumeFloat_setAt(VolumeFloat* volume, Int x, Int y, Int z, Float value);
Float volumeFloat_getAt(VolumeFloat* volume, Int x, Int y, Int z);

void volumeFloat_setAll(VolumeFloat* volume, Float value);

Int volumeFloat_getWidth(VolumeFloat* volume);
Int volumeFloat_getHeight(VolumeFloat* volume);
Int volumeFloat_getDepth(VolumeFloat* volume);

// Print to console
void volumeFloat_printAt(VolumeFloat* volume, Int x, Int y, Int z);



#endif /* VOLUME_H */
