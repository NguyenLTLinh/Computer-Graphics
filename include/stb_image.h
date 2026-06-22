// stb_image.h - v2.29 - public domain image loader
// http://nothings.org/stb
//
// QUICK REFERENCE:
//   int x,y,n;
//   unsigned char *data = stbi_load(filename, &x, &y, &n, 0);
//   // ... use the data ...
//   stbi_image_free(data);
//
// Use STB_IMAGE_IMPLEMENTATION before including to get implementation.
// In your project, put this in ONE .cpp file:
//   #define STB_IMAGE_IMPLEMENTATION
//   #include "stb_image.h"
//
// Download the full header from: https://github.com/nothings/stb/blob/master/stb_image.h
// This file is a stub/placeholder - replace with the actual stb_image.h

#ifndef STBI_INCLUDE_STB_IMAGE_H
#define STBI_INCLUDE_STB_IMAGE_H

// NOTE FOR DEVELOPERS:
// Download the actual stb_image.h from:
// https://raw.githubusercontent.com/nothings/stb/master/stb_image.h
// and replace this file.

#ifdef STB_IMAGE_IMPLEMENTATION
// Placeholder - actual implementation comes from downloading stb_image.h
#endif

#ifndef STBIDEF
#ifdef STB_IMAGE_STATIC
#define STBIDEF static
#else
#define STBIDEF extern
#endif
#endif

#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef unsigned char stbi_uc;
typedef unsigned short stbi_us;

STBIDEF stbi_uc *stbi_load               (char              const *filename,           int *x, int *y, int *channels_in_file, int desired_channels);
STBIDEF stbi_uc *stbi_load_from_memory   (stbi_uc           const *buffer, int len   , int *x, int *y, int *channels_in_file, int desired_channels);
STBIDEF void     stbi_image_free         (void *retval_from_stbi_load);
STBIDEF void     stbi_set_flip_vertically_on_load(int flag_true_if_should_flip);
STBIDEF const char *stbi_failure_reason  (void);

#ifdef __cplusplus
}
#endif

#endif // STBI_INCLUDE_STB_IMAGE_H
