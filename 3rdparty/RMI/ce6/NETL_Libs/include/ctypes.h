
/*
 *    File: include/ctypes.h
 *  Author: Eric DeVolder
 * Purpose: Standard C integer types of specific widths
 *   Notes:
 *
 */

#ifndef _CTYPES_H
#define _CTYPES_H

#if DEBUG != 1
// DEBUG define is only valid if == 1
#undef DEBUG
#endif

#ifdef __cplusplus
extern "C" {
#endif

#if defined(WIN32) || defined(_WIN32_WCE)

#ifndef int8_t
#define int8_t signed char
#endif

#ifndef int16_t
#define int16_t signed short
#endif

#ifndef int32_t
#define int32_t signed int
#endif

#ifndef int64_t
#define int64_t signed long long
#endif

#ifndef uint8_t
#define uint8_t unsigned char
#endif

#ifndef uint16_t
#define uint16_t unsigned short
#endif

#ifndef uint32_t
#define uint32_t unsigned int
#endif

#ifndef uint64_t
#define uint64_t unsigned long long
#endif

#else

#include <stdint.h>

#endif


typedef int8_t  int8;
typedef int16_t int16;
typedef int32_t int32;
typedef int64_t int64;

typedef uint8_t  uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;
typedef uint64_t uint64;

#ifdef __cplusplus
}
#endif

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

#ifndef NULL
#define NULL ((void *)0)
#endif

#ifndef phys_t
#define phys_t uint32
#endif

#endif // _CTYPES_H

