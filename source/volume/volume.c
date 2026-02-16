/******************************************/
/* Implementation of volume class methods */
/******************************************/

#include "volume.h"
#include "volume_private.h"
#include "tools.h"

#include <stdlib.h>  // malloc() and free()

#include <assert.h>
Int _volume_maxWidth = LARGEST_TABLE;
Int _volume_maxHeight = LARGEST_TABLE;
Int _volume_maxDepth = LARGEST_TABLE;

Int _volumeInt_maxValue = LARGEST_INT;
Int _volumeInt_minValue = SMALLEST_INT;

Float _volumeFloat_maxValue = LARGEST_FLOAT;
Float _volumeFloat_minValue = SMALLEST_FLOAT;

#define ASSERT_VOLUME(volume) \
  assert(volume); assert(volume->elements); \
  assert(volume->width > 0); assert(volume->width < _volume_maxWidth);	\
  assert(volume->height > 0); assert(volume->height < _volume_maxHeight); \
  assert(volume->depth > 0); assert(volume->depth < _volume_maxDepth);

#define ASSERT_VOLUME_INDEX_X(volume, x) \
  assert(x >= 0); assert(x < volume->width);

#define ASSERT_VOLUME_INDEX_Y(volume, y) \
  assert(y >= 0); assert(y < volume->height);

#define ASSERT_VOLUME_INDEX_Z(volume, z) \
  assert(z >= 0); assert(z < volume->depth);

#define ASSERT_VOLUME_VALUE_INT(value) \
  assert(value > _volumeInt_minValue); assert(value < _volumeInt_maxValue);

#define ASSERT_VOLUME_VALUE_FLOAT(value) \
  assert(value > _volumeFloat_minValue); assert(value < _volumeFloat_maxValue);

#define ESTABLISH_VOLUME_OFFSET(volume, x, y, z) \
  Int W = volume->width; \
  Int H = volume->height; \
  Int D = volume->depth; \
  Int offset = ((x)*H + (y))*D + (z);
  


/***********************************************************/
/* Volume with integer-valued elements (prefix: volumeInt) */
/***********************************************************/

/* Constructors */

// Creation
VolumeInt* __volumeInt_allocate(void) {
  return malloc(sizeof(VolumeInt));
}

void __volumeInt_initialize(VolumeInt* volume, Int width, Int height, Int depth) {
  assert(volume);
  
  assert(width > 0); assert(width < _volume_maxWidth);
  assert(height > 0); assert(height < _volume_maxHeight);
  assert(depth > 0); assert(depth < _volume_maxDepth);

  // Set the size attributes
  volume->width = width;
  volume->height = height;
  volume->depth = depth;

  // Allocate space for the elements and initialize them to zero
  volume->elements = malloc(sizeof(Int) * width * height * depth);
  for (Int i = 0; i < width * height * depth; i += 1) {
    *(volume->elements + i) = 0;
  }
}

VolumeInt* volumeInt_create(Int width, Int height, Int depth) {
  VolumeInt* volume = __volumeInt_allocate();
  __volumeInt_initialize(volume, width, height, depth);
  return volume;
}

// Destruction
void __volumeInt_deinitialize(VolumeInt* volume) {
  ASSERT_VOLUME(volume);
  
  // Destroy the elements
  free(volume->elements);
}

void __volumeInt_deallocate(VolumeInt* volume) {
  assert(volume);  // volume->elements should already be gone
  free(volume);
}

void volumeInt_destroy(VolumeInt* volume) {
  __volumeInt_deinitialize(volume);
  __volumeInt_deallocate(volume);
}

// Copy
void volumeInt_copy(VolumeInt* destination, VolumeInt* source) {
  ASSERT_VOLUME(destination);
  ASSERT_VOLUME(source);

  Int width = minInt(destination->width, source->width);
  assert(width > 0); assert(width < _volume_maxWidth);
  
  Int height = minInt(destination->height, source->height);
  assert(height > 0); assert(height < _volume_maxHeight);
  
  Int depth = minInt(destination->depth, source->depth);
  assert(depth > 0); assert(depth < _volume_maxDepth);

  for (Int x = 0; x < width; x += 1) {
    for (Int y = 0; y < height; y += 1) {
      for (Int z = 0; z < depth; z += 1) {
	volumeInt_setAt(destination, x, y, z,
			volumeInt_getAt(source, x, y, z));
      }
    }
  }
}


/* Set and get */

void volumeInt_setAt(VolumeInt* volume, Int x, Int y, Int z, Int value) {
  ASSERT_VOLUME(volume);

  ASSERT_VOLUME_INDEX_X(volume, x);
  ASSERT_VOLUME_INDEX_Y(volume, y);
  ASSERT_VOLUME_INDEX_Z(volume, z);

  ASSERT_VOLUME_VALUE_INT(value);

  ESTABLISH_VOLUME_OFFSET(volume, x, y, z);
  *(volume->elements + offset) = value;
}

Int volumeInt_getAt(VolumeInt* volume, Int x, Int y, Int z) {
  ASSERT_VOLUME(volume);

  ASSERT_VOLUME_INDEX_X(volume, x);
  ASSERT_VOLUME_INDEX_Y(volume, y);
  ASSERT_VOLUME_INDEX_Z(volume, z);

  ESTABLISH_VOLUME_OFFSET(volume, x, y, z);
  return *(volume->elements + offset);
}

void volumeInt_setAll(VolumeInt* volume, Int value) {
  ASSERT_VOLUME(volume);
  ASSERT_VOLUME_VALUE_INT(value);

  /* No strides necessary for this one, just do a 1d loop */
  for (Int i = 0; i < (volume->width)*(volume->height)*(volume->depth); i += 1) {
    *(volume->elements + i) = value;
  }
}

Int volumeInt_getWidth(VolumeInt* volume) {
  ASSERT_VOLUME(volume);
  return volume->width;
}

Int volumeInt_getHeight(VolumeInt* volume) {
  ASSERT_VOLUME(volume);
  return volume->height;
}

Int volumeInt_getDepth(VolumeInt* volume) {
  ASSERT_VOLUME(volume);
  return volume->depth;
}


/* Print to console */

void volumeInt_printAt(VolumeInt* volume, Int x, Int y, Int z) {
  ASSERT_VOLUME(volume);

  ASSERT_VOLUME_INDEX_X(volume, x);
  ASSERT_VOLUME_INDEX_Y(volume, y);
  ASSERT_VOLUME_INDEX_Z(volume, z);
  
  Int value = volumeInt_getAt(volume, x, y, z);
  ASSERT_VOLUME_VALUE_INT(value);
  
  #ifdef __APPLE__
  printf("V_{%lld,%lld,%lld} = %lld\n", x, y, z, value);

  #elif __linux__
  printf("V_{%ld,%ld,%ld} = %ld\n", x, y, z, value);

  #else
  printf("Error: Failed to identify operating system.\n");
  assert(FALSE);
  
  #endif /* Platform check */
}




/********************************************************************/
/* Volume with floating-point-valued elements (prefix: volumeFloat) */
/********************************************************************/

/* Constructors */

// Creation
VolumeFloat* __volumeFloat_allocate(void) {
  return malloc(sizeof(VolumeFloat));
}

void __volumeFloat_initialize(VolumeFloat* volume, Int width, Int height, Int depth) {
  assert(volume);
  
  assert(width > 0); assert(width < _volume_maxWidth);
  assert(height > 0); assert(height < _volume_maxHeight);
  assert(depth > 0); assert(depth < _volume_maxDepth);

  // Set the size attributes
  volume->width = width;
  volume->height = height;
  volume->depth = depth;

  // Allocate space for the elements and initialize them to zero
  volume->elements = malloc(sizeof(Float) * width * height * depth);
  for (Int i = 0; i < width * height * depth; i += 1) {
    *(volume->elements + i) = 0;
  }
}

VolumeFloat* volumeFloat_create(Int width, Int height, Int depth) {
  VolumeFloat* volume = __volumeFloat_allocate();
  __volumeFloat_initialize(volume, width, height, depth);
  return volume;
}

// Destruction
void __volumeFloat_deinitialize(VolumeFloat* volume) {
  ASSERT_VOLUME(volume);
  
  // Destroy the elements
  free(volume->elements);
}

void __volumeFloat_deallocate(VolumeFloat* volume) {
  assert(volume);  // volume->elements should already be gone
  free(volume);
}

void volumeFloat_destroy(VolumeFloat* volume) {
  __volumeFloat_deinitialize(volume);
  __volumeFloat_deallocate(volume);
}

// Copy
void volumeFloat_copy(VolumeFloat* destination, VolumeFloat* source) {
  ASSERT_VOLUME(destination);
  ASSERT_VOLUME(source);

  Int width = minInt(destination->width, source->width);
  assert(width > 0); assert(width < _volume_maxWidth);
  
  Int height = minInt(destination->height, source->height);
  assert(height > 0); assert(height < _volume_maxHeight);
  
  Int depth = minInt(destination->depth, source->depth);
  assert(depth > 0); assert(depth < _volume_maxDepth);

  for (Int x = 0; x < width; x += 1) {
    for (Int y = 0; y < height; y += 1) {
      for (Int z = 0; z < depth; z += 1) {
	volumeFloat_setAt(destination, x, y, z,
			  volumeFloat_getAt(source, x, y, z));
      }
    }
  }
}

/* Set and get */

void volumeFloat_setAt(VolumeFloat* volume, Int x, Int y, Int z, Float value) {
  ASSERT_VOLUME(volume);

  ASSERT_VOLUME_INDEX_X(volume, x);
  ASSERT_VOLUME_INDEX_Y(volume, y);
  ASSERT_VOLUME_INDEX_Z(volume, z);

  ASSERT_VOLUME_VALUE_FLOAT(value);

  ESTABLISH_VOLUME_OFFSET(volume, x, y, z);
  *(volume->elements + offset) = value;
}

Float volumeFloat_getAt(VolumeFloat* volume, Int x, Int y, Int z) {
  ASSERT_VOLUME(volume);
  
  ASSERT_VOLUME_INDEX_X(volume, x);
  ASSERT_VOLUME_INDEX_Y(volume, y);
  ASSERT_VOLUME_INDEX_Z(volume, z);

  ESTABLISH_VOLUME_OFFSET(volume, x, y, z);
  return *(volume->elements + offset);
}

void volumeFloat_setAll(VolumeFloat* volume, Float value) {
  ASSERT_VOLUME(volume);
  ASSERT_VOLUME_VALUE_FLOAT(value);

  /* No strides necessary for this one, just do a 1d loop */
  for (Int i = 0; i < (volume->width)*(volume->height)*(volume->depth); i += 1) {
    *(volume->elements + i) = value;
  }
}

Int volumeFloat_getWidth(VolumeFloat* volume) {
  ASSERT_VOLUME(volume);
  return volume->width;
}

Int volumeFloat_getHeight(VolumeFloat* volume) {
  ASSERT_VOLUME(volume);
  return volume->height;
}

Int volumeFloat_getDepth(VolumeFloat* volume) {
  ASSERT_VOLUME(volume);
  return volume->depth;
}


/* Print to console */

void volumeFloat_printAt(VolumeFloat* volume, Int x, Int y, Int z) {
  ASSERT_VOLUME(volume);

  ASSERT_VOLUME_INDEX_X(volume, x);
  ASSERT_VOLUME_INDEX_Y(volume, y);
  ASSERT_VOLUME_INDEX_Z(volume, z);
  
  Float value = volumeFloat_getAt(volume, x, y, z);
  ASSERT_VOLUME_VALUE_FLOAT(value);
  
  #ifdef __APPLE__
  printf("V_{%lld,%lld,%lld} = %0.4f\n", x, y, z, value);

  #elif __linux__
  printf("V_{%ld,%ld,%ld} = %0.4f\n", x, y, z, value);

  #else
  printf("Error: Failed to identify operating system.\n");
  assert(FALSE);
  
  #endif /* Platform check */
}
