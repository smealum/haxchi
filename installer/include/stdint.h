#ifndef _STDINT_WRAP_H
#define _STDINT_WRAP_H

#include <../../powerpc-eabi/include/stdint.h>

#define int32_t my_int32_t
#define uint32_t my_uint32_t
typedef signed int int32_t;
typedef unsigned int uint32_t;

#endif