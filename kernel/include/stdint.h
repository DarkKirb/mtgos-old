#pragma once
#define __NATIVE_TYPE__ long /* SHould be accurate */
#define __INT8_TYPE__ char
#define __INT16_TYPE__ short
#define __INT32_TYPE__ int
#define __INT64_TYPE__ long long
#define __UINT8_TYPE__ unsigned char
#define __UINT16_TYPE__ unsigned short
#define __UINT32_TYPE__ unsigned int
#define __UINT64_TYPE__ unsigned long long
#define __INT_LEAST8_TYPE__ __INT8_TYPE__
#define __INT_LEAST16_TYPE__ __INT16_TYPE__
#define __INT_LEAST32_TYPE__ __INT32_TYPE__
#define __INT_LEAST64_TYPE__ __INT64_TYPE__
#define __UINT_LEAST8_TYPE__ __UINT8_TYPE__
#define __UINT_LEAST16_TYPE__ __UINT16_TYPE__
#define __UINT_LEAST32_TYPE__ __UINT32_TYPE__
#define __UINT_LEAST64_TYPE__ __UINT64_TYPE__
#define __INT_FAST8_TYPE__ __INT8_TYPE__  /* For chars */
#define __INT_FAST16_TYPE__ __NATIVE_TYPE__
#define __INT_FAST32_TYPE__ __NATIVE_TYPE__
#define __INT_FAST64_TYPE__ __INT64_TYPE__
#define __UINT_FAST8_TYPE__ unsigned __NATIVE_TYPE__
#define __UINT_FAST16_TYPE__ unsigned __NATIVE_TYPE__
#define __UINT_FAST32_TYPE__ unsigned __NATIVE_TYPE__
#define __UINT_FAST64_TYPE__ __UINT64_TYPE__
#define __UINTPTR_TYPE__ unsigned __NATIVE_TYPE__
#if __x86_64__ || __ppc64__
#define __INTMAX_TYPE__ __int128
#define __UINTMAX_TYPE__ unsigned __int128
#define __NATIVE_MAX__ 9223372036854775807ll
#define __UNATIVE_MAX__ 18446744073709551615ull
#define __INTNATIVE_MAX__ ((((__int128)(0x7FFFFFFFFFFFFFFFull))<<64)|0xFFFFFFFFFFFFFFFFull)
#define __INTNATIVE_MAX__ ((((unsigned __int128)(0xFFFFFFFFFFFFFFFFull))<<64)|0xFFFFFFFFFFFFFFFFull)
#else
#define __INTMAX_TYPE__ long long
#define __UINTMAX_TYPE__ unsigned long long
#define __NATIVE_MAX__ 2147483647
#define __UNATIVE_MAX__ 4294967295
#define __INTMAX_MAX__ 9223372036854775807ll
#define __UINTMAX_MAX__ 18446744073709551615ull
#endif
#define __INT8_MAX__ 127
#define __UINT8_MAX__ 255
#define __INT16_MAX__ 32767
#define __UINT16_MAX__ 65535
#define __INT32_MAX__ 2147483647
#define __UINT32_MAX__ 4294967295
#define __INT64_MAX__ 9223372036854775807ll
#define __UINT32_MAX__ 18446744073709551615ull
#define __INT_LEAST8_MAX__ __INT8_MAX__
#define __INT_LEAST16_MAX__ __INT16_MAX__
#define __INT_LEAST32_MAX__ __INT32_MAX__
#define __INT_LEAST64_MAX__ __INT64_MAX__
#define __UINT_LEAST8_MAX__ __UINT8_MAX__
#define __UINT_LEAST16_MAX__ __UINT16_MAX__
#define __UINT_LEAST32_MAX__ __UINT32_MAX__
#define __UINT_LEAST64_MAX__ __UINT64_MAX__
#define __INT_FAST8_MAX__ __INT8_MAX__
#define __INT_FAST16_MAX__ __NATIVE_MAX__
#define __INT_FAST32_MAX__ __NATIVE_MAX__
#define __INT_FAST64_MAX__ __NATIVE_MAX__
#define __UINT_FAST8_MAX__ __UNATIVE_MAX__
#define __UINT_FAST16_MAX__ __UNATIVE_MAX__
#define __UINT_FAST32_MAX__ __UNATIVE_MAX__
#define __UINT_FAST64_MAX__ __UNATIVE_MAX__
#define __UINTPTR_MAX__ __UNATIVE_MAX__
#ifndef __arm__
typedef unsigned __NATIVE_TYPE__ size_t;
#else
typedef unsigned int size_t;
#endif
#include <stdint-gcc.h>
